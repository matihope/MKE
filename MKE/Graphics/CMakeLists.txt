add_library(MKE-Graphics
    MKE/Init.cpp
    MKE/Init.hpp
    MKE/Window.cpp
    MKE/Window.hpp
    MKE/Color.hpp
    MKE/Shader.cpp
    MKE/Shader.hpp
    MKE/Image.cpp
    MKE/Image.hpp
    MKE/Texture.cpp
    MKE/Texture.hpp
    MKE/Vertex.cpp
    MKE/Vertex.hpp
    MKE/Drawable.hpp
    MKE/RenderTarget.cpp
    MKE/RenderTarget.hpp
    MKE/DrawContext.cpp
    MKE/DrawContext.hpp
    MKE/RenderWindow.cpp
    MKE/RenderWindow.hpp
    MKE/RenderTexture.cpp
    MKE/RenderTexture.hpp
    MKE/View.hpp
    MKE/View.cpp
)
target_link_libraries(MKE-Graphics MKE-Base GLAD glfw stb)
target_include_directories(MKE-Graphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
