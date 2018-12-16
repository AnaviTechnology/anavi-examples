#!/usr/bin/env python
#
# Toshiba AC IR Code Decoder
#   Copyleft 2018 - Georgi Kolev
#
# ---------------------------------------------------------------------------------------
# $ ./ac-toshiba.py -d /dev/lirc0 -v
# Trying device: /dev/lirc0
# Using device: /dev/lirc0
# Toshiba AC -  Mode: Hot    Temp: 22   Fan: Auto
#   Hex:   0xF9     0x06     0x81     0xFE     0x00     0xA8     0x01     0x80     0x29
#   Bin: 11111001 00000110 10000001 11111110 00000000 10101000 00000001 10000000 00101001
#
# Toshiba AC -  Mode: Hot    Temp: 23   Fan: Auto
#   Hex:   0xF9     0x06     0x81     0xFE     0x00     0xB0     0x01     0x80     0x31
#   Bin: 11111001 00000110 10000001 11111110 00000000 10110000 00000001 10000000 00110001
#
# Toshiba AC -  Mode: Hot    Temp: 24   Fan: Auto
#   Hex:   0xF9     0x06     0x81     0xFE     0x00     0xB8     0x01     0x80     0x39
#   Bin: 11111001 00000110 10000001 11111110 00000000 10111000 00000001 10000000 00111001
#
# ---------------------------------------------------------------------------------------
# Toshiba AC protocol overview:
#
# There are few types of packets
#  1. 144bits - Power, Mode, Temperature, Fan_speed
#  2. 160bits - Eco Mode/HiPower mode
#  3. 112bits - Swing control
#  4. Others
#
#
# 1. 144bits - Power, Mode, Temperature, Fan speed
#    In reality this is 9bytes (72bit) repeated twice with spacer.
#    You can think of the first 5bytes as static (actually the first 2 are)
#  Mode:        Byte7 bits 6-8
#    .....001 - Cold
#    .....011 - Hot
#    .....000 - Auto
#    .....010 - Dry
#    .....100 - Fan
#    .....111 - Off
#
#  Temperature: Byte6 bits 1-5
#    10000... - 16
#    10001... - 17
#    10010... - 18
#    ...
#    11101... - 29
#    11110... - 30
#
#  Fan speed:   Byte7 Bits1-3
#    000..... - Auto
#    001..... - Quiet
#    010..... - Fan speed 1
#    011..... - Fan speed 2
#    100..... - Fan speed 3
#    101..... - Fan speed 4
#    110..... - Fan speed 5
#
#  Checksum:    Byte9
#    Calculated by ^ bits1 to 7
#
#
# 2. 160bits - Eco/HiPower Mode
#    First 2 bytes are the same as with the 144bits packet. Byte 3 is +1.
#    Checksum is calculated the same way (skiping the last byte that is always 0)
#
#  Eco/HiPower mode:
#    Byte9 - Bit8
#     .......1 - Eco Mode
#     .......0 - HiPower Mode
#
# 3. 122bits - TBA...
#
#

import os
import sys
import argparse
import subprocess


MIN_LEN = 48
SPACE_ONE = 1000
MAX_SPACE = 15000


def start_process(device):

    toshiba = toshiba_ac()
    pattern = ''
    bin_string = ''
    command = 'mode2 -g %s -d %s' % (MAX_SPACE, device)

    process = subprocess.Popen(
        command, shell=True,
        stdout=subprocess.PIPE
    )

    while True:
        sig_res = read_signal(
            process.stdout.readline()
        )
        if sig_res > -1:
            bin_string += str(sig_res)
        elif sig_res == -1:
            if len(bin_string) < MIN_LEN:
                continue

            pattern = pattern_search(bin_string)[:-2]
            bin_string = ''

            if toshiba.packet(pattern):
                print("Toshiba AC -  Mode: %s %s   Temp: %s   Fan: %s" % (
                    toshiba.get_mode(), toshiba.smode,
                    toshiba.get_temp(), toshiba.get_fan())
                )
                if args.verbose:
                    print('  Hex: %s' % ' '.join(bin2hex(toshiba.pkt)))
                    print('  Bin: %s\n' % ' '.join(
                        map(''.join, zip(*[iter(toshiba.pkt)]*8)))
                    )
        else:
            continue
    return


