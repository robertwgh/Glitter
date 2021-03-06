cmake_minimum_required(VERSION 3.0)
project(Glitter)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(Glitter/Vendor/glfw)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(Glitter/Vendor/assimp)

option(BUILD_BULLET2_DEMOS OFF)
option(BUILD_CPU_DEMOS OFF)
option(BUILD_EXTRAS OFF)
option(BUILD_OPENGL3_DEMOS OFF)
option(BUILD_UNIT_TESTS OFF)
add_subdirectory(Glitter/Vendor/bullet)


option(BUILD_SOIL OFF)
add_subdirectory(Glitter/Vendor/Simple-OpenGL-Image-Library)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(Glitter/Headers/
                    Glitter/Vendor/assimp/include/
                    Glitter/Vendor/bullet/src/
                    Glitter/Vendor/glad/include/
                    Glitter/Vendor/glfw/include/
                    Glitter/Vendor/glm/
                    Glitter/Vendor/stb/
                    Glitter/Vendor/Simple-OpenGL-Image-Library/src/)

file(GLOB VENDORS_SOURCES Glitter/Vendor/glad/src/glad.c)
file(GLOB PROJECT_HEADERS Glitter/Headers/*.hpp)
file(GLOB PROJECT_SOURCES Glitter/Sources/*.cpp)
file(GLOB PROJECT_SHADERS Glitter/Shaders/*.comp
                          Glitter/Shaders/*.frag
                          Glitter/Shaders/*.geom
                          Glitter/Shaders/*.vert)
file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          Readme.md
                         .gitattributes
                         .gitignore
                         .gitmodules)
file(GLOB IMAGE_FILES	  image/*.jpg)						 

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Vendors" FILES ${VENDORS_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")


# To add new tests, just add the project name here. 
set(APP_LIST code5.1 code5.2 ex5.1 ex5.2 ex5.3 
             code6.1 code6.2 code6.3 ex6.1 ex6.2 ex6.3
             code7.1 code8.1 code9.1 code9.2 code9.3
             code10.1 code10.2 code10.3 code10.4 code10.5 code10.6)

# generate all binaries files into build/bin directory.			 
if(WIN32)
foreach(config ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${config} CONFIG)
    #set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG} "${PCL_OUTPUT_LIB_DIR}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG} "${CMAKE_BINARY_DIR}/bin")
    #set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG} "${PCL_OUTPUT_BIN_DIR}")
  endforeach(config)
else(WIN32)
  #set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PCL_OUTPUT_LIB_DIR}")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
  #set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PCL_OUTPUT_LIB_DIR}")
endif(WIN32)


foreach (APP ${APP_LIST})
    file(GLOB SOURCE_FILE Glitter/Sources/${APP}.cpp)
    add_executable(${APP} ${SOURCE_FILE} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES})
    target_link_libraries(${APP} assimp glfw SOIL
                            ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
                            BulletDynamics BulletCollision LinearMath)
    set_target_properties(${APP} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
ENDFOREACH(APP)

# copy image files into the binary folder.
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
configure_file(${IMAGE_FILES} ${CMAKE_BINARY_DIR}/bin COPYONLY)
