file(GLOB SOURCES "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

# soil
include_directories(${CMAKE_SOURCE_DIR}/lib/soil/src)
target_link_libraries(${PROJECT_NAME} SOIL)

# opengl
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})

# glew
find_package(GLEW REQUIRED)
target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES})

# glfw3
find_package(glfw3 REQUIRED)
target_link_libraries(${PROJECT_NAME} ${GLFW3_LIBRARY})