class toshiba_ac():
    def __init__(self):
        self.pkt = ''
        self.pkt_id = '1111100100000110'
        self.hex_pkt = []
        self.modes = {
            '000': 'Auto',
            '001': 'Cold',
            '010': 'Dry',
            '011': 'Hot',
            '100': 'Fan',
            '111': '0ff'
        }

    def packet(self, pkt):
        # Strip any extra bits
        if not pkt.startswith(self.pkt_id) and (len(pkt) > 72):
            pkt = pkt[pkt.find(self.pkt_id):][:72]
        # Valid lenght and checksum is needed to proceed
        if (len(pkt) == 72) and self.checksum(bin2hex(pkt)):
            self.pkt = pkt
            self.smode = ''
            return True
        elif (len(pkt) == 80) and (get_byte(pkt, 2) == 0x82) and self.checksum(bin2hex(pkt)):
            if get_byte(pkt, 8):
                self.smode = ' [ECO]'
            else:
                self.smode = ' [HiPower]'
            return True

        return False

    def get_status(self):
        if len(self.pkt) > 53:
            return self.pkt[54]
        return False

    def get_temp(self):
        if len(self.pkt) > (5 * 8):
            return (get_byte(self.pkt, 5) >> 3) + 1
        return False

    def get_fan(self):
        if len(self.pkt) < (6 * 8):
            return False
        temp_fan = (get_byte(self.pkt, 6) >> 4) - 1
        if temp_fan == -1:
            return 'Auto'
        elif temp_fan == 0:
            return 'Quiet'
        else:
            return temp_fan

    def get_mode(self):
        if len(self.pkt) < 54:
            return False
        if self.pkt[54:][:3] in self.modes:
            return self.modes[self.pkt[54:][:3]]
        return 'Unknown'

    def checksum(self, pkt):
        checksum = 0
        for b in pkt[:-2]:
            checksum ^= int(b, 16)
        if checksum == int(pkt[-1], 16):
            return True
        return False


def get_byte(d, b):
    return int(d[(b * 8):][:8], 2)


def bin2hex(in_str):
    hex_array = []
    while in_str:
        hex_array.append(
            '  0x%.2X  ' % int(in_str[:8], 2)
        )
        in_str = in_str[8:]
    return hex_array


def pattern_search(b_array):
    prefix_array = []

    for i in range(len(b_array)):
        prefix_array.append(b_array[:i])

    for i in prefix_array[:1:-1]:
        if b_array.count(i) > 1:
            offset = b_array[len(i):].find(i)
            return b_array[:len(i)+offset]
            #break
    return b_array


def read_signal(signal):
    if signal.startswith('space') or signal.startswith('pulse'):
        signals = signal.split()
        if int(signals[1]) > MAX_SPACE:
            return -1
        if signals[0] == 'space':
            if int(signals[1]) > SPACE_ONE:
                return 1
            else:
                return 0
        else:
            return -2
    return -1


def array2string(arr):
    return ''.join(str(e) for e in arr)


def check_args():
    if len(sys.argv) != 2:
        print("Usage: ")
        print("  %s <device name>" % sys.argv[0])
        print("  %s /dev/lirc0" % sys.argv[0])
        return False

    if os.path.exists(sys.argv[1]):
        print("device: %s found." % sys.argv[1])
        return True
    else:
        print("%s is not a valid device." % sys.argv[1])
        return False


parser = argparse.ArgumentParser()
parser.add_argument("-d", "--device", type=str, help="device")
parser.add_argument("-v", "--verbose", action="store_true", help="verbose")
args = parser.parse_args()

if args.device:
    start_process(args.device)
else:
    print("No device specified.")
    sys.exit(0)
