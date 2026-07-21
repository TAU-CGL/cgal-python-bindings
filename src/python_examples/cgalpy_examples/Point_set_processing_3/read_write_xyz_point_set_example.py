import sys
import importlib

lib = 'CGALPY'
i = 1
if len(sys.argv) > 1 and sys.argv[1].startswith('CGALPY'):
  lib = sys.argv[1]
  i = 2

CGALPY = importlib.import_module(lib)
Psp = CGALPY.Psp

input_filename = (
  sys.argv[i]
  if len(sys.argv) > i
  else CGALPY.data_file_path("points_3/oni.pwn")
)
i += 1

output_filename = (
  sys.argv[i]
  if len(sys.argv) > i
  else "oni_copy.xyz"
)

points = Psp.read_points_with_normals(input_filename)
if len(points) == 0:
  sys.stderr.write(f"Error: cannot read file {input_filename}\n")
  sys.exit(1)

if not Psp.write_points_with_normals(output_filename, points):
  sys.stderr.write(f"Error: cannot write file {output_filename}\n")
  sys.exit(1)

print(f"Read {len(points)} point(s) with normals")
print(f"Wrote {output_filename}")
