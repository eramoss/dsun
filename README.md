# dsun

data structures lib

## Build

Require `Cmake` tools  https://cmake.org/ and C++20 (recommend ninja builder).
You can also run as a [python script](#python-scripts).

To build inside `build/` you can run:
``` sh
 mkdir build
 cd build
 cmake -DCMAKE_BUILD_TYPE=Debug ../
```

and then:
### Linux
``` sh
  make
```
### Windows
``` sh
  cmake --build . --config Debug
```

### Manual
You can also build your projects manually passing include path:
```sh
  clang++ ./examples/huffman.cpp -I/path/to/dsun/ -I/path/to/standard/includes -std=c++20
```

## test

require `CTest`
You can also choose to test with [python](#python-scripts)
But manually just need it after build:

```sh 
  cd build
  ctest -j10 -C Debug -T test --output-on-failure # or just ctest
```

## Python scripts
There are scripts to build, test and configure vscode (in my style)
to build and test just run:
``` sh
  python3 scripts/build.py
  python3 scripts/test.py
```


