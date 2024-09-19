add_library(MKE-BASE
    # JsonBridge
    MKE/JsonBridge/JsonBridge.cpp
    MKE/JsonBridge/JsonBridge.hpp
    # Math
    MKE/Math/Base.cpp
    MKE/Math/Base.hpp
    MKE/Math/Geometry.cpp
    MKE/Math/Geometry.hpp
    MKE/Math/Matrix.cpp
    MKE/Math/Matrix.hpp
    MKE/Math/Rect.hpp
    MKE/Math/Vector2.cpp
    MKE/Math/Vector2.hpp
    # Random
    MKE/Random/Random.cpp
    MKE/Random/Random.hpp
    # ResourceManager
    MKE/ResourceManager/ResourceManager.cpp
    MKE/ResourceManager/ResourceManager.hpp
    # ResPath
    MKE/ResPath/ResPath.cpp
    MKE/ResPath/ResPath.hpp
    # Updatable
    MKE/Updatable/Updatable.hpp
    # WorldEntity
)
target_include_directories(MKE-BASE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(MKE-BASE PUBLIC nlohmann-json glfw GLAD)
