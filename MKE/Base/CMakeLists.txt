add_library(MKE-Base

    # Math
    MKE/Math/Base.cpp
    MKE/Math/Base.hpp
    MKE/Math/Geometry.cpp
    MKE/Math/Geometry.hpp
    MKE/Math/Matrix.hpp
    MKE/Math/MatrixUtils.hpp
    MKE/Math/Rect.hpp
    MKE/Math/Vector.cpp
    MKE/Math/Vector.hpp

    # JsonBridge
    MKE/JsonBridge.cpp
    MKE/JsonBridge.hpp

    # Random
    MKE/Random.cpp
    MKE/Random.hpp

    # ResPath
    MKE/ResPath.cpp
    MKE/ResPath.hpp

    # Updatable
    MKE/Updatable.hpp

    # Transformable
    MKE/Transformable.cpp
    MKE/Transformable.hpp

    # Ints
    MKE/Ints.hpp

    # StrUtils
    MKE/StrUtils.hpp
)
target_include_directories(MKE-Base PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(MKE-Base nlohmann_json)
