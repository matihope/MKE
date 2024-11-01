add_library(MKE-Common
    MKE/Shaders/SimpleShader.cpp
    MKE/Shaders/SimpleShader.hpp
    MKE/ResourceManager.cpp
    MKE/ResourceManager.hpp
    MKE/VertexArray.cpp
    MKE/VertexArray.hpp
    MKE/Font.cpp
    MKE/Font.hpp
    MKE/Clock.cpp
    MKE/Clock.hpp
    MKE/Camera.cpp
    MKE/Camera.hpp
    MKE/Game.cpp
    MKE/Game.hpp
    MKE/WorldEntity.cpp
    MKE/WorldEntity.hpp

    MKE/Primitives/2d/RectPrimitive.cpp
    MKE/Primitives/2d/RectPrimitive.hpp
    MKE/Primitives/2d/Text.cpp
    MKE/Primitives/2d/Text.hpp
    MKE/Primitives/3d/CubePrimitive.cpp
    MKE/Primitives/3d/CubePrimitive.hpp

    MKE/Nodes/GUI/Label.cpp
    MKE/Nodes/GUI/Label.hpp
    MKE/Nodes/GUI/Clickable.cpp
    MKE/Nodes/GUI/Clickable.hpp
    MKE/Nodes/GUI/Button.cpp
    MKE/Nodes/GUI/Button.hpp
    MKE/Nodes/2d/RectShape.cpp
    MKE/Nodes/2d/RectShape.hpp
    MKE/Nodes/3d/CubeShape.cpp
    MKE/Nodes/3d/CubeShape.hpp

    MKE/Collisions/CollisionComponent/CollisionComponent.cpp
    MKE/Collisions/CollisionComponent/CollisionComponent.hpp
    MKE/Collisions/CollisionShape/CollisionShape.cpp
    MKE/Collisions/CollisionShape/CollisionShape.hpp
)
target_link_libraries(MKE-Common MKE-Base MKE-Graphics ${FREETYPE_LIBRARIES})
target_include_directories(MKE-Common PUBLIC ${FREETYPE_INCLUDE_DIRS})
target_include_directories(MKE-Common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
