import json
import os
import re

INDENTATION = '    '

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
            c1[class_name] = json.loads(json.dumps(class_def))
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


def print_class(class_name, class_def, indent=0):
    print(INDENTATION*indent + '#models: ' + str(class_def["concepts"]))
    print(INDENTATION*indent + 'class ' + class_name + '():')
    for typedef, value in class_def["typedefs"].items():
        print(INDENTATION*(indent+1) + typedef + " = " + value)
    if class_def["typedefs"]:
        print()
    for inner_class_name, inner_class_def in class_def["classes"].items():
        print_class(inner_class_name, inner_class_def, indent+1)
    for method, overloads in class_def["methods"].items():
        for overload in overloads:
            if (len(overloads)) > 1:
                print(INDENTATION*(indent+1) + '@overload')
            print(INDENTATION*(indent+1) + 'def ' + method + '(', end='')
            variables = ['self']
            for variable, variable_type in overload["variables"].items():
                variables.append(variable + ": " + variable_type)
            print(", ".join(variables), end='')
            print(') -> ' + overload["return"] + ": ...")
    print()


if __name__ == "__main__":
    import os
    import sys
    filename = sys.argv[1]
    script_dir = os.path.dirname(__file__)
    file_path = os.path.join(script_dir, filename)
    f = open(file_path)
    definitions_string = f.read()
    f.close()
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

    print("from typing import Iterator, overload")
    for module in imports:
        print('import ' + module)

    print()
    print()
    
    for typedef, value in module_typedefs.items():
        print(typedef + " = " + value)
    if module_typedefs:
        print()

    for class_name, class_def in classes:
        print_class(class_name, class_def)

    for function, overloads in module_functions.items():
        for overload in overloads:
            if (len(overloads)) > 1:
                print('@overload')
            print('def ' + function + '(', end='')
            variables = []
            for variable, variable_type in overload["variables"].items():
                variables.append(variable + ": " + variable_type)
            print(", ".join(variables), end='')
            print(') -> ' + overload["return"] + ": ...")
