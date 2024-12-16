FROM ubuntu:22.04
RUN apt update
RUN apt install -y software-properties-common
RUN apt install -y python3
RUN apt install -y python3-pip

# Minimal installation
# RUN apt install -y git
# RUN pip3 install git+https://mytaucgl@bitbucket.org/taucgl/cgal-python-bindings.git

# From local
WORKDIR /usr/src/cgal-python-bindings
COPY ./CMakeLists.txt ./
COPY ./conanfile.txt ./
COPY ./config.cmake ./
COPY ./pyproject.toml ./
COPY ./README.md ./
ADD ./src ./src
ADD ./cmake ./cmake
RUN python3 -m pip install --upgrade pip
RUN pip3 install -v --root-user-action ignore .
