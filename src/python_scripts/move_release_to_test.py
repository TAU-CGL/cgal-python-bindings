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
    files = os.listdir(f"{ROOT_DIR}/cmake/tests/release")
    # check if the directory exists
    if not os.path.exists(f"{ROOT_DIR}/cmake/tests/test"):
        os.makedirs(f"{ROOT_DIR}/cmake/tests/test")
    for file in files:
        if file.endswith('.cmake'):
            # shutil.copy2(f'../release/{file}', f'./{file}')
            shutil.copy2(f"{ROOT_DIR}/cmake/tests/release/{file}", f'{ROOT_DIR}/cmake/tests/test/{file}')
    # get the current directory
    current_dir = os.getcwd()
    # get the list of all the files in the current directory
    files = os.listdir(current_dir)
    # iterate through all the files
    for file in files:
        # check file extension .cmake
        if not file.endswith('.cmake'):
            continue
        # replace the lines with set(CGALPY_FIXED_LIBRARY_NAME OFF CACHE BOOL "" FORCE) to ON
        with open(file, 'r') as f:
            lines = f.readlines()
        with open(file, 'w') as f:
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
