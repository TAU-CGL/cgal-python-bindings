from distutils.core import setup
package_data = {'': [@TARGET_DATA@] }
setup(name='@TARGET_NAME@',
      version='@SPHINX_TARGET_VERSION_MAJOR@.@SPHINX_TARGET_VERSION_MINOR@',
      description='CGAL Python Bindings',
      py_modules=['@TARGET_MODULES@'],
      packages=[''],
      platforms=['any'],
      author='@TARGET_AUTHOR@',
      author_email='@TARGET_EMAIL@',
      url='@TARGET_URL@',
      package_data=package_data,
      license='GPLv3+')
