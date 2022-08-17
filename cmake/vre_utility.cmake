
# check whether compile mode is relase or debug
# result stored in ${identifier_to_use}
function(vre_utility_detect_compile_mode identifier_to_use)
	if("${CMAKE_CONFIGURATION_TYPES}" STREQUAL "Release" OR "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
		vre_msg_info("Detected compilemode: Release")
		set("${identifier_to_use}" false PARENT_SCOPE)
	else()
		vre_msg_info("Detected compilemode: Debug")
		set("${identifier_to_use}" true PARENT_SCOPE)
	endif()
endfunction()