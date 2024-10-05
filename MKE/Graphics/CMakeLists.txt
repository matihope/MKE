add_library(MKE-Graphics
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
    MKE/Drawable.cpp
    MKE/Drawable.hpp
    MKE/RenderTarget.cpp
    MKE/RenderTarget.hpp
    MKE/DrawContext.cpp
    MKE/DrawContext.hpp
    MKE/VertexArray.cpp
    MKE/VertexArray.hpp
    MKE/RenderWindow.hpp

    MKE/Primitives/2d/RectPrimitive.cpp
    MKE/Primitives/2d/RectPrimitive.hpp
)
target_link_libraries(MKE-Graphics MKE-Base GLAD glfw stb)
target_include_directories(MKE-Graphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
