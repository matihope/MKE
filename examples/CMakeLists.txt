file(GLOB examples_list LIST_DIRECTORIES true *)

foreach(name ${examples_list})
    IF(IS_DIRECTORY ${name})
        add_subdirectory(${name})
    ENDIF()
endforeach()

add_example(hello_window_primitive USES glfw GLAD)
add_example(hello_window USES MKE-Engine)
add_example(hello_triangle USES MKE-Engine)
add_example(hello_rectangle USES MKE-Engine glfw)
add_example(fun USES MKE-Engine)
