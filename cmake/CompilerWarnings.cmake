add_library(project_warnings INTERFACE)

set(GCC_CLANG_WARNINGS
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Wcast-align
    -Wunused
    -Woverloaded-virtual
    -Wconversion
    -Wsign-conversion
    -Wnull-dereference
    -Wdouble-promotion
    -Wformat=2
    -Wimplicit-fallthrough
)

set(MSVC_WARNINGS
    /W4
    /permissive-
    /w14242 /w14254 /w14263 /w14265 /w14287
    /w14296 /w14311 /w14545 /w14546 /w14547
    /w14549 /w14555 /w14619 /w14640 /w14826
    /w14905 /w14906 /w14928
)

target_compile_options(project_warnings INTERFACE
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:${GCC_CLANG_WARNINGS}>
    $<$<CXX_COMPILER_ID:MSVC>:${MSVC_WARNINGS}>
)
