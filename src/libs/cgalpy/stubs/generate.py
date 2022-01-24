#!/usr/bin/python3

import json
import os
import re
import argparse
from pathlib import Path

copyright_text = '''// Copyright (c) 2022 Israel.
// All rights reserved to TAU.
'''
indent = 0
delta = 2
prev_eol = True

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
  global prev_eol
  if (dec): decrease_indent()
  if prev_eol:
    print(indent * ' ' + line, file=out, end='\n' if eol else '')
  else:
    print(line, file=out, end='\n' if eol else '')
  if (inc): increase_indent()
  prev_eol = eol

#! Print an empty line.
def print_empty_line(out, inc=False, dec=False):
  if (dec): decrease_indent()
  print_line(out, '')
  if (inc): increase_indent()

def replace_variables(s: str, variables_map: dict):
  splits = s.split('@')
  variables = [splits[i] for i in range(len(splits)) if i%2 == 1]
  for variable in variables:
    s = s.replace('@' + variable + '@', variables_map[variable])
  return s

def add_missing_fields_to_function(f: dict):
  if "required" not in f:
    f["required"] = True
  if "variables" not in f:
    f["variables"] = {}
  if "return" not in f:
    f["return"] = 'None'

def add_missing_fields(d: dict):
  if "required" not in d:
    d["required"] = True
  if "imports" not in d:
    d["imports"] = []
  if "typedefs" not in d:
    d["typedefs"] = {}
  if "models" not in d:
    d["models"] = {}
  if "concepts" not in d:
    d["concepts"] = {}
  if "classes" not in d:
    d["classes"] = {}
  if "methods" not in d:
    d["methods"] = {}
  for _, concept in d["concepts"].items():
    add_missing_fields(concept)
  for _, _class in d["classes"].items():
    add_missing_fields(_class)
  for _, method in d["methods"].items():
    for overload in method:
      add_missing_fields_to_function(overload)

def filter_required(d: dict):
  d["classes"] = {_class: d["classes"][_class] for _class in d["classes"] if d["classes"][_class]["required"]}
  for class_name, class_def in d["classes"].items():
    filter_required(class_def)
    d["concepts"] = {concept: d["concepts"][concept] for concept in d["concepts"] if d["concepts"][concept]["required"]}
  for concept, concept_def in d["concepts"].items():
    filter_required(concept_def)
  d["models"] = {concept: d["models"][concept] for concept in d["models"] if d["models"][concept]}
  for method, overloads in d["methods"].items():
    d["methods"][method] = [overload for overload in overloads if overload["required"]]

def merge_classes(c1, c2):
  for class_name, class_def in c2["classes"].items():
    if class_name in c1["classes"]:
      merge_classes(c1["classes"][class_name], class_def)
    else:
      c1["classes"][class_name] = json.loads(json.dumps(class_def))
  for method, method_overloads in c2["methods"].items():
    if method in c1["methods"]:
      c1["methods"][method].extend(method_overloads)
    else:
      c1["methods"][method] = json.loads(json.dumps(method_overloads))
  c1["typedefs"].update(c2["typedefs"])

def get_all_concepts(concept):
  concepts = concept["models"]
  res = json.loads(json.dumps(concepts))
  if concepts:
    for concept in concepts:
      res.update(get_all_concepts(module_concepts[concept]))
  return res

def resolve_class(class_def):
  class_def["models"] = get_all_concepts(class_def)
  for concept in class_def["models"]:
    merge_classes(class_def, module_concepts[concept])
  class_def["resolved"] = True

def print_class(out, class_name, class_def):
  print_line(out, '#models: ' + str(class_def["concepts"]))
  print_line(out, 'class ' + class_name + '():', inc=True)
  for typedef, value in class_def["typedefs"].items():
    print_line(out, typedef + " = " + value)
  if class_def["typedefs"]:
    print_empty_line(out)
  for inner_class_name, inner_class_def in class_def["classes"].items():
    print_class(out, inner_class_name, inner_class_def)
  for method, overloads in class_def["methods"].items():
    for overload in overloads:
      if (len(overloads)) > 1:
        print_line(out, '@overload')
      print_line(out, 'def ' + method + '(', eol=False)
      variables = ['self']
      for variable, variable_type in overload["variables"].items():
          variables.append(variable + ": " + variable_type)
      print_line(out, ", ".join(variables), eol=False)
      print_line(out, ') -> ' + overload["return"] + ": ...")
  print_empty_line(out, dec=True)

