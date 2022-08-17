# info message
function(vre_msg_info text)
	message("[vre][info] ${text}")
endfunction()

# warning message
function(vre_msg_warning text)
	message("[vre][warning] ${text}")
endfunction()

# error message and stop cmake
function(vre_msg_error text)
	message(FATAL_ERROR "[vre][error] ${text}")
endfunction()