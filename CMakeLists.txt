cmake_minimum_required (VERSION 3.1)
project (Fade)

file(GLOB fade_SRC
    "src/*.hpp"
    "src/*.cpp"
)

add_executable(Fade ${fade_SRC})
