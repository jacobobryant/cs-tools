#!/usr/bin/python
import re
import argparse
import sys

class BaseHeader:
    indent = ""
    position = 0
    contents = []
    key_length = 0

    def __str__(self):
        self.key_length = max([len(k) for k, v in self.contents]) \
                + len(": ")
        ret = self.box_top()
        for k, v in self.contents:
            if isinstance(v, str):
                ret += self.hline(k, v)
            else:
                ret += self.hlines(k, v)
        ret += self.box_top()
        return ret

    def hline(self, key, value=''):
        if key:
            key += ":"
        keyString = "{0: <{1}}".format(key, self.key_length)
        return "{0}// {1}{2}\n".format(self.indent, keyString, value)

    def hlines(self, key, values):
        ret = ''
        if values:
            ret += self.hline(key, values[0])
            ret += ''.join([self.hline('', value) for value in values[1:]])
        return ret

    def box_top(self):
        numTabs = self.indent.count('\t')
        width = args.width - numTabs * (args.tab_width - 1)
        return "{0:-<{1}}\n".format(self.indent + '//', width)

    @staticmethod
    def equalize(keys):
        width = max([len(key) for key in keys]) + len(": ")
        keyString = lambda key: "{0: <{1}}".format(key + ":", width)
        return [keyString(key) for key in keys]

    @staticmethod
    def merge(text, headers):
        output = ""
        position = 0
        for header in sorted(headers, key=lambda h: h.position):
            output += text[position:header.position]
            output += str(header)
            position = header.position
        output += text[position:]
        return output

class FunctionHeader(BaseHeader):
    def __init__(self, func, position):
        self.indent = func['indent']
        self.type = func['type']
        self.class_ = func['class']
        self.name = func['name']
        self.args = re.sub(r'\s+', ' ', func['args'])
        self.position = position
        self.calls = []
        self.called_by = []
        self.signature = "{0}({1})".format(self.name, self.args)
        self.definition = self.type + ' ' if self.type else ''
        self.definition += self.signature

    @classmethod
    def parse(cls, text):
        headers = []
        pattern = re.compile(r"""
                ^(?P<indent>\s*)
                ((?P<type>(const\ )?[\w&*]+)\ )?
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
            headers.append(cls(func, match.start()))
        return headers

    def set_calls(self, text, names):
        block = get_block(text, self.position)
        pattern = re.compile(r'[^:](\w+)\(')
        matches = set(pattern.findall(block))
        self.calls = [name for name in matches if name in names]

    def set_called_by(self, headers):
        for called in self.calls:
            for header in headers:
                if header.name == called:
                    header.called_by.append(self.name)

    def __str__(self):
        self.contents = self.title() + \
                [["Description", ""],
                 ["Programmers", args.name],
                 ["Version", args.version],
                 ["Environment", args.environment],
                 ["Input", ""],
                 ["Output", ""]]
        if self.calls:
            self.contents.append(["Calls", ", ".join(self.calls)])
        if self.called_by:
            self.contents.append(["Called by", ", ".join(self.called_by)])
        if self.args:
            self.contents.append(["Parameters", self.params()])
        if self.type != 'void':
            self.contents.append(["Returns", ""])
        self.contents.append(["History", ""])
        return super().__str__()

    def params(self):
        args = [arg.strip() for arg in self.args.split(',')]
        return self.equalize(args)

    def title(self):
        if self.class_:
            return [["Class", self.class_],
                    ["Method", self.signature]]
        else:
            return [["Function", self.signature]]

class CPPFileHeader(BaseHeader):
    def __init__(self, filename, headers):
        functions = [header.definition for header in headers]
        self.contents = [["File", filename],
                         ["Functions", functions]]

class HFileHeader(BaseHeader):
    def __init__(self, filename, text):
        classes = re.findall(r'^\s*class\s+(\w+)', text, re.MULTILINE)
        self.contents = [["File", filename],
                         ["Classes", classes]]

class ClassHeader(BaseHeader):
    def __init__(self, class_, text, position):
        self.indent = class_['indent']
        self.name = class_['name']
        self.position = position
        block = get_block(text, position)
        props = re.findall(r'^\s*((?:[\w&*]+\s+)+\w+);', block,
                re.MULTILINE)
        self.properties = [' '.join(p.split()) for p in props]

    @classmethod
    def parse(cls, text):
        headers = []
        pattern = re.compile(r'^(?P<indent>\s*)class\s+(?P<name>\w+)',
                re.MULTILINE)
        for match in pattern.finditer(text):
            class_ = match.groupdict()
            headers.append(cls(class_, text, match.start()))
        return headers

    def __str__(self):
        self.contents = [["Class", self.name],
                         ["Description", ""],
                         ["Programmers", args.name],
                         ["Version", args.version],
                         ["Environment", args.environment]]
        if self.properties:
            self.contents.append(["Properties",
                self.equalize(self.properties)])
        self.contents.append(["History", ""])
        return super().__str__()

def get_block(text, start):
    start = text.find("{", start)
    level = 0
    for i in range(start, len(text)):
        if text[i] == "{":
            level += 1
        elif text[i] == "}":
            level -= 1
        if level == 0:
            end = i
            break
    return text[start:end]

def insert_headers(filename, text):
    headers = []
    ext = filename.split(".")[-1]
    if ext == "h":
        headers.append(HFileHeader(filename, text))
        headers += ClassHeader.parse(text)
    else:
        fheaders = FunctionHeader.parse(text)
        names = set([h.name for h in fheaders])
        for h in fheaders:
            h.set_calls(text, names)
            h.set_called_by(fheaders)
        headers.append(CPPFileHeader(filename, fheaders))
        headers += fheaders
    return BaseHeader.merge(text, headers)

parser = argparse.ArgumentParser(description="Add function headers to " \
        + "c++ source files. Writes to stdout unless the -i option is " \
        + "included.")
parser.add_argument("files", nargs="+", help="the input files")
parser.add_argument("-n", "--name", default='Jacob O\'Bryant', help="Your name")
parser.add_argument("-w", "--width", type=int, default=74, help="The maximum line width")
parser.add_argument("-i", "--in-place", action="store_true", help="modify file in place")
parser.add_argument("-e", "--environment", default="Intel Pentium PC, Arch Linux, gcc 4.7", help="The maximum line width")
parser.add_argument("-t", "--tab-width", type=int, default=4, help="The width of tabs")
parser.add_argument("-v", "--version", default="1.0", help="The starting version")
args = parser.parse_args()

for filename in args.files:
    try:
        infile = open(filename, "r")
    except IOError:
        sys.stderr.write("Couldn't open " + filename +
                " for reading.\n")
        continue
    output = insert_headers(filename, infile.read())
    infile.close()
    try:
        outfile = open(filename, 'w') if args.in_place else sys.stdout
    except IOError:
        sys.stderr.write("Couldn't open " + filename +
                " for writing.\n")
        continue
    outfile.write(output)
    if outfile is not sys.stdout:
        outfile.close()
