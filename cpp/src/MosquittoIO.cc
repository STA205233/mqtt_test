#include "MosquittoIO.hh"
#include <iostream>
namespace gramsballoon::pgrams {
MosquittoIO::~MosquittoIO() {
  HandleError(disconnect());
  HandleError(mosqpp::lib_cleanup());
}

void MosquittoIO::Connect() {
  HandleError(connect(host_.c_str(), port_, keepAlive_));
}
void MosquittoIO::on_connect(int rc) {
  if (rc == 0) {
    std::cout << "Connected" << std::endl;
  }
  else {
    std::cout << "Connection failed: error code" << mosqpp::strerror(rc) << std::endl;
  }
}
void MosquittoIO::on_disconnect(int rc) {
  if (rc == 0) {
    std::cout << "Disconnected" << std::endl;
  }
  else {
    std::cout << "Disconnection failed: error code" << mosqpp::strerror(rc) << std::endl;
  }
}
void MosquittoIO::on_publish(int mid) {
  std::cout << "Published message with id: " << mid << std::endl;
}
void MosquittoIO::on_message(const struct mosquitto_message *message) {
  std::cout << "Received message: " << message->payload << std::endl;
}
void MosquittoIO::on_subscribe(int mid, int qos_count, const int *granted_qos) {
  std::cout << "Subscribed\ngranted_qos: " << *granted_qos << std::endl;
}
int MosquittoIO::HandleError(int error_code) {
  if (error_code != mosq_err_t::MOSQ_ERR_SUCCESS) {
    std::cerr << "Error in Mosquitto!: " << mosqpp::strerror(error_code) << std::endl;
  }
  return error_code;
}

} // namespace gramsballoon::pgrams