def is_valid_file(parser, arg):
  if not os.path.exists(arg):
    parser.error("The file %s does not exist!" % arg)
  else:
    return open(arg, 'r')  # return an open file handle

def readable_dir(prospective_dir):
  if not os.path.isdir(prospective_dir):
    parser.error("readable_dir:{0} is not a valid path".format(prospective_dir))
  if os.access(prospective_dir, os.R_OK):
    return prospective_dir
  else:
    parser.error("readable_dir:{0} is not a readable dir".format(prospective_dir))

if __name__ == "__main__":
  import os
  import sys

  parser = argparse.ArgumentParser(description='Generate stub file.')
  parser.add_argument('filename', metavar="filename", nargs='?',
                      help='the specification file name')
  parser.add_argument('--input-path', type=readable_dir, nargs='*',
                      dest="input_paths", default='./')
  parser.add_argument('--output-path', type=readable_dir,
                      dest='output_path', default='./')
  parser.add_argument('--output-file', dest='output_file',
                      default='__init__.pyi')
  parser.add_argument('--name', help='the node name')
  parser.add_argument('--imports', dest='imports',
                      help='external imports')
  args = parser.parse_args()

  # Extract node name:
  name = args.name
  filename = args.filename
  output_path = args.output_path
  output_file = args.output_file
  imports = args.imports

  print(imports)

  if not name and not filename:
    parser.error("Both the the class name and the file name are missing!")

  if not name:
    name = Path(filename).stem

  # Extract file name:
  filename = None
  if not filename:
    filename = name + ".json"

  # Extract configuration input full file name:
  fullname = None
  for path in args.input_paths:
    tmp = os.path.join(path, filename)
    if os.path.exists(tmp):
      fullname = tmp
      break

  if not fullname:
    parser.error("The file %s cannot be found!" % filename)
    exit(-1)

  # Output path
  if Path(output_file).stem == name:
    pyi_path = output_path
  else:
    pyi_path = os.path.join(output_path, name)
  if not os.path.isdir(pyi_path):
    os.mkdir(pyi_path)
  if not os.access(pyi_path, os.W_OK):
    parser.error("{0} is not a readable dir".format(pyi_path))

  pyi_filename = os.path.join(pyi_path, output_file)

  with open(fullname, 'r') as f:
    # filename = sys.argv[1]
    # script_dir = os.path.dirname(__file__)
    # file_path = os.path.join(script_dir, filename)
    # f = open(file_path)
    definitions_string = f.read()
    # f.close()

    with open(pyi_filename, 'w') as out:
      definitions_string = replace_variables(definitions_string, {})
      definitions = json.loads(definitions_string)

      add_missing_fields(definitions)
      filter_required(definitions)

      module_concepts = definitions["concepts"]
      module_classes = definitions["classes"]
      module_functions = definitions["methods"]
      module_typedefs = definitions["typedefs"]
      imports = definitions["imports"]

      classes = module_classes.items()

      for class_name, class_def in classes:
        resolve_class(class_def)

      # print(json.dumps(definitions, indent=4, sort_keys=True))

      print_line(out, "from typing import Iterator, overload")
      for module in imports:
        print_line(out, 'import ' + module)

      print_empty_line(out)

      for typedef, value in module_typedefs.items():
        print_line(out, typedef + " = " + value)
      if module_typedefs:
        print_empty_line(out)

      for class_name, class_def in classes:
        print_class(out, class_name, class_def)

      for function, overloads in module_functions.items():
        for overload in overloads:
          if (len(overloads)) > 1:
            print_line(out, '@overload')
          print_line(out, 'def ' + function + '(', eol=False)
          variables = []
          for variable, variable_type in overload["variables"].items():
            variables.append(variable + ": " + variable_type)
          print_line(out, ", ".join(variables), eol=False)
          print_line(out, ') -> ' + overload["return"] + ": ...")
