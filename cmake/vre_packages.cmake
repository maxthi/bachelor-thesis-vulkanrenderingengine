
# Git
find_package(Git)
if(NOT Git_FOUND)
	vre_msg_warning("No Git executable found. You may need it if you want to download&build assimp!")
endif()

# Qt 5
find_package(Qt5 COMPONENTS Widgets REQUIRED)