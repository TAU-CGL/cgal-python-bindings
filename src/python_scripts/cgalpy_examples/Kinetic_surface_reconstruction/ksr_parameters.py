import time
import math
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
if lib == 'CGALPY':
  sys.path.append(os.path.abspath('../precompiled'))
CGALPY = importlib.import_module(lib)

# #include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
# #include <CGAL/Kinetic_surface_reconstruction_3.h>
# #include <CGAL/Point_set_3.h>
# #include <CGAL/Point_set_3/IO.h>
# #include <CGAL/Real_timer.h>
# #include <CGAL/IO/PLY.h>
# #include <CGAL/IO/polygon_soup_io.h>
# #include <CGAL/pca_estimate_normals.h>
# #include <CGAL/mst_orient_normals.h>
# #include <CGAL/bounding_box.h>
# #include <sstream>
#
# #include "include/Parameters.h"
# #include "include/Terminal_parser.h"
#
# using Kernel    = CGAL::Exact_predicates_inexact_constructions_kernel;
# using FT        = typename Kernel::FT;
# using Point_3   = typename Kernel::Point_3;
# using Vector_3  = typename Kernel::Vector_3;
# using Segment_3 = typename Kernel::Segment_3;
#
# using Point_set    = CGAL::Point_set_3<Point_3>;
# using Point_map    = typename Point_set::Point_map;
# using Normal_map   = typename Point_set::Vector_map;
#
# using KSR = CGAL::Kinetic_surface_reconstruction_3<Kernel, Point_set, Point_map, Normal_map>;
#
# using Parameters      = CGAL::KSR::All_parameters<FT>;
# using Terminal_parser = CGAL::KSR::Terminal_parser<FT>;
# using Timer = CGAL::Real_timer;
#
# template <typename T>
# std::string to_stringp(const T a_value, const int n = 6)
# {
#   std::ostringstream out;
#   out.precision(n);
#   out << std::fixed << a_value;
#   return out.str();
# }
#
# void parse_terminal(Terminal_parser& parser, Parameters& parameters) {
#   // Set all parameters that can be loaded from the terminal.
#   // add_str_parameter  - adds a string-type parameter
#   // add_val_parameter  - adds a scalar-type parameter
#   // add_bool_parameter - adds a boolean parameter
#
#   std::cout << std::endl;
#   std::cout << "--- INPUT PARAMETERS: " << std::endl;
#
#   parser.add_str_parameter("-data", parameters.data);
#
#   // Shape detection.
#   parser.add_val_parameter("-kn"   , parameters.k_neighbors);
#   parser.add_val_parameter("-dist" , parameters.maximum_distance);
#   parser.add_val_parameter("-angle", parameters.maximum_angle);
#   parser.add_val_parameter("-minp" , parameters.min_region_size);
#
#
#   // Shape regularization.
#   parser.add_bool_parameter("-regparallel", parameters.regparallel);
#   parser.add_bool_parameter("-regcoplanar", parameters.regcoplanar);
#   parser.add_bool_parameter("-regorthogonal", parameters.regorthogonal);
#   parser.add_bool_parameter("-regsymmetric", parameters.regsymmetric);
#
#   parser.add_val_parameter("-regoff", parameters.maximum_offset);
#   parser.add_val_parameter("-regangle", parameters.angle_tolerance);
#
#   // Shape regularization.
#   parser.add_bool_parameter("-reorient", parameters.reorient);
#
#   // Partitioning.
#   parser.add_val_parameter("-k", parameters.k_intersections);
#   parser.add_val_parameter("-odepth", parameters.max_octree_depth);
#   parser.add_val_parameter("-osize", parameters.max_octree_node_size);
#
#   // Reconstruction.
#   parser.add_val_parameter("-lambda", parameters.graphcut_lambda);
#   parser.add_val_parameter("-ground", parameters.use_ground);
#
#   // Debug.
#   parser.add_bool_parameter("-debug", parameters.debug);
#
#   // Verbose.
#   parser.add_bool_parameter("-verbose", parameters.verbose);
# }
#
# int main(const int argc, const char** argv) {
#   // Parameters.
#   std::cout.precision(20);
#   std::cout << std::endl;
#   std::cout << "--- PARSING INPUT: " << std::endl;
#   const auto kernel_name = boost::typeindex::type_id<Kernel>().pretty_name();
#   std::cout << "* used kernel: " << kernel_name << std::endl;
#   const std::string path_to_save = "";
#   Terminal_parser parser(argc, argv, path_to_save);
#
#   Parameters parameters;
#   parse_terminal(parser, parameters);
#
#   // If no input data is provided, use input from data directory.
#   if (parameters.data.empty())
#     parameters.data = CGAL::data_file_path("points_3/building.ply");
#
#   // Input.
#   Point_set point_set(parameters.with_normals);
#   CGAL::IO::read_point_set(parameters.data, point_set);
#
#   if (point_set.size() == 0) {
#     std::cout << "input file not found or empty!" << std::endl;
#     return EXIT_FAILURE;
#   }
#
#   if (!point_set.has_normal_map()) {
#     point_set.add_normal_map();
#     CGAL::pca_estimate_normals<CGAL::Parallel_if_available_tag>(point_set, 9);
#     CGAL::mst_orient_normals(point_set, 9);
#   }
#
#   for (std::size_t i = 0; i < point_set.size(); i++) {
#     Vector_3 n = point_set.normal(i);
#     if (abs(n * n) < 0.05)
#       std::cout << "point " << i << " does not have a proper normal" << std::endl;
#   }
#
#   if (parameters.maximum_distance == 0) {
#     CGAL::Bbox_3 bbox = CGAL::bbox_3(CGAL::make_transform_iterator_from_property_map(point_set.begin(), point_set.point_map()),
#       CGAL::make_transform_iterator_from_property_map(point_set.end(), point_set.point_map()));
#
#     FT d = CGAL::approximate_sqrt
#     ((bbox.xmax() - bbox.xmin()) * (bbox.xmax() - bbox.xmin())
#       + (bbox.ymax() - bbox.ymin()) * (bbox.ymax() - bbox.ymin())
#       + (bbox.zmax() - bbox.zmin()) * (bbox.zmax() - bbox.zmin()));
#
#     parameters.maximum_distance = d * 0.03;
#   }
#
#   if (parameters.min_region_size == 0)
#     parameters.min_region_size = static_cast<std::atomic_size_t>(point_set.size() * 0.01);
#
#   std::cout << std::endl;
#   std::cout << "--- INPUT STATS: " << std::endl;
#   std::cout << "* number of points: " << point_set.size() << std::endl;
#
#   std::cout << "verbose " << parameters.verbose << std::endl;
#   std::cout << "maximum_distance " << parameters.maximum_distance << std::endl;
#   std::cout << "maximum_angle " << parameters.maximum_angle << std::endl;
#   std::cout << "min_region_size " << parameters.min_region_size << std::endl;
#   std::cout << "k " << parameters.k_intersections << std::endl;
#   std::cout << "graphcut_lambda " << parameters.graphcut_lambda << std::endl;
#
#   auto param = CGAL::parameters::maximum_distance(parameters.maximum_distance)
#     .maximum_angle(parameters.maximum_angle)
#     .k_neighbors(parameters.k_neighbors)
#     .minimum_region_size(parameters.min_region_size)
#     .debug(parameters.debug)
#     .verbose(parameters.verbose)
#     .max_octree_depth(parameters.max_octree_depth)
#     .max_octree_node_size(parameters.max_octree_node_size)
#     .reorient_bbox(parameters.reorient)
#     .regularize_parallelism(parameters.regparallel)
#     .regularize_coplanarity(parameters.regcoplanar)
#     .regularize_orthogonality(parameters.regorthogonal)
#     .regularize_axis_symmetry(parameters.regsymmetric)
#     .angle_tolerance(parameters.angle_tolerance)
#     .maximum_offset(parameters.maximum_offset);
#
#   // Algorithm.
#   KSR ksr(point_set, param);
#
#   Timer timer;
#   timer.start();
#   std::size_t num_shapes = ksr.detect_planar_shapes(param);
#
#   std::cout << num_shapes << " detected planar shapes" << std::endl;
#
#   FT after_shape_detection = timer.time();
#
#   ksr.initialize_partition(param);
#
#   FT after_init = timer.time();
#
#   ksr.partition(parameters.k_intersections);
#
#   FT after_partition = timer.time();
#
#   std::vector<Point_3> vtx;
#   std::vector<std::vector<std::size_t> > polylist;
#
#   std::map<typename KSR::KSP::Face_support, bool> external_nodes;
#
#   if (parameters.use_ground) {
#     external_nodes[KSR::KSP::Face_support::ZMIN] = false;
#     ksr.reconstruct_with_ground(parameters.graphcut_lambda, std::back_inserter(vtx), std::back_inserter(polylist));
#   }
#   else
#     ksr.reconstruct(parameters.graphcut_lambda, external_nodes, std::back_inserter(vtx), std::back_inserter(polylist));
#
#   FT after_reconstruction = timer.time();
#
#   if (polylist.size() > 0)
#     CGAL::IO::write_polygon_soup("polylist_" + std::to_string(parameters.graphcut_lambda) + (parameters.use_ground ? "_g" : "_") + ".off", vtx, polylist);
#
#   timer.stop();
#   const FT time = static_cast<FT>(timer.time());
#
#   std::vector<FT> lambdas{0.3, 0.5, 0.6, 0.7, 0.73, 0.75, 0.77, 0.8, 0.9, 0.95, 0.99};
#
#   bool non_empty = false;
#
#   for (FT l : lambdas) {
#     if (l == parameters.graphcut_lambda)
#       continue;
#
#     vtx.clear();
#     polylist.clear();
#
#
#     if (parameters.use_ground)
#       ksr.reconstruct_with_ground(l, std::back_inserter(vtx), std::back_inserter(polylist));
#     else
#       ksr.reconstruct(l, external_nodes, std::back_inserter(vtx), std::back_inserter(polylist));
#
#
#     if (polylist.size() > 0) {
#       non_empty = true;
#       CGAL::IO::write_polygon_soup("polylist_" + std::to_string(l) + (parameters.use_ground ? "_g" : "_") + ".off", vtx, polylist);
#     }
#   }
#
#   std::cout << "Shape detection:        " << after_shape_detection << " seconds!" << std::endl;
#   std::cout << "Kinetic partition:      " << (after_partition - after_shape_detection) << " seconds!" << std::endl;
#   std::cout << " initialization:        " << (after_init - after_shape_detection) << " seconds!" << std::endl;
#   std::cout << " partition:             " << (after_partition - after_init) << " seconds!" << std::endl;
#   std::cout << "Kinetic reconstruction: " << (after_reconstruction - after_partition) << " seconds!" << std::endl;
#   std::cout << "Total time:             " << time << " seconds!" << std::endl << std::endl;
#
#   return (non_empty) ? EXIT_SUCCESS : EXIT_FAILURE;
# }
class Terminal_parser:
    def __init__(self, num_parameters, parameters, path_to_save=""):
        self.m_path_to_save = path_to_save
        self.show_help(num_parameters, parameters)
        input_parameters = dict()
        self.set_input_parameters(num_parameters, parameters, input_parameters)
        required = list()
        required.append("-data")
        self.set_parameters(input_parameters, required)
        exceptions = list()
        exceptions.append("-data")
        exceptions.append("-params")
        self.save_parameters_to_file(input_parameters, exceptions)

    def get_input_parameters(self):
        return self.m_parameters
    
    def add_val_parameter(self, parameter_name, variable_value):
        if not self.does_parameter_exist(parameter_name, self.m_parameters):
            return
        parameter_value = self.m_parameters[parameter_name]
        if parameter_value != "default":
            variable_value = float(parameter_value)
        print(parameter_name, ":", variable_value)

    def add_str_parameter(self, parameter_name, variable_value):
        if not self.does_parameter_exist(parameter_name, self.m_parameters):
            return
        parameter_value = self.m_parameters[parameter_name]
        if parameter_value != "default":
            variable_value = parameter_value
        print(parameter_name, ":", variable_value)

    def add_bool_parameter(self, parameter_name, variable_value):
        if not self.does_parameter_exist(parameter_name, self.m_parameters):
            return
        variable_value = True
        print(parameter_name, ":", variable_value)

    def show_help(self, num_parameters, parameters):
        if not self.is_asked_for_help(num_parameters, parameters):
            return
        self.print_help()
        exit(0)

    def is_asked_for_help(self, num_parameters, parameters):
        for i in range(num_parameters):
            if parameters[i] == "-help":
                return True
        return False

    def print_help(self):
        print("\n* HELP:")
        print("\n* EXAMPLE:")
        print("your terminal name $ .")
        print(self.m_path_to_save)
        print("kinetic_reconstruction_example -data path_to_data")
        print(self.m_path_to_save)
        print("data_name.ply -other_param_name -other_param_value")
        print("\n")
        print("REQUIRED PARAMETERS:")
        print("\n")
        print("parameter name: -data")
        print("parameter value: path_to_data")
        print(self.m_path_to_save)
        print("data_name.ply")
        print("description: path to the file with input data")
        print("\n")
        print("OPTIONAL PARAMETERS:")
        print("\n")
        print("parameter name: -silent")
        print("description: suppress any intermediate output except for the final result")
        print("\n")
        print("parameter name: -params")
        print("parameter value: path_to")
        print(self.m_path_to_save)
        print("parameters.ksr")
        print("description: load parameters from the file")
        print("\n")

    def set_input_parameters(self, num_parameters, parameters, input_parameters):
        assert(num_parameters > 0)
        for i in range(1, num_parameters):
            str = parameters[i]
            first_letter = str[0]
            if first_letter == "-":
                if i + 1 < num_parameters:
                    str = parameters[i + 1]
                    first_letter = str[0]
                    if first_letter != "-":
                        input_parameters[parameters[i]] = parameters[i + 1]
                    else:
                        input_parameters[parameters[i]] = "default"
                else:
                    input_parameters[parameters[i]] = "default"

    def set_parameters(self, input_parameters, required):
        if self.parameters_should_be_loaded(input_parameters):
            self.load_parameters_from_file(input_parameters)
        self.m_parameters = input_parameters

    def are_required_parameters_set(self, input_parameters, required):
        are_all_set = True
        for i in range(len(required)):
            if not self.is_required_parameter_set(required[i], input_parameters):
                are_all_set = False
        return are_all_set

    def is_required_parameter_set(self, parameter_name, input_parameters):
        is_set = self.does_parameter_exist(parameter_name, input_parameters) and not self.does_parameter_have_default_value(parameter_name, input_parameters)
        if not is_set:
            print(parameter_name, "PARAMETER IS REQUIRED!")
        return is_set

    def does_parameter_exist(self, parameter_name, input_parameters):
        for parameter in input_parameters:
            if parameter[0] == parameter_name:
                return True
        return False

    def does_parameter_have_default_value(self, parameter_name, input_parameters):
        assert(self.does_parameter_exist(parameter_name, input_parameters))
        return input_parameters[parameter_name] == "default"

    # Loading from a file.
    def parameters_should_be_loaded(self, input_parameters):
        if self.does_parameter_exist("-params", input_parameters):
            return True
        return False

    def load_parameters_from_file(self, input_parameters):
        filePath = input_parameters["-params"]
        if filePath == "default":
            print("ERROR: PATH TO THE FILE WITH PARAMETERS IS NOT DEFINED!")
            exit(1)

        file = open(filePath, "r")
        if not file:
            print("ERROR: ERROR LOADING FILE WITH PARAMETERS!")
            exit(1)

        tmp_parameters = dict()
        for line in file:
            parameter_name, parameter_value = line.split()
            if parameter_name == "" or parameter_value == "":
                continue
            tmp_parameters[parameter_name] = parameter_value

        for pit in tmp_parameters:
            input_parameters[pit] = tmp_parameters[pit]
        file.close()

    def parameter_should_be_saved(self, parameter_name, exceptions):
        for i in range(len(exceptions)):
            if exceptions[i] == parameter_name:
                return False
        return True

    def save_parameters(self, file_path, input_parameters, exceptions):
        file = open(file_path, "w")
        if not file:
            print("ERROR: SAVING FILE WITH THE NAME", file_path)
            exit(1)

        for parameter in input_parameters:
            if self.parameter_should_be_saved(parameter[0], exceptions):
                file.write(parameter[0] + " " + parameter[1] + "\n")
        file.close()

    def save_input_parameters(self, path_to_save, input_parameters, exceptions):
        file_path = path_to_save + "parameters.ksr"
        self.save_parameters(file_path, input_parameters, exceptions)
        print("* parameters are saved in:", file_path)

    # Saving to a file.
    def save_parameters_to_file(self, input_parameters, exceptions):        
        self.save_input_parameters(self.m_path_to_save, input_parameters, exceptions)
        return

