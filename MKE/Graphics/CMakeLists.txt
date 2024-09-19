add_library(MKE-Graphics
    MKE/Window.cpp
    MKE/Window.hpp
    MKE/Color.cpp
    MKE/Color.hpp
)
target_link_libraries(MKE-Graphics MKE-Base GLAD glfw)
target_include_directories(MKE-Graphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
