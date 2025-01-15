add_library(voxelslib
        Chunk.hpp
        VoxelVertex.cpp
        VoxelVertex.hpp
        Chunk.cpp)
target_link_libraries(voxelslib MKE-Engine)
target_include_directories(voxelslib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
