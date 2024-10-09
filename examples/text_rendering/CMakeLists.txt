add_example(text_rendering USES MKE-Engine)
target_link_libraries(text_rendering ${FREETYPE_LIBRARIES})
target_include_directories(text_rendering PRIVATE ${FREETYPE_INCLUDE_DIRS})
add_example_assets(text_rendering)
