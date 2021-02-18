cmake_minimum_required(VERSION 3.16.3)


set(PROJECT_NAME Minecraft)
set(TARGET_NAME Minecraft)


# Create solution
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/CMake/)
set(IMGUI_SOURCES ${PROJECT_SOURCE_DIR}/vendor/imgui)
file(GLOB IMGUI_FILES "${IMGUI_SOURCES}/*.cpp" "${IMGUI_SOURCES}/*.h")

# Create Project
include(CMake/CMakeLists_sources.cmake)

message( ${${TARGET_NAME}_SOURCES} )

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${${TARGET_NAME}_SOURCES})
add_executable(${TARGET_NAME} ${${TARGET_NAME}_SOURCES} ${IMGUI_FILES})
set_property(TARGET ${TARGET_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_property(TARGET ${TARGET_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
if (MSVC)
    string (REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT}")
	target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
endif()


# Source
set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/source)
set(SCRIPT_SOURCE ${PROJECT_SOURCE_DIR}/_scripts/generate_cmake_sources.sh)

# Vendor
set(VENDOR_DIR ${PROJECT_SOURCE_DIR}/vendor)
set(GLAD_SOURCE ${VENDOR_DIR}/glad45/src/glad.c)
set(GLAD_INCLUDE ${VENDOR_DIR}/glad45/include)
set(STB_INCLUDE ${VENDOR_DIR}/STB)
set(GLFW_DIR ${VENDOR_DIR}/glfw3)
set(Irrklang_DIR ${VENDOR_DIR}/irrKlang/)
set(GLM_DIR ${VENDOR_DIR}/glm/)
set(GLFW3_ROOT ${GLFW_DIR})
set(Irrklang_ROOT_DIR ${Irrklang_DIR})


# Link library
find_package(GLFW3 REQUIRED)
find_package(IRRKLANG REQUIRED)
target_include_directories(${TARGET_NAME}
							PRIVATE
							${SOURCE_DIR}
							${STB_INCLUDE}
							${GLAD_INCLUDE}
							${GLFW3_INCLUDE_DIR}
							${GLM_DIR}
							${IMGUI_SOURCES}
							${Irrklang_INCLUDE_DIR})

target_sources(${TARGET_NAME} PRIVATE ${SCRIPT_SOURCE})
target_sources(${TARGET_NAME} PRIVATE ${GLAD_SOURCE})
target_sources(${TARGET_NAME} PRIVATE ${GLM_DIR}util/glm.natvis)
set_source_files_properties(${GLAD_SOURCE} PROPERTIES LANGUAGE CXX)


target_compile_definitions(${TARGET_NAME} PRIVATE GLFW_INCLUDE_NONE)
target_link_libraries(${TARGET_NAME} PRIVATE ${GLFW3_LIBRARY}
						${Irrklang_IMPORT_LIB})
string(REPLACE .lib .dll GLFW_DLL ${GLFW3_LIBRARY})
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GLFW_DLL} $<TARGET_FILE_DIR:${TARGET_NAME}>)
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Irrklang_LIBRARY} $<TARGET_FILE_DIR:${TARGET_NAME}>)


set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})
