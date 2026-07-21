# normal_estimation.py
#----------------------------------------------------------
# Normal estimation:
# Reads a point set, compute and orient its normals,
# and save the point set.
# Input file formats are .off, .xyz and .pwn.
# Output file formats are .xyz and .pwn.
#----------------------------------------------------------
# normal_estimation file_in file_out [options]

import time
import os
import sys
import importlib
lib = 'CGALPY'
i = 1
if len(sys.argv) > 1:
  str = sys.argv[1]
  if str.startswith('CGALPY'):
    lib = str
    i = 2

CGALPY = importlib.import_module(lib)
Psp = CGALPY.Psp

def run_pca_estimate_normals(points, nb_neighbors_pca_normals):
    task_timer = time.perf_counter()
    sys.stderr.write(f"Estimates Normals Direction by PCA (k=" +
        f"{nb_neighbors_pca_normals})...\n")

    # Estimates normals direction.
    # Note: pca_estimate_normals() requires an iterator over points
    # as well as property maps to access each point's position and normal.
    points = Psp.pca_estimate_normals(points, nb_neighbors_pca_normals)

    task_time = time.perf_counter() - task_timer
    memory = CGALPY.Memory_sizer().virtual_size()
    sys.stderr.write(f"done: {task_time:.2f} seconds, " +
          f"{memory>>20} Mb allocated\n")
    return points

# Computes normals direction by Jet Fitting
def run_jet_estimate_normals(points, # input points + output normals
                             nb_neighbors_jet_fitting_normals): # number of neighbors
    task_timer = time.perf_counter()
    sys.stderr.write(f"Estimates Normals Direction by Jet Fitting (k=" +
          f"{nb_neighbors_jet_fitting_normals})...\n")

    # Estimates normals direction.
    # Note: jet_estimate_normals() requires an iterator over points
    # + property maps to access each point's position and normal.
    points = Psp.jet_estimate_normals(points, nb_neighbors_jet_fitting_normals)

    task_time = time.perf_counter() - task_timer
    memory = CGALPY.Memory_sizer().virtual_size()
    sys.stderr.write(f"done: {task_time:.2f} seconds, " +
          f"{memory>>20} Mb allocated\n")
    return points

# Computes normals direction by VCM
def run_vcm_estimate_normals(points, # input points + output normals
                             R, # radius of the offset
                             r): # radius used during the convolution
    task_timer = time.perf_counter()
    sys.stderr.write(f"Estimates Normals Direction using VCM" +
          f" (R={R} and r={r})...\n")

    # Estimates normals direction.
    # Note: vcm_estimate_normals() requires an iterator over points
    # + property maps to access each point's position and normal.
    points = Psp.vcm_estimate_normals(points, R, r)

    task_time = time.perf_counter() - task_timer
    memory = CGALPY.Memory_sizer().virtual_size()
    sys.stderr.write(f"done: {task_time:.2f} seconds, " +
          f"{memory>>20} Mb allocated\n")
    return points

# Hoppe92 normal orientation using a Minimum Spanning Tree.
def run_mst_orient_normals(points, # input points + input/output normals
                           nb_neighbors_mst): # number of neighbors
    task_timer = time.perf_counter()
    sys.stderr.write(f"Orients Normals with a Minimum Spanning Tree (k=" +
          f"{nb_neighbors_mst})...\n")

    # Orients normals.
    # Note: mst_orient_normals() requires an iterator over points
    # as well as property maps to access each point's position and normal.
    points, unoriented_points_index = Psp.mst_orient_normals(points, nb_neighbors_mst)
    points = points[:unoriented_points_index]

    task_time = time.perf_counter() - task_timer
    memory = CGALPY.Memory_sizer().virtual_size()
    sys.stderr.write(f"done: {task_time:.2f} seconds, " +
          f"{memory>>20} Mb allocated\n")
    return points


sys.stderr.write("Normal estimation\n")

#****************************************
# decode parameters
#****************************************

# usage
if len(sys.argv)-1 < 2:
    sys.stderr.write("Reads a point set, compute and orient its normals,\n")
    sys.stderr.write("and save the point set.\n")
    sys.stderr.write("If the input mesh has normals, print the normals deviation.\n")
    sys.stderr.write("\n")
    sys.stderr.write("Usage: " + sys.argv[0] + " file_in file_out [options]\n")
    sys.stderr.write("Input file formats are .off, .xyz and .pwn.\n")
    sys.stderr.write("Output file formats are .xyz and .pwn.\n")
    sys.stderr.write("Options:\n")
    sys.stderr.write("  -estimate plane|quadric|vcm          Estimates normals direction\n")
    sys.stderr.write("  using a tangent plane or quadric or vcm (default=quadric)\n")
    sys.stderr.write("  -nb_neighbors_pca <int>              Number of neighbors\n")
    sys.stderr.write("  to compute tangent plane (default=18)\n")
    sys.stderr.write("  -nb_neighbors_jet_fitting <int>      Number of neighbors\n")
    sys.stderr.write("  to compute quadric (default=18)\n")
    sys.stderr.write("  -offset_radius_vcm <double>           Offset radius\n")
    sys.stderr.write("  to compute VCM (default=0.1)\n")
    sys.stderr.write("  -convolve_radius_vcm <double>         Convolve radius\n")
    sys.stderr.write("  to compute VCM (default=0)\n")
    sys.stderr.write("  -orient MST                          Orient normals\n")
    sys.stderr.write("  using a Minimum Spanning Tree (default=MST)\n")
    sys.stderr.write("  -nb_neighbors_mst <int>              Number of neighbors\n")
    sys.stderr.write("  to compute the MST (default=18)\n")
    sys.stderr.write(f"Running with {sys.argv[0]}data/ChineseDragon-10kv.off ChineseDragon-10kv.pwn" +
          f" -nb_neighbors_jet_fitting 10 -nb_neighbors_mst 10\n")

