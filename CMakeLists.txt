cmake_minimum_required(VERSION 3.21)
project(Euterpe)

set(CMAKE_PREFIX_PATH "/Users/mikraselene/Qt/6.2.4/macos")
#set(CMAKE_PREFIX_PATH "D:/QT/QT6.2.4/6.2.4/mingw_64") # weiting's
#set(CMAKE_PREFIX_PATH "C:/Qt/Qt6.2/6.2.4/msvc2019_64/lib/cmake")
#set(CMAKE_PREFIX_PATH "D:/Qt/6.2.4/msvc2019_64/lib/cmake")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(QRC_SOURCES_FILES "UI/res.qrc")

find_package(Qt6 COMPONENTS
	Core
	Gui
	Sql
	Widgets
	Multimedia
	MultimediaWidgets
	REQUIRED
	)

qt6_add_resources(QRC_FILES ${QRC_SOURCES_FILES})
source_group("Resource Files" FILES ${QRC_SOURCES_FILES})
add_executable(Euterpe main.cpp Player/Player.cpp Player/Player.h UI/Player.ui PlayerController/PlayerController.cpp PlayerController/PlayerController.h MetaData/MetaDataFloatTable.h MetaData/MetaDataFloatTable.cpp UI/MetaDataFloatTable.ui ${QRC_FILES}
	MediaList/MediaList.h MediaList/MediaList.cpp
	PlayerController/PlayerShortcut.h PlayerController/PlayerShortcut.cpp Player/GetFrameData.h Player/GetFrameData.cpp MediaList/MediaItemBox.h MediaList/MediaItemBox.cpp UI/MediaItemBox.ui
	MediaList/MediaListSql.cpp MediaList/MediaListSql.h MetaData/MetaData.cpp MetaData/MetaData.h)

target_link_libraries(Euterpe
	Qt::Core
	Qt::Gui
	Qt::Sql
	Qt::Widgets
	Qt::Multimedia
	Qt::MultimediaWidgets
	)

