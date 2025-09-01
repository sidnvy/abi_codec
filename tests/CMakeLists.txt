set(NAME abi_tests)
project(${NAME})

# Ensure npm dependencies are installed
add_custom_target(npm_deps ALL
  COMMAND ${CMAKE_COMMAND} -E env NODE_OPTIONS= -- npm ci
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMENT "Installing npm dependencies"
)

# New trait-based tests (recommended)
add_executable(${NAME} abi_tests.cpp)
target_include_directories(${NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${NAME} abi_codec_deps)
add_dependencies(${NAME} npm_deps)
add_test(NAME ${NAME} COMMAND ${NAME} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set_tests_properties(${NAME} PROPERTIES
  ENVIRONMENT "ABI_SCRIPTS_DIR=${CMAKE_SOURCE_DIR}/scripts"
)
