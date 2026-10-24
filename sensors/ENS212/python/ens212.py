#!/usr/bin/env python3

"""Read temperature and humidity from a ScioSense ENS212 (ENS21x family) over I2C."""

import argparse
import time

from smbus2 import SMBus, i2c_msg

ADDR = 0x45

PART_ID = 0x00
SYS_CTRL = 0x10
SYS_STAT = 0x11
SENS_RUN = 0x21
SENS_START = 0x22
# T_VAL (3 bytes) followed by H_VAL (3 bytes)
T_VAL = 0x30

# single-shot T+H conversion, with margin
CONVERSION_S = 0.15


def crc7(val: int) -> int:
    """CRC-7 (poly 0x89, init 0x7F) over 17-bit payload, as in the ENS21x datasheet."""
    width, poly, ivec, dwidth = 7, 0x89, 0x7F, 17
    pol = poly << (dwidth - width - 1) << width
    bit = (1 << (dwidth - 1)) << width
    mask = ((1 << dwidth) - 1) << width
    val = (val << width) | ivec
    while bit & mask:
        if bit & val:
            val ^= pol
        bit >>= 1
        pol >>= 1
    return val


def decode(raw: bytes) -> int:
    word = raw[0] | raw[1] << 8 | raw[2] << 16
    data, valid, crc = word & 0xFFFF, (word >> 16) & 1, (word >> 17) & 0x7F
    if not valid:
        raise ValueError("measurement not valid")
    if crc7(word & 0x1FFFF) != crc:
        raise ValueError("CRC mismatch")
    return data


class ENS212:
    def __init__(self, bus: int = 1, addr: int = ADDR):
        self.bus = SMBus(bus)
        self.addr = addr
        self._init()

    def _read(self, reg: int, n: int) -> bytes:
        w, r = i2c_msg.write(self.addr, [reg]), i2c_msg.read(self.addr, n)
        self.bus.i2c_rdwr(w, r)
        return bytes(r)

    def _write(self, reg: int, val: int) -> None:
        self.bus.write_byte_data(self.addr, reg, val)

    def _init(self) -> None:
        self._write(SYS_CTRL, 0x80)  # reset
        time.sleep(0.01)
        self._write(SYS_CTRL, 0x00)  # disable low-power so it stays active
        time.sleep(0.01)
        pid = int.from_bytes(self._read(PART_ID, 2), "little")
        if pid & 0xFF00 != 0x0200:
            raise RuntimeError(f"Unexpected part ID 0x{pid:04X}")
        print(f"Found ENS21x part ID 0x{pid:04X}")

    def read(self) -> tuple[float, float]:
        self._write(SENS_RUN, 0x00)    # single-shot for T and H
        self._write(SENS_START, 0x03)  # start T and H
        time.sleep(CONVERSION_S)
        raw = self._read(T_VAL, 6)
        temp_c = decode(raw[0:3]) / 64.0 - 273.15
        hum = decode(raw[3:6]) / 512.0
        return temp_c, hum

    def close(self) -> None:
        self.bus.close()


def main() -> None:
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("-b", "--bus", type=int, default=1)
    p.add_argument("-a", "--addr", type=lambda x: int(x, 0), default=ADDR)
    p.add_argument("-i", "--interval", type=float, default=2.0, help="seconds; 0 = read once")
    args = p.parse_args()

    sensor = ENS212(args.bus, args.addr)
    try:
        while True:
            try:
                t, h = sensor.read()
                print(f"{time.strftime('%H:%M:%S')}  {t:6.2f} °C  {h:6.2f} %RH", flush=True)
            except (ValueError, OSError) as e:
                print(f"read error: {e}")
            if args.interval <= 0:
                break
            time.sleep(args.interval)
    except KeyboardInterrupt:
        pass
    finally:
        sensor.close()


if __name__ == "__main__":
    main()
