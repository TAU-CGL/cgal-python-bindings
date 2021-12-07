import json
f = open('./definitions.json')
definitions = json.load(f)
f.close()

module_concepts = definitions["concepts"]
module_classes = definitions["classes"]

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
            c1[method] = json.loads(json.dumps(method_overloads))

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

if __name__ == "__main__":
    for class_name, class_def in module_classes.items():
        resolve_class(class_def)

    print(json.dumps(definitions, indent=4, sort_keys=True))

