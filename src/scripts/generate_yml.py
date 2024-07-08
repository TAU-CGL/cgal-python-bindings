import click

def generate_yml(path_to_cmakes: str, compile_mode: str, cmake2example: dict) -> str:
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
  # for file in os.listdir(path_to_cmakes):
  #   name = file.split(".")[0]
    # if file.endswith(".cmake"):
    #   yml += f"""
    #     - step:
    #       name: Test {name}
    #       script:
    #         - cmake -C ../cmake/tests/release/{file} ../
    #         - make
    #         - pip install src/libs/cgalpy/dist/*.whl
    #     """
  for cmake_name, examples in cmake2example.items():
    yml += f"""
      - step:
        name: Test {cmake_name} examples
        script:
          - cmake -C ../{path_to_cmakes}/{cmake_name}_{compile_mode}.cmake ../
          - make
          - pip install src/libs/cgalpy/dist/*.whl
          - cd ../src/python_scripts # go to examples
          - ./compare_examples.sh ../../../cgal {" ".join(examples)} # run examples
      """
  
  return yml

@click.command()
@click.option('--mode', '-m', help='Compile mode [release/debug/test]', type=str, default="test", show_default=True)
@click.option('--cmake2example', '-c', help='Path to cmake2example file', type=str, default="example2cmake/cmake2example.txt")
@click.option('--output', '-o', help='Path to the bitbucket-pipelines.yml output file', type=click.Path(), default="../../bitbucket-pipelines.yml")
def main(mode, cmake2example, output):
  """This script generates bitbucket-pipelines.yml to test CGAL examples"""
  path_to_cmakes = f"cmake/tests/{mode}"
  with open(cmake2example, "r") as f:
    cmake2example_lines = f.readlines()
    cmake2example = {line.split()[0]:line.split()[1:] for line in cmake2example_lines}
  yml = generate_yml(path_to_cmakes, mode, cmake2example)
  with open(output, "w") as f:
    f.write(yml)
  click.echo(f"Succesfully generated yml file at {output}")

if __name__ == "__main__":
    main()

