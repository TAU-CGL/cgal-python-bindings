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

