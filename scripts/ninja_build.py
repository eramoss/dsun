import os
import subprocess
import sys

def build_cmake_project(source_dir, build_dir, config):
    """
    Build CMake project.

    Args:
        source_dir (str): Path to the source directory of the CMake project.
        build_dir (str): Path to the directory where the build will be performed.
        config (str): Build configuration (Debug or Release).
    """

    if not os.path.exists(build_dir):
        os.makedirs(build_dir)

    os.chdir(build_dir)

    cmake_command = ['cmake', '-GNinja', '-DCMAKE_BUILD_TYPE=' + config, source_dir]
    subprocess.run(cmake_command, check=True)

    build_command = ['ninja']
    subprocess.run(build_command, check=True)

if __name__ == "__main__":
    self_dir = os.path.dirname(os.path.abspath(__file__))
    source_directory = os.path.join(self_dir, "..")
    build_directory =  os.path.join(self_dir, "../build")

    if len(sys.argv) != 2 or sys.argv[1] not in ['Debug', 'Release']:
        config = 'Debug'
    else:
        config = sys.argv[1]

    build_cmake_project(source_directory, build_directory, config)
