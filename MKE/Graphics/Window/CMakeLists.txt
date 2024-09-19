add_library(MKE-Window
    MKE/Window.cpp
    MKE/Window.hpp
)
target_include_directories(MKE-Window PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(MKE-Window PRIVATE GLAD glfw MKE-Base)
