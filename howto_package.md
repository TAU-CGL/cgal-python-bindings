# How to package a PyBind11/nanobind-Project

In this document, we learn how to properly package you PyBind11/nanobind-bindings into a self-building Python package that you can share on PyPI, making it available on most systems with a simple `pip install PACKAGE_NAME`. For an example of a reasonably clean and well-structured project, check https://github.com/CG-SHOP/pyutils23 (at least we put a lot of effort into it, feel free to give us feedback).

There are tools to extend this approach to pre-built packages, but they may require a lot CI-minutes (at least for building for Windows and Mac), non-affordable for small projects.

## What tools are we going to use?

In the following, are going to use the following tools. You probably know already some of them.

- **cmake**: You C++ project should be based on cmake and we will have to make some non-invasive changes to it.
- **pybind11/nanobind**: Only mentioned  for completeness. This part should already be done.
- **CPM**: For handling simple C++-dependencies (which you could acquire by just checking out a repository). There are multiple alternatives, but we settled for this one because it has worked fine so far.
- **conan**: For handling complex C++-dependencies, if there are any.
- **scikit-build**: For building a `setup.py` that allows the Python-package to build itself.

As a further step: Continuous Integration to automatically test building the package on various systems and upload new version to PyPI.
Currently, GitHub gives you 200 minutes/month of CI-time for free, which suffices to
build projects even using template-heavy libraries like CGAL multiple times a month.

## What is given?

You have to have a C++-project with

- CMake
- PyBind11 or nanobind-bindings, that are already a target in CMake.

## Step 1: Guard your CMake-changes with if(SKBUILD) … endif()

We will have to modify the CMake-files. If you want to preserve the previous CMake-behavior, you can warp all additive changes with

```cmake
if(SKBUILD)
	...
endif()
```

and all subtractive changes with

```cmake
if(NOT SKBUILD)
	...
endif()
```

The tool we are going to use later will define `SKBUILD` when we are build a Python-package, allowing CMake to only apply the changes when building the package. You can also remove parts that are not needed or may be even problematic when we only want to build the package (e.g., tests, documentations, or unrelated executables).

If you want to test you project in the next steps, you have to add `-DSKBUILD=ON` to the command line (or just define `SKBUILD=ON` somehow), as otherwise your changes will not be applied in a normal CMake-call.

## Step 2: Use a C++-package manager to handle the dependencies

The next step is to manage the C++-dependencies by some package manager, so during the Python-installation, Python will be able to fetch the dependencies on its own and does not require the user to have the dependencies installed system wide. This part can also be useful for pure C++-projects, but if you don’t want any changes, use the guards of the previous step.

### CPM

For simple dependencies, which just require you to check out a repository, I recommend to use CPM. It isn’t perfect but it  is nice and easy. We are using it to include PyBind11 or nanobind.

