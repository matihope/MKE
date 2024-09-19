include(FetchContent)

set(NLOHMANN_JSON_VERSION 3.11.3)

FetchContent_Declare(
        json
        URL "https://github.com/nlohmann/json/archive/v${NLOHMANN_JSON_VERSION}.zip"
)

# nlohmann-json
FetchContent_MakeAvailable(json)

