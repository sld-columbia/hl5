#!/usr/bin/env python

__author__      = "Paolo Mantovani"
__copyright__   = "Copyright 2017, Columbia University, NY"
__credits__     = "Giuseppe Di Guglielmo"
__license__     = "DO NOT DISTRIBUTE!"
__maintainer__  = "Paolo Mantovani"
__email__       = "paolo@cs.columbia.edu"
__status__      = "Testing"

import sys

if len(sys.argv) != 2:
  print "Usage: ./srec2text.py <input_file>"
  print ""
  print "  <input_file>   : Object in SREC format"
  print ""
  sys.exit(0)

try:
  fd = open(sys.argv[1], 'r')
except:
  print "Error: failed to open " + sys.argv[1]
  sys.exit(0)

current_address = 0;

for line in fd:
  if not line.strip():
    continue

  entry = line.strip()

  s_type = int(entry[1])
  byte_cnt = 0
  data_start = 0

  if s_type == 0:
    # Header (skip)
    continue

  elif s_type == 5 or s_type == 6:
    # Optional S1/S2/S3 count (skip)
    continue

  elif s_type == 1 or s_type == 9:
    # 16-bit address entry
    byte_cnt = int(entry[2] + entry[3], 16) - 3
    data_start = 8

  elif s_type == 2 or s_type == 8:
    # 24-bit address entry
    byte_cnt = int(entry[2] + entry[3], 16) - 4
    data_start = 10

  elif s_type == 3 or s_type == 7:
    # 32-bit address entry
    byte_cnt = int(entry[2] + entry[3], 16) - 5
    data_start = 12

  else:
    # Unexpected entry
    print "Error: SREC decoding error -> \"" + entry + "\""
    sys.exit(0)

  address = int(entry[4:data_start], 16)

  if s_type == 7 or s_type == 8 or s_type == 9:
    # print str(hex(current_address)) + " 0xFFFFFFFF"
    # print str(hex(address))
    continue

  if current_address != 0 and address != current_address:
    print "Error: STRC skipping addresses -> \"" + entry + "\""
    sys.exit(0)

  current_address = address

  for i in range(0, byte_cnt / 4):
    little_end = entry[data_start + i * 8:data_start + 8 + i * 8]
    big_end = little_end[6] + little_end[7] + little_end[4] + little_end[5] + little_end[2] + little_end[3] + little_end[0] + little_end[1]
    print str(hex(current_address)) + " 0x" + big_end
    current_address = current_address + 4