# Normals Computing options
nb_neighbors_pca_normals = 18 # K-nearest neighbors = 3 rings (estimate normals by PCA)
nb_neighbors_jet_fitting_normals = 18 # K-nearest neighbors (estimate normals by Jet Fitting)
nb_neighbors_mst = 18 # K-nearest neighbors (orient normals by MST)
offset_radius_vcm = 0.1 # Offset radius (estimate normals by VCM)
convolve_radius_vcm = 0 # Convolve radius (estimate normals by VCM)
estimate = "quadric" # estimate normals by jet fitting
orient = "MST" # orient normals using a Minimum Spanning Tree

# decode parameters
input_filename  = CGALPY.data_file_path("meshes/ChineseDragon-10kv.off") if len(sys.argv)==1 else sys.argv[i]
i += 1
output_filename = "ChineseDragon-10kv.pwn" if len(sys.argv)==1 else sys.argv[i]
i += 1
if len(sys.argv)==2:
    nb_neighbors_jet_fitting_normals = 10
    nb_neighbors_mst = 10

while i < len(sys.argv):
    option = sys.argv[i]

    if option == "-estimate":
        i += 1
        estimate = sys.argv[i]
        if estimate != "plane" and estimate != "quadric" and estimate != "vcm":
            sys.stderr.write("invalid value for -estimate: " + estimate + "\n")

    elif option == "-nb_neighbors_pca":
        i += 1
        nb_neighbors_pca_normals = int(sys.argv[i])

    elif option == "-nb_neighbors_jet_fitting":
        i += 1
        nb_neighbors_jet_fitting_normals = int(sys.argv[i])

    elif option == "-offset_radius_vcm":
        i += 1
        offset_radius_vcm = float(sys.argv[i])

    elif option == "-convolve_radius_vcm":
        i += 1
        convolve_radius_vcm = float(sys.argv[i])

    elif option == "-orient":
        i += 1
        orient = sys.argv[i]
        if orient != "MST":
            sys.stderr.write("invalid value for -orient: " + orient + "\n")

    elif option == "-nb_neighbors_mst":
        i += 1
        nb_neighbors_mst = int(sys.argv[i])

    else:
        sys.stderr.write("invalid option " + option + "\n")

    i += 1

accumulated_fatal_err = 0

task_timer = time.perf_counter()

#****************************************
# Loads point set
#****************************************

# Reads a point set file in points[].
points = []
sys.stderr.write("Open " + input_filename + " for reading...")

points = Psp.read_points_with_normals(input_filename)

if len(points) == 0:
    sys.stderr.write("Error: cannot read file " + input_filename + "\n")
    sys.exit(1)

# Prints status
nb_points = len(points)
task_time = time.perf_counter() - task_timer
sys.stderr.write(f"Reads file {input_filename}: {nb_points} points, " +
                 f"{task_time:.2f} seconds\n")
task_timer = time.perf_counter()

#****************************************
# Check requirements
#****************************************

if nb_points == 0:
    sys.stderr.write("Error: empty file\n")
    sys.exit(1)

#****************************************
# Computes normals
#****************************************

# Estimates normals direction.
if estimate == "plane":
    points = run_pca_estimate_normals(points, nb_neighbors_pca_normals)
elif estimate == "quadric":
    points = run_jet_estimate_normals(points, nb_neighbors_jet_fitting_normals)
elif estimate == "vcm":
    points = run_vcm_estimate_normals(points, offset_radius_vcm, convolve_radius_vcm)

# Orient normals.
if orient == "MST":
    points = run_mst_orient_normals(points, nb_neighbors_mst)

#****************************************
# Saves the point set
#****************************************

sys.stderr.write("Write file " + output_filename + "\n")

if not Psp.write_points_with_normals(output_filename, points):
    sys.stderr.write("Error: cannot write file " + output_filename + "\n")
    sys.exit(1)

# Returns accumulated fatal error
sys.stderr.write(f"Tool returned {accumulated_fatal_err}\n")
sys.exit(accumulated_fatal_err)
