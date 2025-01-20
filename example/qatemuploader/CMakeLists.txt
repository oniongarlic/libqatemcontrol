find_package(Qt6 REQUIRED COMPONENTS Core Gui Network)

add_executable(uploader main.cpp  qatemuploader.cpp  qatemuploader.h)

target_link_libraries(uploader PRIVATE Qt6::Core Qt6::Gui Qt6::Network qatemcontrol)
