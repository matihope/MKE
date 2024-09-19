add_library(MKE-Base INTERFACE)

add_subdirectory(JsonBridge)
target_link_libraries(MKE-JsonBridge)

add_subdirectory(Math)
target_link_libraries(MKE-Math)

add_subdirectory(Random)
target_link_libraries(MKE-Random)

add_subdirectory(ResPath)
target_link_libraries(MKE-ResPath)

add_subdirectory(Updatable)
target_link_libraries(MKE-Updatable)
