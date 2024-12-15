
set(PROG_NAME mqtt_test)
add_library(${PROG_NAME} SHARED src/MosquittoIO.cc)
add_executable(${PROG_NAME}_pub pub.cc)
target_include_directories(${PROG_NAME}_pub PUBLIC include)
target_include_directories(${PROG_NAME} PUBLIC include)
target_link_libraries(${PROG_NAME} PkgConfig::Mosquitto)
target_link_libraries(${PROG_NAME}_pub PkgConfig::Mosquitto ${PROG_NAME})

