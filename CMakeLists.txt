cmake_minimum_required (VERSION 3.8)
project (Fade)

file(GLOB_RECURSE fade_SRC
	"src/*.hpp"
	"src/*.cpp"
)

add_executable(Fade ${fade_SRC})
set_target_properties(Fade PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED YES
)

if(MSVC)
	# no console in release builds
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
else()
	message("testtesttest")
	target_compile_options(Fade PUBLIC "$<$<CONFIG:DEBUG>:-Wall;-pedantic;-Wextra>")
	target_compile_options(Fade PUBLIC "$<$<CONFIG:RELEASE>:-Wall;-pedantic;-Wextra;-O3;-march=native>")
endif()

#SFML
find_package(SFML COMPONENTS system graphics window audio QUIET)
if (NOT SFML_FOUND)
	message("SFML not found. Attempting to use local submodule version.")
	set(SFML_BUILD_GRAPHICS TRUE)
	set(SFML_BUILD_WINDOW TRUE)
	set(BUILD_SHARED_LIBS FALSE)
	set(SFML_STATIC_LIBRARIES TRUE)
	set(SFML_BUILD_AUDIO TRUE)
	set(SFML_BUILD_EXAMPLES FALSE)
	set(SFML_BUILD_DOC FALSE)
	set(SFML_BUILD_NETWORK FALSE)
	target_include_directories(Fade PRIVATE "dependencies/SFML/include")
	add_subdirectory("dependencies/SFML")
endif(NOT SFML_FOUND)
target_link_libraries(Fade sfml-system sfml-graphics sfml-window sfml-audio)