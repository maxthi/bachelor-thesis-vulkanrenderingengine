
# generate the doxygen documentation automatically
# if it doesn't work, you can use doxygen directly to create the docs
function(vre_doxygen_create_docs)
	if(${BUILD_DOC})
		find_package(Doxygen)
		if (DOXYGEN_FOUND)
			# create doxyfile configuration
			set(DOXYGEN_INFILE "${CMAKE_CURRENT_SOURCE_DIR}/vre_lib/Doxyfile.in")
			set(DOXYGEN_OUTFILE "${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile")
			configure_file(${DOXYGEN_INFILE} ${DOXYGEN_OUTFILE})
	
			# generate
			vre_msg_info("Building doxygen documentation ...")
			execute_process(COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUTFILE} RESULT_VARIABLE rv)
			vre_msg_info("Building documentation exited with result:'${rv}'")
		else()
			vre_msg_info("Could not find doxygen to autogenerate the documentation!")
		endif()
	else()
		vre_msg_info("Skipped building doxygen documentation. Set BUILD_DOC to ON to autogenerate it.")
	endif()
endfunction()