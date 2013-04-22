#!/usr/bin/python
import re
import argparse
import sys

KEY_LENGTH = len("Description:")  # That's the longest key.

def hline(func, key, value=''):
    keyString = "{0: <{1}}".format(key + ":", KEY_LENGTH)
    return "{0}// {1} {2}\n".format(func['indent'], keyString, value)

def box_top(func, width):
    TAB_WIDTH = 4
    numTabs = func['indent'].count('\t')
    width = width - numTabs * (TAB_WIDTH - 1)
    return "{0:-<{1}}\n".format(func['indent'] + '//', width)

def params(func):
    args = re.sub(r'\s+', ' ', func['args'])
    args = [arg.strip() for arg in args.split(',')]
    width = max([len(arg) for arg in args]) + len(":")
    argString = lambda arg: "{0: <{1}}".format(arg + ":", width)
    line = lambda arg: func['indent'] + "// " + ' ' * (KEY_LENGTH + 1) + \
            argString(arg) + "\n"

    ret = ""
    if args[0]:
        ret += hline(func, "Parameters", argString(args[0]))
        for arg in args[1:]:
            ret += line(arg)
    return ret

def signature(func):
    args = re.sub(r'\s+', ' ', func['args'])
    signature = "{0}({1})".format(func['name'], args)
    if func['class']:
        return hline(func, "Class", func['class']) + \
                hline(func, "Method", signature)
    else:
        return hline(func, "Function", signature)

def build_header(func, name, width):
    h = ""
    h += box_top(func, width)
    h += signature(func)
    h += hline(func, "Description")
    h += hline(func, "Programmers", name)
    h += hline(func, "Version", "1.0")
    h += hline(func, "Input")
    h += hline(func, "Output")
    h += hline(func, "Calls")
    h += hline(func, "Called by")
    h += params(func)
    if func['type'] != 'void':
        h += hline(func, "Returns")
    h += hline(func, "History")
    h += box_top(func, width)
    return h

def build_headers(infile):
    headers = []
    pattern = re.compile(r"""
            ^(?P<indent>\s*)
            (?P<type>((const\ )?[\w&*]+\ )?)
            ((?P<class>\w+)::)?
            (?P<name>(\w+|operator\S\S?))
            \(
            (?P<args>[^)]*)
            \)(?!;)
            """, re.VERBOSE|re.MULTILINE|re.DOTALL)
    for match in pattern.finditer(infile.read()):
        func = match.groupdict()
        # Type can only be ommited for constructors/destructors
        if not func['type'] and (func['class'] == None or
                func['class'] != func['name']):
            continue
        headers.append({'position': match.start(),
            'text': build_header(match.groupdict(), args.name, args.width)})
    return headers

def insert_headers(infile, headers):
    charsRead = 0
    output = ""
    for header in headers:
        amount = header['position'] - charsRead
        charsRead += amount
        output += infile.read(amount)
        output += header['text']
    output += infile.read()
    return output

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
            description="Add function headers to c++ source files. Writes"
            + " to stdout unless the -i option is included.")
    parser.add_argument("files", nargs="+", help="the .cpp files")
    parser.add_argument("-n", "--name", default='Jacob O\'Bryant',
            help="Your name")
    parser.add_argument("-w", "--width", type=int, default=74,
            help="The maximum line width")
    parser.add_argument("-i", "--in-place", action="store_true",
            help="modify file in place")
    args = parser.parse_args()

    for filename in args.files:
        try:
            infile = open(filename, "r")
        except IOError:
            sys.stderr.write("Couldn't open " + filename + " for reading.\n")
            continue
        headers = build_headers(infile)
        infile.seek(0)
        output = insert_headers(infile, headers)
        infile.close()
        try:
            outfile = open(filename, 'w') if args.in_place else sys.stdout
        except IOError:
            sys.stderr.write("Couldn't open " + filename + " for writing.\n")
            continue
        outfile.write(output)
        if outfile is not sys.stdout:
            outfile.close()
