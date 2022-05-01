cmake_minimum_required(VERSION 3.21)
project(Euterpe)

#set(CMAKE_PREFIX_PATH "/Users/mikraselene/Qt/6.2.4/macos")
set(CMAKE_PREFIX_PATH "D:/QT/QT6.2.4/6.2.4/mingw_64") # weiting's

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(QRC_SOURCES_FILES "res.qrc")

find_package(Qt6 COMPONENTS
	Core
	Gui
	Widgets
	Multimedia
	MultimediaWidgets
	REQUIRED
	)

qt6_add_resources(QRC_FILES ${QRC_SOURCES_FILES})
source_group("Resource Files" FILES ${QRC_SOURCES_FILES})

add_executable(Euterpe main.cpp Player.cpp Player.h Player.ui PlayerController.cpp PlayerController.h
 FloatTable.h FloatTable.cpp FloatTable.ui ${QRC_FILES})

target_link_libraries(Euterpe
	Qt::Core
	Qt::Gui
	Qt::Widgets
	Qt::Multimedia
	Qt::MultimediaWidgets
	)

