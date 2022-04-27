cmake_minimum_required(VERSION 3.21)
project(Euterpe)

set(CMAKE_PREFIX_PATH "/Users/mikraselene/Qt/6.2.4/macos")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
	Core
	Gui
	Widgets
	Multimedia
	MultimediaWidgets
	REQUIRED
	)

add_executable(Euterpe main.cpp Player.cpp Player.h Player.ui PlayerController.cpp PlayerController.h)
target_link_libraries(Euterpe
	Qt::Core
	Qt::Gui
	Qt::Widgets
	Qt::Multimedia
	Qt::MultimediaWidgets
	)

