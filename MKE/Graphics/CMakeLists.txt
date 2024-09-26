add_library(MKE-Graphics
    MKE/Window.cpp
    MKE/Window.hpp
    MKE/Color.cpp
    MKE/Color.hpp
    MKE/Shader.cpp
    MKE/Shader.hpp
    MKE/Image.cpp
    MKE/Image.hpp
    MKE/Texture.cpp
    MKE/Texture.hpp
)
target_link_libraries(MKE-Graphics MKE-Base GLAD glfw stb)
target_include_directories(MKE-Graphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
