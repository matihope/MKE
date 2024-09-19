add_library(MKE-JsonBridge
    MKE/JsonBridge.cpp
    MKE/JsonBridge.hpp
)
target_include_directories(MKE-JsonBridge PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(MKE-JsonBridge PUBLIC nlohmann_json)
