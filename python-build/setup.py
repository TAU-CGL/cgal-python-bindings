from distutils.core import setup
import os
import sys

files = []
for file in os.listdir('.'):
      if '.so' in file or '.pyi' in file:
            files.append(file)
      if os.path.isdir(file):
            for file2 in os.listdir(file):
                  if '.pyi' in file2:
                        files.append(os.path.join(file, file2))

package_data = {'': files}
print(package_data)

setup(name='CGALPY',
      version='1.0',
      py_modules=['CGALPY'],
      packages=[''],
      package_data=package_data
)