# dsun

data structures lib

## Build

#### cmake

require `Ninja` builder

```shell
cmake -S./ -B./build/ -G Ninja
cmake --build build/ --config Debug --target all --
```

## test

require `CTest`

after build run:

```shell
cd build
ctest -j10 -C Debug -T test --output-on-failure -R dsun_tests
```
