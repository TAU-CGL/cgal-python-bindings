import ast
import astor

class My_visitor(ast.NodeTransformer):
    def visit_FunctionDef(self, node):
        for arg in node.args.args:
            if isinstance(arg.annotation, ast.Name) and arg.annotation.id == 'float':
                arg.annotation.id = 'float | FT'

        return node

file_name = 'test_pmp.py'
with open(file_name, 'r') as f:
    file_content = f.read()

tree = ast.parse(file_content)
tree = My_visitor().visit(tree)

with open("replaced.pyi", 'w') as f:
    f.write(astor.to_source(tree))


