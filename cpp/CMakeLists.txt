
set(PROG_NAME mqtt_test)
add_executable(${PROG_NAME}_pub pub.cc)
add_executable(${PROG_NAME}_sub sub.cc)
target_include_directories(${PROG_NAME}_pub PUBLIC include)
target_include_directories(${PROG_NAME}_sub PUBLIC include)
target_link_libraries(${PROG_NAME}_pub PkgConfig::Mosquitto)
target_link_libraries(${PROG_NAME}_sub PkgConfig::Mosquitto)

