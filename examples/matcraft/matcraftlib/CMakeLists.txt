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
        World.hpp
        PlayerUI.cpp
        PlayerUI.hpp
        Menu.cpp
        Menu.hpp
        Inventory.cpp
        Inventory.hpp
        DeathScreen.cpp
        DeathScreen.hpp
)
target_link_libraries(voxelslib MKE-Engine siv_perlin_noise)
target_include_directories(voxelslib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
