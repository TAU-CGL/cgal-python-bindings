#!/usr/bin/env python3

from distutils.core import setup
import os
import sys
import argparse

# Determine whether the given directory exists and readable
def readable_dir(prospective_dir):
  if not os.path.isdir(prospective_dir):
    parser.error("The directory{} does not exist!".format(prospective_dir))
  if os.access(prospective_dir, os.R_OK):
    return prospective_dir
  else:
    parser.error("The directory {} is not a readable dir!".format(prospective_dir))

indent = 0
delta = 2

#! Increase the indentation level.
def increase_indent():
  global indent
  indent += delta

#! Decrease the indentation level.
def decrease_indent():
  global indent
  indent -= delta

#! Print a single indented line.
def print_line(out, line, eol=True, inc=False, dec=False):
  if (dec): decrease_indent()
  print ('{}{}'.format(indent * ' ', line), file=out, end='\n' if eol else '')
  if (inc): increase_indent()

# Main function
if __name__ == '__main__':
  parser = argparse.ArgumentParser(description='Construct nearest-neighbors search structure.')
  parser.add_argument('--input-path', type=readable_dir,
                      dest="input_path", default='./')
  parser.add_argument('--output-file', default='setup.py', dest="output_file")
  parser.add_argument('--output-path', type=readable_dir,
                      dest="output_path", default='./')
  parser.add_argument('--name', default='CGALPY', dest="name")
  parser.add_argument('--suffix', default='so', dest="suffix")
  parser.add_argument('--module', nargs='*', dest="modules")
  args = parser.parse_args()
  input_path = args.input_path
  output_path = args.output_path
  output_file = args.output_file
  name = args.name
  suffix = args.suffix
  fullname = os.path.join(output_path, output_file)

  package_list = [
    name + "." + suffix,
    name + ".pyi"]
  for module in args.modules:
    package_list.append(module + "/__init__.pyi")
  # print(package_list)
  package_data = {'': package_list}
  # print(package_data)

  with open(fullname, 'w') as out:
    print_line(out, 'from distutils.core import setup')
    print_line(out, "package_data = ", eol=False)
    print_line(out, package_data)

    print_line(out, "setup(name="   + "\'" + name    + "\'"         + ",")
    indent=6
    print_line(out, "version="      + "\'" + "1.0" + "\'"           + ",")
    print_line(out, "py_modules="   + "[\'" + name + "\']"          + ",")
    print_line(out, "packages="     + "[\'\']"                      + ",")
    print_line(out, "author="       + "\'taucgl\'"                  + ",")
    print_line(out, "author_email=" + "\'cgl@tau.ac.il\'"           + ",")
    print_line(out, "url="          + "\'http://acg.cs.tau.ac.il\'" + ",")
    print_line(out, "package_data=" + "package_data"                + ")")
