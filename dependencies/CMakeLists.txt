include(FetchContent)

set(NLOHMANN_JSON_VERSION 3.11.3)
set(STB_VERSION f75e8d1cad7d90d72ef7a4661f1b994ef78b4e31)
set(SIV_PERLIN_NOISE bdf39fe92b2a585cdef485bcec2bca8ab5614095)

FetchContent_Declare(
        json
        URL "https://github.com/nlohmann/json/archive/v${NLOHMANN_JSON_VERSION}.zip"
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
        stb
        URL "https://github.com/nothings/stb/archive/${STB_VERSION}.zip"
)
FetchContent_MakeAvailable(stb)
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})

FetchContent_Declare(
        siv_perlin_noise
        URL "https://github.com/Reputeless/PerlinNoise/archive/${SIV_PERLIN_NOISE}.zip"
)
FetchContent_MakeAvailable(siv_perlin_noise)
add_library(siv_perlin_noise INTERFACE)
target_include_directories(siv_perlin_noise INTERFACE ${siv_perlin_noise_SOURCE_DIR})

add_subdirectory(libs)
