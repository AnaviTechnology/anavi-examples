#!/usr/bin/env python
#
# Simple IR Packet Reader
#   Copyleft 2018 - Georgi Kolev
#
#
#
# $ ./ir_packet_read.py -d /dev/lirc0
#
#  Packet:
#    BIN: 01111100 10000011 01000000 11111111 00000000 01011000 00000000 11000000 00011000 10111111 00100000 11010000 00111111 11000000 00010110 00000000 00110000 00000110
#    HEX:   0x7C     0x83     0x40     0xFF     0x00     0x58     0x00     0xC0     0x18     0xBF     0x20     0xD0     0x3F     0xC0     0x16     0x00     0x30     0x06     0x02
#  Repeating pattern:
#    HEX:   0x7C     0x83     0x08
#    Bin: 01111100 10000011
#
#
#  Packet:
#   BIN: 11111001 00000110 10000001 11111110 00000000 10111000 00000001 10000000 00111001 01111110 01000001 10100000 01111111 10000000 00101110 00000000 01100000 00001110
#   HEX:   0xF9     0x06     0x81     0xFE     0x00     0xB8     0x01     0x80     0x39     0x7E     0x41     0xA0     0x7F     0x80     0x2E     0x00     0x60     0x0E     0x02
#  Repeating pattern:
#   HEX:   0xF9     0x06     0x81     0xFE     0x00     0xB8     0x01     0x80     0x39
#   Bin: 11111001 00000110 10000001 11111110 00000000 10111000 00000001 10000000 00111001
#
#
#  Packet:
#   BIN: 11111001 00000110 10000001 11111110 00000000 10110000 00000001 10000000 00110001 01111110 01000001 10100000 01111111 10000000 00101100 00000000 01100000 00001100
#   HEX:   0xF9     0x06     0x81     0xFE     0x00     0xB0     0x01     0x80     0x31     0x7E     0x41     0xA0     0x7F     0x80     0x2C     0x00     0x60     0x0C     0x02
#  Repeating pattern:
#   HEX:   0xF9     0x06     0x81     0xFE     0x00     0xB0     0x01     0x80     0x31
#   Bin: 11111001 00000110 10000001 11111110 00000000 10110000 00000001 10000000 00110001



import os
import sys
import argparse
import subprocess

MIN_LEN = 4
SPACE_ONE = 1000
MAX_SPACE = 15000


def start_process(device):

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

            print('Packet:')
            print(' BIN: %s' % pretty_bin(bin_string))
            print(' HEX: %s' % ' '.join(bin2hex(bin_string)))

            if len(pattern) > MIN_LEN:
                print('Repeating pattern:')
                print('  HEX: %s' % ' '.join(bin2hex(pattern)))
                print('  Bin: %s\n' % pretty_bin(pattern))
            print('')
            bin_string = ''
        else:
            continue
    return


def pretty_bin(b):
    return ' '.join(
        map(''.join, zip(*[iter(b)] * 8))
    )


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
args = parser.parse_args()

if args.device:
    start_process(args.device)
else:
    print("No device specified.")
    sys.exit(0)
