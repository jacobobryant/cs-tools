#!/usr/bin/python

import sys

infile = open(sys.argv[1], 'r') if len(sys.argv) > 1 else sys.stdin
groupComment = ""
for line in infile:
    if line.lstrip()[:2] == "//":
        groupComment = line.lstrip()[2:].strip()
    elif line.find(";\n") != -1:
        print(line)
    elif line.find(": ") != -1:
        groups = line.split(": ", 1)
        input = groups[0].strip()
        groups = groups[1].split("//", 1)
        output = groups[0].strip()
        comment = groups[1].strip() if len(groups) > 1 else groupComment
        print(input, output, '"', comment, "N/A", sep="\n", end="\n\n")
if infile is not sys.stdin:
    infile.close()
