macro(conan)
    if(NOT EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
        string(REPLACE "." ";" VERSION_LIST ${CMAKE_CXX_COMPILER_VERSION})
        list(GET VERSION_LIST 0 VERSION_1)
        list(GET VERSION_LIST 1 VERSION_2)
        set(VERSION "${VERSION_1}.${VERSION_2}")
        execute_process(COMMAND conan install -s compiler=clang -s compiler.version=${VERSION} -s compiler.libcxx=libc++ ${CMAKE_SOURCE_DIR})
    endif()

    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
endmacro()