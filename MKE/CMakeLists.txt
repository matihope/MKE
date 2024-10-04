add_library(MKE-Engine INTERFACE)

add_subdirectory(Base)
target_link_libraries(MKE-Engine INTERFACE MKE-Base)
add_subdirectory(Graphics)
target_link_libraries(MKE-Engine INTERFACE MKE-Graphics)
add_subdirectory(Common)
target_link_libraries(MKE-Engine INTERFACE MKE-Common)
