add_library(voxelslib
        Chunk.hpp
        VoxelVertex.cpp
        VoxelVertex.hpp
        Chunk.cpp
        VoxelTextureFaces.cpp
        VoxelTextureFaces.hpp
        Player.cpp
        Player.hpp
        chunk_utils.hpp
        chunk_utils.cpp
        ray_cast_voxel.hpp
        World.cpp
        World.hpp)
target_link_libraries(voxelslib MKE-Engine)
target_compile_definitions(voxelslib PUBLIC "CUSTOM_ASSETS_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/../assets\"")
target_include_directories(voxelslib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
