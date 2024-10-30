import ast
import astor

class My_visitor(ast.NodeTransformer):

    def __init__(self, type_name_to_replace: str, replacement: str):
        self.type_name_to_replace = type_name_to_replace
        self.replacement = replacement


    # visit __init__
    def visit_ClassDef(self, node):
        # visit the __init__ function
        for child in node.body:
            if isinstance(child, ast.FunctionDef) and child.name == '__init__':
                child.args.args = [self.visit_arg(arg) for arg in child.args.args]
                child.args.posonlyargs = [self.visit_arg(arg) for arg in child.args.posonlyargs]
                # self.visit(child)
        return node

    # visit function

    def visit_FunctionDef(self, node):
        # Visit all function arguments
        node.args.args = [self.visit_arg(arg) for arg in node.args.args]
        return node

    def visit_arg(self, node):
        # Process the annotation of each argument
        if node.annotation:
            node.annotation = self.visit(node.annotation)
        return node

    def visit_Name(self, node):
        # Replace 'float' with 'float | FT'
        if node.id == self.type_name_to_replace:
            node.id = self.replacement
        return node

    def visit_Subscript(self, node):
        # Recursively visit the value and slice of the subscript
        node.value = self.visit(node.value)
        node.slice = self.visit(node.slice)
        return node

    def visit_Tuple(self, node):
        # Recursively visit elements of the tuple
        node.elts = [self.visit(elt) for elt in node.elts]
        return node

    def visit_Dict(self, node):
        # Recursively visit keys and values of the dictionary
        node.keys = [self.visit(key) for key in node.keys]
        node.values = [self.visit(value) for value in node.values]
        return node

    def visit_Index(self, node):
        # Visit index node in subscripts (e.g., float in tuple[float, float])
        node.value = self.visit(node.value)
        return node

    def visit_Slice(self, node):
        # Visit slice nodes (e.g., for slicing in annotations)
        if node.lower:
            node.lower = self.visit(node.lower)
        if node.upper:
            node.upper = self.visit(node.upper)
        if node.step:
            node.step = self.visit(node.step)
        return node

    def visit_ExtSlice(self, node):
        # Visit extended slices (multi-dimensional arrays)
        node.dims = [self.visit(dim) for dim in node.dims]
        return node

    def visit_AnnAssign(self, node):
        # Handle annotated assignments (e.g., variable: type = value)
        if node.annotation:
            node.annotation = self.visit(node.annotation)
        return node

file_name = 'test_pmp.py'
with open(file_name, 'r') as f:
    file_content = f.read()

tree = ast.parse(file_content)
tree = My_visitor("float", "float | FT").visit(tree)

with open("replaced.pyi", 'w') as f:
    f.write(astor.to_source(tree))
