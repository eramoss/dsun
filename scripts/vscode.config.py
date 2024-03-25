'''Write my configs of vscode to use cpp without have a stroke :)'''

import json
import os
import sys
import subprocess

config_json = {
  "env": {
    "myIncludePath": ["${workspaceFolder}/include", "${workspaceFolder}/src"],
    "myDefines": ["DEBUG", "MY_FEATURE=1"]
  },
  "configurations": [
    {
      "name": "Linux",
      "compilerPath": "/usr/bin/clang",
      "compilerArgs": ["-m32"],
      "intelliSenseMode": "linux-clang-x64",
      "includePath": ["${myIncludePath}", "/usr/include"],
      "defines": ["${myDefines}"],
      "cStandard": "c17",
      "cppStandard": "c++20",
      "configurationProvider": "ms-vscode.cmake-tools",
      "forcedInclude": ["${workspaceFolder}/common.h"],
      "compileCommands": "${workspaceFolder}/build/compile_commands.json",
      "dotconfig": "${workspaceFolder}/.config",
      "mergeConfigurations": True,
      "customConfigurationVariables": {
        "custom_vars" : "here"
      },
      "browse": {
        "path": ["${myIncludePath}", "/usr/include", "${workspaceFolder}"],
        "limitSymbolsToIncludedHeaders": True,
        "databaseFilename": "${workspaceFolder}/.vscode/browse.vc.db"
      }
    },
    {
      "name": "Mac",
      "compilerPath": "/usr/bin/clang",
      "intelliSenseMode": "macos-clang-x64",
      "includePath": ["${myIncludePath}"],
      "defines": ["${myDefines}"],
      "cStandard": "c11",
      "cppStandard": "c++17",
      "macFrameworkPath": ["/System/Library/Frameworks", "/Library/Frameworks"],
      "browse": {
        "path": ["${myIncludePath}", "${workspaceFolder}"]
      }
    },
    {
      "name": "Win32",
      "compilerPath": "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.28.29333/bin/Hostx64/x64/cl.exe",
      "intelliSenseMode": "windows-msvc-x64",
      "includePath": ["${myIncludePath}"],
      "defines": ["${myDefines}", "_WINDOWS"],
      "cStandard": "c17",
      "cppStandard": "c++20",
      "windowsSdkVersion": "10.0.19041.0",
      "browse": {
        "path": ["${myIncludePath}", "${workspaceFolder}"]
      }
    }
  ],
  "version": 4,
  "enableConfigurationSquiggles": True
}

def write_on_vscode_config():
    """
    Write the configurations to the .vscode/c_cpp_properties.json file.
    """
    vscode_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "../.vscode")
    if not os.path.exists(vscode_dir):
        os.makedirs(vscode_dir)

    subprocess.run('rm -rf .vscode/c_cpp_properties.json', shell=True)
    subprocess.run(["touch", os.path.join(vscode_dir, "c_cpp_properties.json")], check=True)

    settings_file = os.path.join(vscode_dir, "c_cpp_properties.json")

    with open(settings_file, "w") as file:
        json.dump(config_json, file, indent=4)

if __name__ == "__main__":
    write_on_vscode_config()
    print("Configurations written to .vscode/c_cpp_properties.json.")
    sys.exit(0)