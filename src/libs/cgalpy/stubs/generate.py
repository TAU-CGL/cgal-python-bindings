import json

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

def get_all_concepts(concept):
    concepts = concept["concepts"]
    res = json.loads(json.dumps(concepts))
    if concepts:
        for concept in concepts:
            res.update(get_all_concepts(module_concepts[concept]))
    return res

def resolve_class(class_def):
    class_def["concepts"] = get_all_concepts(class_def)
    for concept in class_def["concepts"]:
        merge_classes(class_def, module_concepts[concept])

    class_def["resolved"] = True

def print_class(class_name, class_def, indent=0):
    print("    "*indent + 'class ' + class_name + '():')
    # print("    "*indent + '#models: ' + str(class_def["concepts"]))
    for inner_class_name, inner_class_def in class_def["classes"].items():
        print_class(inner_class_name, inner_class_def, indent+1)
    for method, overloads in class_def["methods"].items():
        for overload in overloads:
            if (len(overloads)) > 1:
                print("    "*(indent+1) + '@overload')
            print("    "*(indent+1) + 'def ' + method + '(' ,end='')
            variables = []
            for variable, variable_type in overload["variables"].items():
                variables.append(variable + ": " + variable_type)
            print(", ".join(variables), end='')
            print(') -> ' + overload["return"])


if __name__ == "__main__":
    f = open('./definitions.json')
    definitions = json.load(f)
    f.close()

    module_concepts = definitions["concepts"]
    module_classes = definitions["classes"]

    for class_name, class_def in module_classes.items():
        resolve_class(class_def)

    # print(json.dumps(definitions, indent=4, sort_keys=True))

    classes = module_classes.items()
    for class_name, class_def in module_classes.items():
        print_class(class_name, class_def)

