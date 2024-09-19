add_library(MKE-JsonBridge
    JsonBridge/JsonBridge.cpp
    JsonBridge/JsonBridge.hpp
)
target_include_directories(MKE-JsonBridge PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(MKE-JsonBridge PUBLIC nlohmann-json)
