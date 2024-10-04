add_library(MKE-Common
    MKE/ResourceManager.cpp
    MKE/ResourceManager.hpp

    # MKE/Nodes/2d/RectShape.cpp
    # MKE/Nodes/2d/RectShape.hpp
    # MKE/WorldEntity.cpp
    # MKE/WorldEntity.hpp
)
target_link_libraries(MKE-Common MKE-Base MKE-Graphics)
target_include_directories(MKE-Common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
