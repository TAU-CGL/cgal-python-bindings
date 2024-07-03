# script that generates a yml file for every file in ../cmake/tests/release/*.cmake

import os
import sys

def generate_yml(path_to_cmakes: str):
  yml ="""
pipelines:
  default:
    - step: # sequential step
        image: ubuntu:latest
        script:
          - export DEBIAN_FRONTEND=noninteractive
          - export TZ=Asia/Kolkata
          - apt-get update
          - apt-get install -yq cmake g++ python3-pip python3 git python3.12-venv make libgmp3-dev libboost-all-dev libmpfr-dev qt6-base-dev libeigen3-dev
          - git clone https://github.com/CGAL/cgal.git
          - cd cgal
          - mkdir build
          - cd build
          - cmake ..
          - make install
          - cd ../..
          - python3 -m venv venv
          - source venv/bin/activate
          - python -m pip install build
          - git clone https://github.com/wjakob/nanobind.git
          - cd nanobind
          - git submodule update --init --recursive
          - export nanobind_DIR=$(pwd)
          - cd ..
          - mkdir build
          - cd build
    - parallel:
      steps:"""
  for file in os.listdir(path_to_cmakes):
    name = file.split(".")[0]
    if file.endswith(".cmake"):
      yml += f"""
        - step:
          name: Test {name}
          script:
            - cmake -C ../cmake/tests/release/{file} ../
            - make
            - pip install src/libs/cgalpy/dist/*.whl
        """
  return yml

if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.stderr.write("Usage: python generate_yml.py <path_to_cmakes>")
        sys.exit(1)
    path_to_cmakes = sys.argv[1]
    yml = generate_yml(path_to_cmakes)
    print(yml)

