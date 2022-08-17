cmake_minimum_required(VERSION 3.8)
project("vre")

# build the doxygen library?
option(BUILD_DOC "Build documentation" OFF)

#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON)

# define dependencies folder
set(VRE_DEPENDENCIES ${CMAKE_CURRENT_SOURCE_DIR}/dependencies)

# include additional cmake functionality
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
include(vre_logging)
include(vre_packages)
include(vre_utility)

# debug or release mode?
vre_utility_detect_compile_mode(VRE_DEBUG)

# download & build assimp
include(vre_assimp)
vre_assimp_get_assimp(VRE_ASSIMP_DESTINATION_FOLDER VRE_ASSIMP_LIB_MAIN VRE_ASSIMP_LIB_ZLIB VRE_ASSIMP_LIB_IRRXML)

# build documentation if VRE_GENERATE_DOXYGEN is ON
include(vre_doxygen)
vre_doxygen_create_docs()

# copyResourceFiles( projectname )
# copy all files from a /resources folder to the binary directory
# the binary output directory is compiler dependent
# and also depends on debug/release mode
function(copyResourceFiles projectname)
	SET(PROJ_SOURCEDIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")
	SET(PROJ_TARGETDIR "${CMAKE_CURRENT_BINARY_DIR}/${VRE_COMPILER_OUTPUT_PREFIX}")
	
	# if the directory with the prefix does not exist, try the directory wihout 2nd
	if(NOT EXISTS ${PROJ_TARGETDIR})
		vre_msg_warning("vre was told to use prefix, but prefix folder was not created! switching to no prefix!")
		SET(PROJ_TARGETDIR "${CMAKE_CURRENT_BINARY_DIR}")
	endif()

    copyResourceFilesFromTo(${projectname} ${PROJ_SOURCEDIR} ${PROJ_TARGETDIR})
endfunction()


# function copyResourceFilesFromTo( sourcedir, targetdir )
# copies the content of a given source directory into a given target directory
function(copyResourceFilesFromTo projectname sourcedir targetdir)
    if(EXISTS ${sourcedir} AND EXISTS ${targetdir})
            vre_msg_info("${projectname}: success: copyResourceFilesFromTo: post build copy ${sourcedir} to ${targetdir}")
            add_custom_command(TARGET ${projectname} POST_BUILD
                                       COMMAND ${CMAKE_COMMAND} -E copy_directory
                                       ${sourcedir} ${targetdir})
    else()
		vre_msg_warning("${projectname}: copyResourceFilesFromTo: one of the requested folders is not existing!")
        if(NOT EXISTS ${sourcedir})
                vre_msg_warning("${projectname}: copyResourceFilesFromTo: No resources were copied because resource directory doesn't exist! ResourceDir: ${sourcedir}")
        endif()
        if(NOT EXISTS ${targetdir})
			vre_msg_warning("${projectname}: copyResourceFilesFromTo: No resources were copied because target directory doesn't exist! TargetDir: ${targetdir}")
        endif()
    endif()
endfunction()


# function log_project_start( projectname )
# writes a visible line to the console, signaling the start of a process
function(log_project_start projectname)
	vre_msg_info("-------------------------- START ${projectname} --------------------------")
endfunction()


# function log_project_end( projectname )
# writes a well visible line to the console, signaling the end of a process
function(log_project_end projectname)
	vre_msg_info("END ${projectname}")
	vre_msg_info("")
endfunction()


# setup_project( projectname )
# call this function in a project cmake txt file
# does the basic setup (linking qt, vre_lib, source files, vulkan, etc.)
function(setup_project projectname)
	# set the project name here
	log_project_start(${projectname})
	project(${projectname} LANGUAGES CXX)

	set(CMAKE_INCLUDE_CURRENT_DIR ON)

	# Qt related 
	set(CMAKE_AUTOUIC ON)
	set(CMAKE_AUTOMOC ON)
	set(CMAKE_AUTORCC ON)

	# use c++ 17 std
	set(CMAKE_CXX_STANDARD 17)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)

	find_package(Qt5 COMPONENTS Widgets REQUIRED)

	file(GLOB_RECURSE src_project "*.cpp")
	file(GLOB_RECURSE header_project "*.hpp")
	file(GLOB_RECURSE ui_project "*.ui")

	include_directories("../../vre_lib/")

	SET(VRE_PROJECT_PATH "${CMAKE_SOURCE_DIR}/projects/${projectname}/")
	if(NOT EXISTS ${VRE_PROJECT_PATH})
		VRE_MSG_ERROR("The project directory name and the name passed to setup_project must be the same!")
	endif()
	VRE_MSG_INFO("configure_file ../../vre_lib/resource_path.hpp.in -> ${VRE_PROJECT_PATH}resource_path.hpp")
	configure_file("../../vre_lib/resource_path.cpp.in" "${VRE_PROJECT_PATH}resource_path.cpp")

	# locate vulkan sdk
	find_package(Vulkan REQUIRED)
	include_directories(${Vulkan_INCLUDE_DIRS})

	add_executable(${projectname} ${src_project} ${header_project} ${ui_project} "${VRE_PROJECT_PATH}resource_path.cpp")
	# add vre lib
	target_link_libraries(${projectname} vre_lib)
	# add qt widgets for gui apps
	target_link_libraries(${projectname} Qt5::Widgets)

	copyResourceFiles(${projectname})

	log_project_end(${projectname})
endfunction()


add_subdirectory("dependencies")
add_subdirectory("vre_lib")
add_subdirectory("projects")