class All_parameters:
    def __init__(self):
        self.data = ""
        # boolean tags
        self.with_normals = True
        self.verbose = False
        self.debug = False
        # shape detection / shape regularization
        self.k_neighbors = 12
        self.maximum_distance = 0
        self.maximum_angle = 10
        self.min_region_size = 0
        self.max_octree_depth = 3
        self.max_octree_node_size = 40
        self.regparallel = False
        self.regcoplanar = True
        self.regorthogonal = False
        self.regsymmetric = False
        self.angle_tolerance = 5
        self.maximum_offset = 0
        # partition
        self.k_intersections = 1
        self.enlarge_bbox_ratio = 11 / 10
        self.reorient = False
        self.use_ground = False
        # reconstruction
        self.graphcut_lambda = 1 / 2

Ker = CGALPY.Ker
KSR = CGALPY.Kinetic_surface_reconstruction_3
Parameters = All_parameters

def to_stringp(a_value, n=6):
    return f"{a_value:.{n}f}"

def parse_terminal(parser, parameters):
    # Set all parameters that can be loaded from the terminal.
    # add_str_parameter  - adds a string-type parameter
    # add_val_parameter  - adds a scalar-type parameter
    # add_bool_parameter - adds a boolean parameter

    print("\n--- INPUT PARAMETERS:")

    parser.add_str_parameter("-data", parameters.data)

    # Shape detection.
    parser.add_val_parameter("-kn"   , parameters.k_neighbors)
    parser.add_val_parameter("-dist" , parameters.maximum_distance)
    parser.add_val_parameter("-angle", parameters.maximum_angle)
    parser.add_val_parameter("-minp" , parameters.min_region_size)


    # Shape regularization.
    parser.add_bool_parameter("-regparallel", parameters.regparallel)
    parser.add_bool_parameter("-regcoplanar", parameters.regcoplanar)
    parser.add_bool_parameter("-regorthogonal", parameters.regorthogonal)
    parser.add_bool_parameter("-regsymmetric", parameters.regsymmetric)

    parser.add_val_parameter("-regoff", parameters.maximum_offset)
    parser.add_val_parameter("-regangle", parameters.angle_tolerance)

    # Shape regularization.
    parser.add_bool_parameter("-reorient", parameters.reorient)

    # Partitioning.
    parser.add_val_parameter("-k", parameters.k_intersections)
    parser.add_val_parameter("-odepth", parameters.max_octree_depth)
    parser.add_val_parameter("-osize", parameters.max_octree_node_size)

    # Reconstruction.
    parser.add_val_parameter("-lambda", parameters.graphcut_lambda)
    parser.add_val_parameter("-ground", parameters.use_ground)

    # Debug.
    parser.add_bool_parameter("-debug", parameters.debug)

    # Verbose.
    parser.add_bool_parameter("-verbose", parameters.verbose)


print('\n--- PARSING INPUT:')
kernel_name = type(Ker).__name__
print(f"* used kernel: {kernel_name}")
path_to_save = ""
parser = Terminal_parser(len(sys.argv), sys.argv, path_to_save)

parameters = Parameters()
parse_terminal(parser, parameters)

# If no input data is provided, use input from data directory.
if parameters.data == "":
    parameters.data = CGALPY.data_file_path("points_3/building.ply")

# Input.
point_set = CGALPY.Point_set_3(parameters.with_normals)
CGALPY.read_point_set(parameters.data, point_set)

if point_set.size() == 0:
    print("input file not found or empty!")
    exit(1)

# TODO: work in progress

