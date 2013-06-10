#!/usr/bin/python
from argparse import ArgumentParser
from sys import stdin, stdout
from cgi import escape

headers = ("input", "expected output", "actual output",
           "reason for test", "reason for difference")

def print_row(outfile, row, isHeader=False):
    tag = "th" if isHeader else "td"
    outfile.write("<tr>")
    for cell in row:
        outfile.write("<{0}>{1}</{0}>".format(tag, escape(cell)))
    if len(row) < len(headers):
        for i in range(len(row), len(headers) - 1):
            outfile.write("<td />")
        outfile.write("<td>N/A</td>")
    outfile.write("</tr>\n")

def print_declaration(outfile, declaration):
    outfile.write("<tr><th class=\"declaration\" colspan=5>" +
            declaration[0] + "</th></tr>\n")

if __name__ == "__main__":
    parser = ArgumentParser(description="Formats a test plan as html.")
    parser.add_argument('infile', nargs='?', help="The output from"
            + " parse_test.py. Reads from stdin if not supplied.")
    parser.add_argument('outfile', nargs='?', help="The output file."
            + " Writes to stdout if not supplied.")
    args = parser.parse_args()
    infile = open(args.infile, "r") if args.infile != None else stdin
    outfile = open(args.outfile, "w") if args.outfile != None else stdout

    outfile.write("""<html><head><style type="text/css">
            table { border-collapse: collapse; }
            th,td { padding: 3px; font-size: 9pt; }
            .declaration { text-align: left; }
            </style></head><body><table border="1">\n""")
    rows = [block.strip().split("\n")
                for block in infile.read().split("\n\n")]
    if infile is not stdin:
        infile.close()
    print_row(outfile, headers, True)
    for row in rows:
        if len(row) == 1:
            print_declaration(outfile, row)
        else:
            print_row(outfile, row)
    outfile.write("</table></body></html>\n")
    if outfile is not stdout:
        outfile.close()
