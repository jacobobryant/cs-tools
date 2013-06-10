#!/usr/bin/python
from argparse import ArgumentParser
import sys

parser = ArgumentParser(description="Converts the output from a " +
        "driver into the correct format for format_plan.py.")
parser.add_argument('infile', nargs='?', help="The driver output."
        + " Reads from stdin in not supplied.")
parser.add_argument('outfile', nargs='?', help="The output file."
        + " Writes to stdout if not supplied.")
args = parser.parse_args()

infile = open(args.infile, 'r') if args.infile != None else sys.stdin
outfile = open(args.outfile, "w") if args.outfile != None else sys.stdout
groupComment = ""
for line in infile:
    if line.lstrip()[:2] == "//":
        groupComment = line.lstrip()[2:].strip()
    elif line.find(";\n") != -1:
        print(line, file=outfile)
    elif line.find(": ") != -1:
        groups = line.split(": ", 1)
        input = groups[0].strip()
        groups = groups[1].split("//", 1)
        output = groups[0].strip()
        comment = groups[1].strip() if len(groups) > 1 else groupComment
        print(input, output, '"', comment, "N/A", sep="\n", end="\n\n",
                file=outfile)
if infile is not sys.stdin:
    infile.close()
if outfile is not sys.stdout:
    outfile.close()
