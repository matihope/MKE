add_library(Engine
        mke/Graphics/Window/Window.cpp
        # Math
        mke/Math/
)
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Engine PRIVATE GLAD glfw nlohmann_json)
