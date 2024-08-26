"""
This file instructs scitkit-build how to build the module. This is very close
to the classical setuptools, but wraps some things for us to build the native
modules easier and more reliable.
For a proper installation with `pip install .`, you additionally need a
`pyproject.toml` to specify the dependencies to load this `setup.py`.
You can use `python3 setup.py install` to build and install the package
locally, with verbose output. To build this package in place, which may be
useful for debugging, use `python3 setup.py develop`. This will build
the native modules and move them into your source folder.
The setup options are documented here:
https://scikit-build.readthedocs.io/en/latest/usage.html#setup-options
"""

from skbuild import setup
import sys
import os

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Configure CGALPY here
PACKAGE_NAME = "cgalpy"  # The name on PyPI you will use  for `pip install`
IMPORT_NAME = "CGALPY"  # The name for `import ...`
CGALPY_CONFIGURATION = [
                        # "-DCGALPY_ARRANGEMENT_ON_SURFACE_2_BINDINGS=ON",
                        # "-DCGALPY_AOS2_POINT_LOCATION_BINDINGS=ON",
                        # "-DCGALPY_AOS2_VERTEX_EXTENDED=ON",
                        # "-DCGALPY_AOS2_HALFEDGE_EXTENDED=ON",
                        # "-DCGALPY_AOS2_FACE_EXTENDED=ON",
                        # "-DCGALPY_AOS2_GEOMETRY_TRAITS_NAME=circleSegment",
                        "-DCGALPY_KERNEL_BINDINGS=epec",
                        # "-DCGALPY_KERNEL_INTERSECTION_BINDINGS=ON",
                        # "-DCGALPY_BOOLEAN_SET_OPERATIONS_2_BINDINGS=ON",
                        # "-DCGALPY_CONVEX_HULL_2_BINDINGS=ON",
                        # "-DCGALPY_KERNEL_D_BINDINGS=ON",
                        # "-DCGALPY_KERNEL_D_NAME=epicd",
                        # "-DCGALPY_POLYGON_2_BINDINGS=ON",
                        # "-DCGALPY_MINKOWSKI_SUM_2_BINDINGS=ON",
                        # "-DCGALPY_POLYGON_MESH_PROCESSING_BINDINGS=ON",
                        # "-DCGALPY_PMP_POLYGONAL_MESH_NAME=surfaceMesh",
                        # "-DCGALPY_TRIANGULATED_SURFACE_MESH_SIMPLIFICATION_BINDINGS=ON"
                        # "-DCGALPY_3D_POINT_SET_BINDINGS=ON",
                        # "-DCGALPY_SPATIAL_SEARCHING_BINDINGS=ON",
                        # "-DCGALPY_SURFACE_MESH_BINDINGS=ON",
                        # "-DCGALPY_TRIANGULATION_2_BINDINGS=ON",
                        # "-DCGALPY_TRI2_VERTEX_WITH_INFO=ON",
                        # "-DCGALPY_TRI2_FACE_WITH_INFO=ON",
                        # "-DCGALPY_TRI2_HIERARCHY=ON",
                        # "-DCGALPY_TRIANGULATION_3_BINDINGS=ON",
                        # "-DCGALPY_TRI3_NAME=delaunay"
                        "-DCGALPY_WITH_VISUAL=OFF"
                       ]
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

def prepare():
    # Automatically create proxy modules for new configurations.
    folder = f"python/{IMPORT_NAME}"
    proxy_init = "# This file has been automatically created by setup.py.\n" \
                 "# It is a proxy to import the compiled CGALPY code.\n" \
                 "from ._CGALPY import *"
    if not os.path.exists(f"{folder}/__init__.py"):
        print(f"Automatically creating proxy package '{folder}'.")
        os.makedirs(f"{folder}", exist_ok=True)
        with open(f"{folder}/__init__.py", "w") as f:
            f.write(proxy_init)


def run_conan():
    import subprocess

    # Make sure to access the local conan
    cmd = "-m conans.conan install . --build missing"
    subprocess.run([sys.executable, *cmd.split(" ")], check=True)


def readme():
    # Simply return the README.md as string
    with open("README.md") as file:
        return file.read()

prepare()
run_conan()  # automatically running conan. Ugly workaround, but does its job.
setup(  # https://scikit-build.readthedocs.io/en/latest/usage.html#setup-options
    # ~~~~~~~~~ BASIC INFORMATION ~~~~~~~~~~~
    name=PACKAGE_NAME,
    version="1.0.0",  # TODO: Use better approach for managing version number.
    description="CGAL Bindings",
    long_description=readme(),
    url="https://bitbucket.org/taucgl/cgal-python-bindings/src/master/",
    long_description_content_type="text/markdown",
    author="Nir Goren and Efi Fogel",
    author_email="...",
    classifiers=[
        "Development Status :: 4 - Beta",
        #     "License :: OSI Approved :: MIT License",
        #     "Programming Language :: Python :: 3",
    ],
    # ~~~~~~~~~~~~ CRITICAL PYTHON SETUP ~~~~~~~~~~~~~~~~~~~
    # This project structures defines the python packages in a subfolder.
    # Thus, we have to collect this subfolder and define it as root.
    packages=[f"{IMPORT_NAME}"],
    package_dir={"": "python"},  # The root for our python package is in `./python`.
    python_requires=">=3.7",  # lowest python version supported.
    install_requires=[
        # requirements necessary for basic usage (subset of requirements.txt)
        "chardet>=4.0.0",
        "networkx>=2.5.1",
        "requests>=2.25.1",
    ],
    # ~~~~~~~~~~~ CRITICAL CMAKE SETUP ~~~~~~~~~~~~~~~~~~~~~
    # Especially LTS systems often have very old CMake version (or none at all).
    # Defining this will automatically install locally a working version.
    cmake_minimum_required_version="3.26",
    #
    # By default, the `install` target is built (automatically provided).
    # To compile a specific target, use the following line.
    # Alternatively, you can use `if(NOT SKBUILD) ... endif()` in CMake, to
    # remove unneeded parts for packaging (like tests).
    # cmake_install_target = "install"
    #
    # In the cmake you defined by install(...) where to move the built target.
    # This is critical als only targets with install will be used by skbuild.
    # This should be relative paths to the project root, as you don't know
    # where the package will be packaged. You can change the root for the
    # install-paths with the following line. Note that you can also access
    # the installation root (including this modification) in cmake via
    # `CMAKE_INSTALL_PREFIX`. If your package misses some binaries, you
    # probably messed something up here or in the `install(...)` path.
    # cmake_install_dir = ".",
    # |-----------------------------------------------------------------------|
    # | If you are packing foreign code/bindings, look out if they do install |
    # | targets in global paths, like /usr/libs/. This could be a problem.    |
    # |-----------------------------------------------------------------------|
    #
    # Some CMake-projects allow you to configure it using parameters. You
    # can specify them for this Python-package using the following line.
    cmake_args=[f"-DCGALPY_IMPORT_NAME={IMPORT_NAME}", f'-DCMAKE_PREFIX_PATH={os.path.join(os.getcwd(), "cmake")}'] + CGALPY_CONFIGURATION
    #
    # There are further options, but you should be fine with these above.
)
