# Projects Settings
cmake_minimum_required (VERSION 3.1)
project (ODSconvert)

# The main executable
add_executable(main
        source/ODSconvert.cpp

#        osu-parser/src/OsrParser.cpp
        osu-parser/src/OsuParser.cpp
        osu-parser/src/OsUtils.cpp
)

include_directories(osu-parser/include)

set_property(TARGET main PROPERTY CXX_STANDARD 20)
target_compile_options(main PRIVATE -Wall -Wextra -O3)
