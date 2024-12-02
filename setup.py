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
import shutil
import sys
import os
import json

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Configure CGALPY here
PACKAGE_NAME = "cgalpy"  # The name on PyPI you will use  for `pip install`
IMPORT_NAME = "CGALPY"  # This is set in config.cmake unless it's not found
# find import name in config.cmake this scripts path and look for set(CGALPY_IMPORT_NAME "CGALPY_epic" CACHE STRING "" FORCE)
this_path = os.path.dirname(os.path.realpath(__file__))
with open(os.path.join(this_path, "config.cmake")) as f:
    for line in f:
        if "set(CGALPY_IMPORT_NAME " in line:
            IMPORT_NAME = line.split('"')[1]
            break
        if "set(CGALPY_PACKAGE_NAME " in line:
            PACKAGE_NAME = line.split('"')[1]
            break

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

def readme():
    # Simply return the README.md as string
    with open("README.md") as file:
        return file.read()

prepare()
setup(  # https://scikit-build.readthedocs.io/en/latest/usage.html#setup-options
    # ~~~~~~~~~ BASIC INFORMATION ~~~~~~~~~~~
    name=PACKAGE_NAME,
    version="1.0.4",  # TODO: Use better approach for managing version number.
    description="CGAL Bindings",
    long_description=readme(),
    url="https://bitbucket.org/taucgl/cgal-python-bindings/src/master/",
    long_description_content_type="text/markdown",
    author="Nir Goren and Efi Fogel",
    author_email="radekaadek@gmail.com",
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
    python_requires=">=3.8",  # lowest python version supported.
    install_requires=[
        # requirements necessary for basic usage (subset of requirements.txt)
        # "chardet>=4.0.0",
        # "networkx>=2.5.1",
        # "requests>=2.25.1",
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
      cmake_args=[f'-DCMAKE_PREFIX_PATH={os.path.join(os.getcwd(), "cmake")}']
   #
    # There are further options, but you should be fine with these above.
)
