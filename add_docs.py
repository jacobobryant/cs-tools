#!/usr/bin/python
import re
import argparse
import sys

KEY_LENGTH = len("Description:")  # That's the longest key.

class Header:
    def __init__(self, func, name, width, position):
        self.func = func
        self.name = name
        self.width = width
        self.position = position
        self.calls = []

        TAB_WIDTH = 4
        numTabs = func['indent'].count('\t')
        width -= numTabs * (TAB_WIDTH - 1)
        self._box_top = "{0:-<{1}}\n".format(func['indent'] + '//', width)

    def add_calls(self, text, names):
        pass

    def text(self):
        h = ""
        h += self._box_top()
        h += self._signature()
        h += self._hline("Description")
        h += self._hline("Programmers", name)
        h += self._hline("Version", "1.0")
        h += self._hline("Input")
        h += self._hline("Output")
        h += self._hline("Calls")
        h += self._hline("Called by")
        h += self._params()
        if self.func['type'] != 'void':
            h += self._hline("Returns")
        h += self._hline("History")
        h += self._box_top()
        return h

    def _hline(self, key, value=''):
        keyString = "{0: <{1}}".format(key + ":", KEY_LENGTH)
        return "{0}// {1} {2}\n".format(self.func['indent'], keyString, value)

    def _box_top(self):
        TAB_WIDTH = 4
        numTabs = self.func['indent'].count('\t')
        width = self.width - numTabs * (TAB_WIDTH - 1)
        return "{0:-<{1}}\n".format(self.func['indent'] + '//', width)

    def _params(self):
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

    def _signature(self):
        args = re.sub(r'\s+', ' ', self.func['args'])
        signature = "{0}({1})".format(self.func['name'], args)
        if self.func['class']:
            return self._hline("Class", self.func['class']) + \
                    self._hline("Method", signature)
        else:
            return self._hline("Function", signature)

def build_headers(text):
    headers = []
    pattern = re.compile(r"""
            ^(?P<indent>\s*)
            (?P<type>((const\ )?[\w&*]+\ )?)
            ((?P<class>\w+)::)?
            (?P<name>(\w+|operator\S\S?))
            \(
            (?P<args>[^)]*)
            \)(?!;)
            """, re.VERBOSE|re.MULTILINE)
    for match in pattern.finditer(text):
        func = match.groupdict()
        # Type can only be ommited for constructors/destructors
        if not func['type'] and (not func['class'] or
                func['class'] != func['name']):
            continue
        headers.append(Header(func, args.name, args.width, match.start()))
#        headers.append({'position': match.start(), 'text':
#            build_header(match.groupdict(), args.name, args.width)})
    return headers

def insert_headers(text, headers):
    output = ""
    position = 0
    for header in headers:
        output += text[position:header.position]
        output += header.text()
        position = header.position
    output += text[position:]
    return output

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
        sys.stderr.write("Couldn't open " + filename +
                " for reading.\n")
        continue
    text = infile.read()
    infile.close()
    headers = build_headers(text)
    names = set([h.func['name'] for h in headers])
    '''
    for h in headers:
        h.add_calls(text, names)
    '''
    output = insert_headers(text, headers)
    try:
        outfile = open(filename, 'w') if args.in_place else sys.stdout
    except IOError:
        sys.stderr.write("Couldn't open " + filename +
                " for writing.\n")
        continue
    outfile.write(output)
    if outfile is not sys.stdout:
        outfile.close()