For including it, we will copy the file [https://github.com/cpm-cmake/CPM.cmake/blob/master/cmake/CPM.cmake](https://github.com/cpm-cmake/CPM.cmake/blob/master/cmake/CPM.cmake) into the subdirectory `cmake`, which probably already exists. We don’t recommend to use the official `get_CPM` -file as we encountered some problems with it when packaging on some systems. They only advantage of the  `get_CPM`  is  that it  gets you the latest version and requires you to include a smaller file.

Now we can include PyBind11 or nanobind (plus all simple requirements) as follows.

```cmake
include(./cmake/CPM.cmake)
CPMAddPackage("gh:wjakob/nanobind@0.0.8")  # nanobind
CPMAddPackage("gh:pybind/pybind11@2.10.0") # pybind11 
```

It has some kind of automatic caching and stuff. The @-tag refers to a release version on GitHub, you may want to change it to the latest release.

If you can specify all you dependencies with CPM, you can now just run cmake normally to test if everything is working as it should. Best test in on a machine that does not have the dependencies installed on system level, so it really requires CPM to resolve them.

### Conan

For more complex dependencies, such as cgal, we recommend using the more powerful package manager [conan]([https://conan.io/](https://conan.io/)). Conan is much more complex and uses a package index that does not have every github project. So only use it, if CPM does not work  or is too slow.

For conan you have to create a `conanfile.txt` defining the dependencies (you can also use the more modern `conanfile.py` approach, but we won’t do it here for simplicity).

```toml
[requires]
cgal/[==5.5]

[generators]
cmake_find_package 
```

Here we specified that we want `cgal` in version 5.5 and use the `cmake_find_package` -generator which generates `findXXX.cmake` -files for us. There are other generators for any kind of scenario. We just use this one as it worked fine so far, and I had more problems with others.

Create the folder `cmake` , go into it, and run `conan install .. --build=missing`.

This should create within `./cmake` all necessary `findXX.cmake` files. The code above will allow cmake to find them automatically. Afterwards, you should be able to use `find_package(XXX)` as you would with a system installed library. Run cmake as you would normally to test if it is working.

To your root `cmakelists.txt` add the following so these file are found. Do not add them to you repository as they are only for your system. When building the Python-package, the command above will be automatically executed to create these files.

```cmake
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
```

TODO: Maybe we should use an additional folder for this, so we can write a simple `.gitignore` ?

## Step 2: install(…) directive for the bindings target

The next step has been the source of most of my problems when packaging a native element, so read carefully.

The target of your bindings needs to define a `install(...)` directive, such that CMake and scikit-build know where to place the binary (you may think this to be obvious for simple projects, but you still have to specify it, and there are non-obvious cases for complex projects). During installation, scikit-build will copy the Python-elements of your project (specified later in `setup.py`) and copy the just built binary to exactly this location, before copying it to your installation folder.

We recommend to have the Python-code within a `python` subfolder, but you don’t have to.
If you don’t have any Python-code but directly build the module,
you should create a folder `python/your_module_name` and add a `__init__.py` within it.
In this `__init__.py` you just import the binary, e.g., `from .your_module_name import *`.
Note the `.` in the beginning.
In case you want to add some Python-code on top, things are now much easier.

Your install-directive should be

```cmake
install(TARGETS mybindings DESTINATION python/your_module_name/)
```

which will copy the binary of target `mybindings` to `python/your_module_name/` (substitute target and module names). Make sure, you don’t add any leading `/` and that the path is relative to the (CMake-)project root.

## Step 3: Add Python-development requirements.txt

We are going to use some Python-tools to build the package.
These will automatically be installed in the next step when executing `pip install XXX`,
but you may want to do some things by hand to get more feedback on what is happening
(especially, if it doesn't work right away). For this reason, we add `requirements.txt` with the essential packages for developing this package.

Just create a file `requirements.txt` with all Python dependencies a developer using your module should have. It should at least contain the following:

```text
conan>=1.53.0
pytest>=7.1.2
scikit-build>=0.15.0
setuptools>=61.2.0
cmake>=3.14
ninja>=1.11.1
```

Install it using `pip install -r requirements.txt` .

## Step 4: Add a pyproject.toml

We don’t want the user to install the requirements by hand and just specifying it in `setup.py` may be too late, because `setup.py` may already be requiring some of the dependencies (such as scikit-build). For this reason, we add a `pyproject.toml` that tells `pip` what to install before trying to build and install this package. You can just copy the following file to the root of your project.

```toml
[build-system]
requires = [
    "conan",
    "setuptools",
    "scikit-build>=0.13",
    "cmake>=3.17",
    "ninja",
]
build-backend = "setuptools.build_meta"
```

It will for example tell Python that we need `cmake`, `conan` , `ninja` and stuff.

## Step 5: Add a setup.py

The `setup.py` is going to be the entry point for actually building the project. The following file is a good template in which you can simply adapt things as needed. Note that this file expects the Python-code to be in `python` and you need to adapt it if this is not the case.

```python
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

from setuptools import find_packages
from skbuild import setup
import sys

def run_conan():
    import subprocess

    # Make sure to access the local conan
    cmd = "-m conans.conan install . -if cmake --build=missing"
    subprocess.run([sys.executable, *cmd.split(" ")], check=True)

def readme():
    # Simply return the README.md as string
    with open("README.md") as file:
        return file.read()

run_conan()  # automatically running conan. Ugly workaround, but does its job.
setup(  # https://scikit-build.readthedocs.io/en/latest/usage.html#setup-options
    # ~~~~~~~~~ BASIC INFORMATION ~~~~~~~~~~~
    name="cgshop2023_pyutils",
    version="0.2.9",  # TODO: Use better approach for managing version number.
    description="Official utilities for the CG:SHOP Challenge 2023.",
    long_description=readme(),
    url="https://github.com/CG-SHOP/pyutils23",
    long_description_content_type="text/markdown",
    author="TU Braunschweig, IBR, Algorithms Group (Phillip Keldenich and Dominik Krupke)",
    author_email="keldenich@ibr.cs.tu-bs.de, krupke@ibr.cs.tu-bs.de",
    classifiers=[
        "Development Status :: 4 - Beta",
        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python :: 3",
    ],
    # ~~~~~~~~~~~~ CRITICAL PYTHON SETUP ~~~~~~~~~~~~~~~~~~~
    # This project structures defines the python packages in a subfolder.
    # Thus, we have to collect this subfolder and define it as root.
    packages=find_packages("python"),  # Include all packages in `./python`.
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
    cmake_minimum_required_version="3.17",
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
    # cmake_args=["-DCGALPY_KERNEL_BINDINGS=epec",
    #             "-DCGALPY_ARRANGEMENT_ON_SURFACE_2_BINDINGS=ON",]
    #
    # There are further options, but you should be fine with these above.
)
```

Note that `setup.py` will be run before CMake, so if you are using CMake to specify version and stuff like this, you have to do it redundantly here.

## Step 6: Add a MANIFEST.in

This file at project root defines, which files should be shipped, i.e., are required for building the package on a foreign machine. You can probably just copy and paste the following (again expecting the Python-code to be in `python`, C++ code in `src` and `include`).

```txt
exclude tests
exclude apps
include conanfile.txt
include CMakeLists.txt
include setup.py
include pyproject.toml
include requirements.txt
include *.md
include cmake/CCache.cmake
include cmake/CPM.cmake
recursive-include python *.cpp
recursive-include python CMakeLists.txt
recursive-include src *.cpp
recursive-include src *.h
recursive-include src *.hpp
recursive-include src CMakeLists.txt
recursive-include include *.h
recursive-include include *.hpp
```

## Step 7: Ship!

Create a project on PyPI with the desired name.
The easiest way to upload it is using GitHub-CI, as for example in [https://github.com/CG-SHOP/pyutils23/blob/main/.github/workflows/publish-to-pypi.yml](https://github.com/CG-SHOP/pyutils23/blob/main/.github/workflows/publish-to-pypi.yml).
Because this requires you to create an access token, you probably have to do the first upload by hand, e.g., using `twine`.
You can also do all uploads by hand, but this can become tedious.
Note that a new upload requires a new version.