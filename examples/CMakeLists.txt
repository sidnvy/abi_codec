set(NAME user_usage_clean)
project(${NAME})

# Ensure npm dependencies are installed
add_custom_target(npm_deps_examples ALL
  COMMAND ${CMAKE_COMMAND} -E env NODE_OPTIONS= -- npm ci
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMENT "Installing npm dependencies for examples"
)

# Build the example executable
add_executable(${NAME} user_usage_clean.cpp)
target_include_directories(${NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_dependencies(${NAME} npm_deps_examples)

# Link against boost if needed
find_package(Boost REQUIRED)
if(Boost_FOUND)
  target_link_libraries(${NAME} Boost::boost)
endif()
