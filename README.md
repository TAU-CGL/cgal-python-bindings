# cgalpy: CGAL Python Bindings

We introduce bindings that enable the convenient, efficient, and
reliable use of software modules of [https://www.cgal.org/](CGAL)
(Computational Geometry Algorithm Library), which are written in C++,
from within code written in Python.

The bindings exploits [nanobind](https://github.com/wjakob/nanobind),
a lightweight header-only binding generation tool.

## Python Bindings

When running Python code that uses bindings for some C++ modules, one
or more libraries that provide the bindings must be accessible.  A
software package in C++ that adheres to the generic programming
paradigm, such as CGAL, consists of function and class templates;
these templates are instantiated at compile time of the binding
libraries. In other words, the types of the C++ objects that are
bound, that is, instances (instantiated types) of C++ function and
class templates, must be known when the bindings are generated.

## Bindings for CGAL

Instantiated types in CGAL are characterized by long instantiation
chains of C++ class templates. An instantiated type is a template that
has one or more template parameters and every parameter is substituted
by another type that is typically an instance of another
template. While the number of models of most concepts is contained,
the number of potential types of objects that could be bound is
enormous. Offering bindings for all these types in advance is
practically impossible. Moreover, in some cases models need to be
extended with types provided by the user. Our objective is to enable
concurrent bindings, of a great portion of this set, in a convenient
way.

Bindings are generated according to your prescriptions, which enables
the convenient use of any subset of bound object types concurrently.
You can generate a single library that contains bindings for instances
of different CGAL templates, or several libraries (which can be
used concurrently), such that distinct libraries contain different
instances of the same template.

## Installing

There are two ways to build & install the bindings.
Novice users who may only wish to quickly experiment with the bindings may
exploit the simple self-compiling build & install mechanism to obtain a single
library that supports bindings for a large set of instances.
This binding library cannot be mutually used with other binding libraries though.

All you need to do is:

    git clone git@bitbucket.org:taucgl/cgal-python-bindings.git
    pip3 install --user <CGALPY_SRC_DIR>

where `<CGALPY_SRC_DIR>` is the root directory of your local clone of
this repository. This will install a Python module called `CGALPY_epec`.
From the name, you can infer that this module provides binding for
C++ instances that are bsed on the exact-predicate exact-construction
kernel.

If you want to explot the full power of the bindings, you need to perform 
a few steps. You need to install at least C++17, Python 3.8+, 
[CMake 3.15+](https://cmake.org/install/),
[nanobind](https://github.com/wjakob/nanobind#dependencies), and 
[CGAL 5.0+](https://doc.cgal.org/latest/Manual/installation.html) before
attempting to use the bindings.

## Building

If you install CGAL from sources, don't forget to set the environment
variable `CGAL_DIR` to point at the CGAL build directory.

As nanobind includes a git submodule, do not forget to issue the
following command after cloning the nanobind repository: ``` git
submodule update --init ``` Also, set the environment variable
`nanobind_DIR` to point at the root of the clone.

Let's say that you would like to generate bindings for the *2D/3D
Kernel* and *2D Arrangements* packages of **CGAL**, and in particular
for the following:

* `Kernel`, and
* `CGAL::Arrangement<GeometryTraits, Dcel>`

where,

* `Kernel` is `CGAL::Exact_predicate_exact_construction_kernel`,
* `GeometryTraits` is `CGAL::Arr_segment_traits_2<Kernel>`, and
* `Dcel` is the default DCEL.

Assuming you have all dependencies installed, type:

    cmake -C <CGALPY_SRC_DIR>/cmake/tests/release/aos2_epec_fixed_release.cmake <CGALPY_SRC_DIR>
    make -j4
    pip3 install --user src/libs/cgalpy/dist/CGALPY-1.0-py3-none-any.whl

Then, you should be able to execute the program
`<CGALPY_SRC_DIR>/src/python_scripts/cgalpy_examples/aos2.py`.

At some point you will need bindings for additional instances (I
assume); see Section [**Details**](markdown-header-details) for the
relevant instructions.

If you are a developer and would like to build the *cpp*
documentation, type:

    make CGALPY_CPP_DOC

If you would like to build the Python documentation, type:

    make CGALPY_DOC

The Python html manual pages are generated under
`src/libs/cgalpy/CGALPY/build/html/`. The pdf single file is generated
under `src/libs/cgalpy/CGALPY/build/latex/`.

Observe that the `CGALPY` prefix in the target of the above `make` is the binding library based name; this name can be different then `CGALPY`, and depends on your selections; see Section [**CGAL** Bindings](#markdown-header-cgal-bindings).

If you would like to build the documentation for both, type:

    make doc

## Details
