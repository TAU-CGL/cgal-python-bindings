"""
This file instructs scitkit-build how to build the module. This is very close
to the classical setuptools, but wraps some things for us to build the native
modules easier and more reliable.
"""

from setuptools import find_packages
from skbuild import setup

# automatically running conan. Ugly workaround, but does its job.
import subprocess

# because we are using subprocesses, conan has to be in the path. This is mostly
# the case but does not have to be. A workaround is directly accessing the python module.
subprocess.run(
    [
        "python3",
        "-m",
        "conans.conan",
        "install",
        ".",
        "-if",
        "cmake",
        "--build=missing",
    ],
    check=True,
)


setup(
    name="cgalpy",
    version="0.0.1",
    packages=["cgalpy"],
    package_dir={"": "src/libs"},  # The root for out python package is in `./python`.
    cmake_install_dir = "src/libs/cgalpy",
    package_data = {"": ["*.so"]},
    python_requires=">=3.7",
    cmake_minimum_required_version="3.17",
    #cmake_args=["-DCGALPY_KERNEL_BINDINGS=epec","-DCGALPY_ARRANGEMENT_ON_SURFACE_2_BINDINGS=ON","-DCGALPY_AOS2_GEOMETRY_TRAITS_NAME=circleSegment", "-DCGALPY_AOS2_VERTEX_EXTENDED=ON"],
    #cmake_install_target="CGALPY",
    classifiers=[
        "Development Status :: 4 - Beta",
        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python :: 3",
    ],
    install_requires=[
        # requirements necessary for basic usage (subset of requirements.txt)
        "chardet>=4.0.0",
        "networkx>=2.5.1",
        "requests>=2.25.1",
    ],
)
