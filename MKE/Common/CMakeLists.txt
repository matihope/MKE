add_library(MKE-Common
    MKE/Shaders/SimpleShader.cpp
    MKE/Shaders/SimpleShader.hpp
    MKE/ResourceManager.cpp
    MKE/ResourceManager.hpp
    MKE/VertexArray.cpp
    MKE/VertexArray.hpp

    MKE/Primitives/2d/RectPrimitive.cpp
    MKE/Primitives/2d/RectPrimitive.hpp
    MKE/Primitives/3d/CubePrimitive.cpp
    MKE/Primitives/3d/CubePrimitive.hpp

    # MKE/Nodes/2d/RectShape.cpp
    # MKE/Nodes/2d/RectShape.hpp
    # MKE/WorldEntity.cpp
    # MKE/WorldEntity.hpp
)
target_link_libraries(MKE-Common MKE-Base MKE-Graphics)
target_include_directories(MKE-Common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
