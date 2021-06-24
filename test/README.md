# Micro CBOR Test

The MicroCbor test is based on gtest and is found in the test/ directory.

## Test Build

Build and run using cmake with the following commands:

```bash
cd test
mkdir build && cd build
cmake ..
make && ./microcbortest
```

## Test Debugging

For debugging, individual tests can be specified from command line options:

```bash
./microcbortest --gtest_filter=microcbor.basic
```

This can also be handy from gdb by running with these arguments:

```bash
r --gtest_filter=microcbor.basic
```
