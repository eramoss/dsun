import os
import subprocess
import sys

def run_ctest(build_dir):
    """
    Run tests with CTest.

    Args:
        build_dir (str): Path to the build directory of the CMake project.
    """
    os.chdir(build_dir)

    ctest_command = ['ctest', "-V"]
    subprocess.run(ctest_command, check=True)

if __name__ == "__main__":
    script_dir = os.path.dirname(os.path.abspath(__file__))

    build_directory = os.path.join(script_dir, "../build")
    if not os.path.exists(build_directory):
        print("Build directory not found. Run the CMake build script first.")
        sys.exit(1)

    run_ctest(build_directory)
