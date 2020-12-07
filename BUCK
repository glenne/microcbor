export_file(
    name = "MicroCbor.hpp",
    src = "include/microcbor/MicroCbor.hpp",
    mode = "reference",
    visibility = [
        "PUBLIC",
    ],
)

oxx_test(
    name = "microcbortest",
    srcs = [
        "test/MicroCborTest.cpp",
    ],
    include_directories = [
        "include",
    ],
    preprocessor_flags = [
        "-DCONFIG_MICROCBOR_STD_VECTOR",
    ],
    raw_headers = [
        ":MicroCbor.hpp",
    ],
)
