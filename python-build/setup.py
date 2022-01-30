from distutils.core import setup

package_data = {'': ['CGALPY.dylib', 'CGALPY.pyi', 'Aos2/__init__.pyi', 'Ker/__init__.pyi', 'Kerd/__init__.pyi']}

setup(name='CGALPY',
      version='1.0',
      py_modules=['CGALPY'],
      packages=[''],
      package_data=package_data
)