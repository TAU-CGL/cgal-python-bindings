#!/usr/bin/env python3
import argparse

if __name__ == '__main__':
  parser = argparse.ArgumentParser(description='Configure index file.')
  parser.add_argument('filename', metavar="filename", nargs='?',
                      help='the specification file name')
  parser.add_argument('--output-file', default='setup.py', dest="output_file")
  parser.add_argument('--module', nargs='*', dest="modules")
  args = parser.parse_args()
  multiline_modules = ''
  for module in args.modules:
    multiline_modules += '   ' + module + '\n'
  fin = open(args.filename, 'rt')
  fout = open(args.output_file, 'wt')
  for line in fin:
    fout.write(line.replace('@SPHINX_MULTILINE_MODULES@', multiline_modules))
  fin.close()
  fout.close()
