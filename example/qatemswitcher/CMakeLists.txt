find_package(Qt6 REQUIRED COMPONENTS Core Gui Network Widgets )

add_executable(switcher main.cpp mainwindow.cpp mainwindow.h mainwindow.ui)

target_link_libraries(switcher PRIVATE Qt6::Core Qt6::Gui Qt6::Network  Qt6::Widgets qatemcontrol)
