# Micro CBOR Test

The MicroCbor test is based on gtest.

## CMake build

Build and run using cmake with the following commands:

```bash
mkdir build && cd build
cmake ..
make && ./microcbortest
```

For debugging, individual tests can be specified from command line options:

```bash
./microcbortest --gtest_filter=microcbor.basic
```

This can also be handy from gdb by running with these arguments:

```bash
r --gtest_filter=microcbor.basic
```
