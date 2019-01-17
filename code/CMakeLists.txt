cmake_minimum_required(VERSION 3.5.1)
project(ScreenRecorder)

set(CMAKE_CXX_STANDARD 11)

include_directories(
        src/headers_for_coding/boost/boost_1_67_0
        src/headers_for_coding/emsdk/1.38.21/include
)


add_subdirectory(src/renderers/index)
add_subdirectory(src/renderers/settings)
add_subdirectory(src/renderers/ScreenChooser)
add_subdirectory(src/renderers/VideoLibrary)
add_subdirectory(src/renderers/SRVPlayer)
add_subdirectory(src/renderers/about)
add_subdirectory(src/renderers/SrvReaderTaskImpl)
add_subdirectory(tests/ReadSRV)
