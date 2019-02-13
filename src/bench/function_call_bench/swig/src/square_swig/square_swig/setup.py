from distutils.core import setup, Extension


square_swig_module = Extension('_square_swig',
                           sources=['square_swig.cxx', 'square_swig.h'],
                           )

setup (name = 'square_swig',
       version = '0.1',
       author      = "SWIG Docs",
       description = """Simple swig example from docs""",
       ext_modules = [square_swig_module],
       py_modules = ["square_swig"],
       )