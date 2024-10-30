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

