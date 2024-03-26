import os
import subprocess
import sys
import build
import ninja_build as ninja

def run_ctest(build_dir):
    '''
    Run tests with CTest.

    Args:
        build_dir (str): Path to the build directory of the CMake project.
    '''

    os.chdir(build_dir)
    ctest_command = ['ctest', '-V' ,'-C' , 'Debug', '--output-on-failure']
    try:
        subprocess.run(ctest_command, check=True)
    except subprocess.CalledProcessError:
        sys.exit(1)

if __name__ == '__main__':
    self_dir = os.path.dirname(os.path.abspath(__file__))
    source_dir = os.path.join(self_dir, '..')
    build_dir =  os.path.join(self_dir,'../build')
    
    if  not os.path.exists(os.path.join(build_dir, "build.ninja")):
        build.build_cmake_project(source_dir, build_dir, 'Debug')
    else:
        ninja.build_cmake_project(source_dir, build_dir, 'Debug')
       

    if not os.path.exists(build_dir):
        print('Build directory not found. Run the CMake build script first.')
        sys.exit(1)

    run_ctest(build_dir)
