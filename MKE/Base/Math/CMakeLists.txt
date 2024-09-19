add_library(MKE-Math
    MKE/Math/Base.cpp
    MKE/Math/Base.hpp
    MKE/Math/Geometry.cpp
    MKE/Math/Geometry.hpp
    MKE/Math/Matrix.cpp
    MKE/Math/Matrix.hpp
    MKE/Math/Rect.hpp
    MKE/Math/Vector2.cpp
    MKE/Math/Vector2.hpp
)
target_include_directories(MKE-JsonBridge PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(MKE-JsonBridge PUBLIC nlohmann-json)
