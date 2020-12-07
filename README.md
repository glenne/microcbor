# CBOR

- [CBOR](#cbor)
  - [What is CBOR](#what-is-cbor)
  - [What is MicroCbor](#what-is-microcbor)
  - [Including in your build](#including-in-your-build)
  - [Serialization](#serialization)
  - [Deserialization](#deserialization)
  - [Arrays](#arrays)
  - [Example](#example)
  - [Companion Projects](#companion-projects)
  - [Testing](#testing)

## What is CBOR

CBOR (Concise Binary Object Representation) is an Internet standard encoding for structured data defined in [RFC7049](https://tools.ietf.org/html/rfc7049). It defines how various types of data used in programming can be encoded to a stream of bytes to be exchanged over an IPC or communication medium.

“The Concise Binary Object Representation (CBOR) is a data format whose design goals include the possibility of extremely small code size, fairly small message size, and extensibility without the need for version negotiation.”

For example the following data structure in JSON notation can be serialized to binary and deserialized from binary by utilizing CBOR encoding provided by this library.

```json
{
  "t": 72.0,
  "v": 3,
  "sn": "123-456"
}
```

More CBOR details are available at [https://cbor.io](https://cbor.io).

## What is MicroCbor

MicroCbor is a C++ implementation of CBOR implemented as a single header file plus some associated unit tests.

The data structure referenced previously would be encoded with C++ using the following MicroCbor code:

```cpp
    uint8_t   buf[200]; // Serialization output buffer
    MicroCbor cbor(buf, sizeof(buf));

    cbor.startMap();
    cbor.add("t", 72.0f);
    cbor.add("v", 3);
    cbor.add("sn", "123-456");
    cbor.endMap();
```

The types to serialized are implied from the arguments provided. It is also permissible to explicitly inform the type to be used:

```cpp
    cbor.add<int32_t>("parm", value);
```

## Including in your build

Add the following to CMakeLists.txt to pull from git and include in your project.  CMake 3.14 is
needed as it includes FetchContent and the FetchContent_MakeAvailable function.

```cmake

cmake_minimum_required(VERSION 3.14.0)
#==============================================================================
# Load MicroCbor from git
include(FetchContent)

FetchContent_Declare(
  microcbor
  GIT_REPOSITORY https://github.com/glenne/microcbor.git
  GIT_TAG        main
)
FetchContent_MakeAvailable(microcbor)
#==============================================================================
target_link_libraries( YourLibraryName
    INTERFACE microcbor
)
```

In your source file:

```cpp
#include "microcbor/MicroCbor.hpp"
```

## Serialization

To serialize:

1. Allocate some memory
2. Call startMap() to start a key/value pair dictionary
3. Call add(key,value) for each item to serialize
4. Call endMap() to end the dictionary

In most cases, serializing will be inline code stuffing bytes into the output buffer and will not require any function calls.

## Deserialization

1. Initialize with a cbor encoded buffer.
2. Call `get<T>(key, defaultValue)` to retrieve a value.

Getters are assumed to never fail and either return the default value provided or the value contained in the serialized stream. This allows code to be written without a sea of if/else clauses and provides a vaccine for version-itis. In other words, newer code can ask for a property it expects and proceed normally using a default even if the property was not provided by the sender.

## Arrays

Arrays are serialized by copying into the output buffer. On reading, arrays retrieve a pointer to the array data contained in the serialized stream. The pointer can used as-is for the lifetime of the serialized stream or it can be used to copy the array to some other location.

By default, arrays are aligned in the output serialization buffer on natural boundaries. That is, an array of int32_t values would insure the output buffer has the start of the array on an 4 byte boundary. This should reduce the need for copies by allowing the array to be used 'in place'.

The following code illustrates obtaining an array from a serialized stream. The return value from getPointer is actually a structure with a pointer and length:

```cpp
    auto array = cbor.getPointer<int32_t>("pts", nullptr); // default is nullptr
    if (array.p == nullptr)
    {
        LOG_ERROR("Array not provided\n");
    }
    else
    {
        cout << "The array is " << array.length << " elements long" << std::endl;
    }
```

## Example

This example from the unit tests illustrates some of the forms. See the unit test for more examples.

```cpp
TEST(microcbor, ints)
{
    uint8_t   buf[200];
    MicroCbor cbor(buf, sizeof(buf));
    cbor.startMap();
    cbor.add("true", true);
    cbor.add("false", false);
    cbor.add<int8_t>("i8", -80);
    cbor.add<int16_t>("i16", -16000);
    cbor.add<int32_t>("i32", -32000000);

    cbor.add<uint8_t>("ui8", 80);
    cbor.add<uint16_t>("ui16", 16000);
    cbor.add<uint32_t>("ui32", 32000000);
    cbor.endMap();

    cbor.restart();
    ASSERT_EQ(true, cbor.get<bool>("true", false));
    ASSERT_EQ(false, cbor.get<bool>("false", true));

    ASSERT_EQ(-80, cbor.get<int8_t>("i8", 0));
    ASSERT_EQ(-16000, cbor.get<int16_t>("i16", 0));
    ASSERT_EQ(-32000000, cbor.get<int32_t>("i32", 0));

    ASSERT_EQ(-80, cbor.get("i8", int8_t(0)));
    ASSERT_EQ(-16000, cbor.get("i16", int16_t(0)));
    ASSERT_EQ(-32000000, cbor.get("i32", int32_t(0)));

    ASSERT_EQ(80, cbor.get<uint8_t>("ui8", 0));
    ASSERT_EQ(16000, cbor.get<uint16_t>("ui16", 0));
    ASSERT_EQ(32000000, cbor.get<uint32_t>("ui32", 0));

    ASSERT_EQ(80, cbor.get("ui8", uint8_t(0)));
    ASSERT_EQ(16000, cbor.get("ui16", uint16_t(0)));
    ASSERT_EQ(32000000, cbor.get("ui32", uint32_t(0)));
}
```

## Companion Projects

For a nice structured data abstraction capabile of serializing to and from CBOR, see the
[KArgMap project](https://github.com/glenne/kargmap)
which provides a fantastic C++ interface for structured data and utilizes this project for CBOR support.

## Testing

1. From test/, create a build directory and navigate into it `mkdir build && cd build`

2. Run cmake `cmake ..`

3. Run make `make`

4. Execute the Test `microcbortest`
