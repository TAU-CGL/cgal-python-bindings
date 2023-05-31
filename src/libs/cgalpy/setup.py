import distutils
from distutils.core import Extension, setup
package_data = {'@TARGET_PACKAGE_NAME@': [@TARGET_DATA@] }
setup(name='@TARGET_NAME@',
      version='@TARGET_VERSION_MAJOR@.@TARGET_VERSION_MINOR@.@TARGET_VERSION_PATCH@',
      description='CGAL Python Bindings',
      packages=['@TARGET_PACKAGE_NAME@'],
      # platforms=['any'], # linux_x86_64, darwin_arm64
      platforms=[distutils.util.get_platform().replace('-','_').replace('.','_')],
      author='@TARGET_AUTHOR@',
      author_email='@TARGET_EMAIL@',
      url='@TARGET_URL@',
      package_dir={'@TARGET_PACKAGE_NAME@': '.'},
      package_data=package_data,
      license='GPLv3+',
      ext_modules=[Extension('fake', sources=[])])
