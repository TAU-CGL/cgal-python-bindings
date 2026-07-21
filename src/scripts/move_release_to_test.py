# change release from the name of all the files in the directory test
import os
import re
import sys
import shutil

ROOT_DIR = '../..'

def change_release(matched_name: str):
    matched_name = matched_name.replace('release', 'test')
    return matched_name

def main():
    # copy all files from ../release here
    cmake_release_dir = f"{ROOT_DIR}/cmake/tests/release"
    cmake_test_dir = f"{ROOT_DIR}/cmake/tests/test"
    files = os.listdir(cmake_release_dir)
    # check if the directory exists
    if not os.path.exists(f"{ROOT_DIR}/cmake/tests/test"):
        os.makedirs(f"{ROOT_DIR}/cmake/tests/test")
    for file in files:
        if file.endswith('.cmake'):
            name = change_release(file)
            shutil.copy2(f"{cmake_release_dir}/{file}", f'{cmake_test_dir}/{name}')
    # iterate through all the files
    files = os.listdir(cmake_test_dir)
    for file in files:
        # check file extension .cmake
        if not file.endswith('.cmake'):
            continue
        # replace the lines with set(CGALPY_FIXED_LIBRARY_NAME OFF CACHE BOOL "" FORCE) to ON
        with open(f"{str(cmake_test_dir)}/{file}", 'r') as f:
            lines = f.readlines()
        with open(f"{str(cmake_test_dir)}/{file}", 'w') as f:
            for line in lines:
                if 'set(CGALPY_FIXED_LIBRARY_NAME OFF CACHE BOOL "" FORCE)' in line:
                    f.write('set(CGALPY_FIXED_LIBRARY_NAME ON CACHE BOOL "" FORCE)\n')
                else:
                    f.write(line)
        # check if the file name contains the word release
        if re.search(r'release', file):
            # change the name of the file
            new_name = change_release(file)
            # rename the file
            os.rename(file, new_name)
    

if __name__ == '__main__':
    main()
