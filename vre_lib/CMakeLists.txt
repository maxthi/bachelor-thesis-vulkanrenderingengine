cmake_minimum_required(VERSION 3.8)

log_project_start("vre_lib")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# vulkan
find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})
set(vulkanheader ${Vulkan_INCLUDE_DIR}/vulkan/vulkan.h)

# collect source files for vre_lib
file(GLOB_RECURSE src "${PROJECT_SOURCE_DIR}" "*.cpp")
file(GLOB_RECURSE header "${PROJECT_SOURCE_DIR}" "*.hpp")
add_library(vre_lib STATIC ${src} ${header} ${vulkanheader})

# linking Qt libs
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core)
target_link_libraries(vre_lib Qt5::Widgets)

# linking vulkan libs
target_link_libraries (vre_lib ${Vulkan_LIBRARIES})

#vk_mem_alloc
include_directories("../dependencies/vk_mem_alloc")
include_directories("-I/../dependencies/vk_mem_alloc")
target_link_libraries(vre_lib vk_mem_alloc)

#link assimp
if(EXISTS "${VRE_ASSIMP_DESTINATION_FOLDER}")
	message("Linking assimp: Main lib: ${VRE_ASSIMP_LIB_MAIN} ZLib:${VRE_ASSIMP_LIB_ZLIB} IrrXml: ${VRE_ASSIMP_LIB_IRRXML}")
	target_link_libraries(vre_lib "${VRE_ASSIMP_LIB_MAIN}")
	target_link_libraries(vre_lib "${VRE_ASSIMP_LIB_ZLIB}")
	target_link_libraries(vre_lib "${VRE_ASSIMP_LIB_IRRXML}")

	message("Assimp include dir: ${VRE_ASSIMP_DESTINATION_FOLDER}/include")
	target_include_directories(vre_lib PUBLIC "${VRE_ASSIMP_DESTINATION_FOLDER}/include")
else()
	message(FATAL_ERROR "Assimp lib build directory not found! Looking in: (${VRE_ASSIMP_DESTINATION_FOLDER}/) \n Did you forget to build assimp?")
endif()

# copy resource directory to build directory
# remove this and hardcopy shaders into code
if(EXISTS ${CMAKE_SOURCE_DIR}/src/resources)
    message("Copying resources ${CMAKE_SOURCE_DIR}/src/resources to ${CMAKE_BINARY_DIR}/resources ")
    add_custom_command(TARGET vre_lib POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/src/resources ${CMAKE_CURRENT_BINARY_DIR}/resources)
endif()

log_project_end("vre_lib")
