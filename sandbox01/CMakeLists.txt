project(sandbox01)

file(GLOB_RECURSE SRCS src/*.cpp)

add_executable(sandbox01 ${SRCS})
target_link_libraries(sandbox01 PRIVATE sfml-graphics sfml-window sfml-system)
target_compile_features(sandbox01 PRIVATE cxx_std_20)
