cmake_minimum_required(VERSION 2.8.4)

### App name
project(CppTasks)
set ( APP_NAME CppTasks )

### Build types. May be: DEBUG|RELEASE|RELWITHDEBINFO|MINSIZEREL
set( CMAKE_BUILD_TYPE DEBUG )

##############################
# Sources
##############################

### Sources directory
set ( SRC_DIR ${PROJECT_SOURCE_DIR}/src )

### Main files
list(APPEND SRC ${SRC_DIR}/main.cpp )
#list(APPEND SRC ${SRC_DIR}/graphics/open_gl/Test.c )

### Code
list(APPEND SRC ${SRC_DIR}/utils/Console.h )
list(APPEND SRC ${SRC_DIR}/utils/Console.cpp )
list(APPEND SRC ${SRC_DIR}/utils/Generator.h )
list(APPEND SRC ${SRC_DIR}/utils/Generator.cpp )

list(APPEND SRC ${SRC_DIR}/mathf/Matrix.cpp )
list(APPEND SRC ${SRC_DIR}/mathf/Mathf.h )
list(APPEND SRC ${SRC_DIR}/mathf/Mathf.cpp )

list(APPEND SRC ${SRC_DIR}/parallel_tasks/hw1/HW1.h )
list(APPEND SRC ${SRC_DIR}/parallel_tasks/hw1/HW1.cpp )
list(APPEND SRC ${SRC_DIR}/parallel_tasks/hw2/HW2.h )
list(APPEND SRC ${SRC_DIR}/parallel_tasks/hw2/HW2.cpp )

### Tests
list(APPEND SRC ${SRC_DIR}/tests/HW1Tests.cpp )
list(APPEND SRC ${SRC_DIR}/tests/HW2Tests.cpp )
list(APPEND SRC ${SRC_DIR}/tests/FIOStreamsTests.cpp )

add_executable ( ${APP_NAME} ${SRC} )

##############################
# /Sources
##############################

### Libs dirs
#link_directories ( /path/to/lib_1 )
#link_directories ( /path/to/lib_2 )

### Libs names
#target_link_libraries( ${APP_NAME} name_1 )
#target_link_libraries( ${APP_NAME} name_2 )

### OpenMP
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

### GLUT
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)

### OPENGL
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

# create the openGL program "test"
target_link_libraries(${APP_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
