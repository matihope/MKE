file(GLOB examples_list LIST_DIRECTORIES true RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *)

foreach(name ${examples_list})
    IF(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${name})
        IF(NOT ${name} STREQUAL "assets")
            add_subdirectory(${name})
        ENDIF()
    ENDIF()
endforeach()

add_example(hello_window_primitive USES glfw GLAD)
add_example(hello_window USES MKE-Engine)
add_example(hello_triangle USES MKE-Engine)
add_example(hello_rectangle USES MKE-Engine glfw)
add_example(fun USES MKE-Engine)

add_example(vertex_array USES MKE-Engine)
add_example_assets(vertex_array)

add_example(rect_primitive USES MKE-Engine)
add_example_assets(rect_primitive)

add_example(cube_primitive USES MKE-Engine)
add_example_assets(cube_primitive)
