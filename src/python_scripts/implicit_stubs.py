import ast
import astor


file_name = '__init__.pyi'

# read file

with open(file_name, 'r') as f:
    file_content = f.read()

class My_visitor(ast.NodeTransformer):
    def visit_FunctionDef(self, node):
        for arg in node.args.args:
            if isinstance(arg.annotation, ast.Name) and arg.annotation.id == 'float':
                arg.annotation.id = 'float | FT'

        return node

tree = ast.parse(file_content)
tree = My_visitor().visit(tree)

with open("replaced.pyi", 'w') as f:
    f.write(astor.to_source(tree))


