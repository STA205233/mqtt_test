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
  std::shared_ptr<mqtt::mosquitto_message> m_sptr = std::make_shared<mqtt::mosquitto_message>();
  m_sptr->mid = message->mid;
  m_sptr->qos = message->qos;
  m_sptr->retain = message->retain;
  m_sptr->topic = std::string(message->topic);
  m_sptr->payloadlen = message->payloadlen;
  m_sptr->payload = std::make_shared<char>(message->payloadlen);
  memcpy(m_sptr->payload.get(), message->payload, message->payloadlen);
  std::cout << "Received topic: " << m_sptr->topic << std::endl;
  std::cout << "Received message: " << static_cast<char *>(m_sptr->payload.get()) << std::endl;
  payLoad_.push_back(m_sptr);
}
int MosquittoIO::Subscribe(const std::string &topic, int qos) {
  const int ret = HandleError(subscribe(NULL, topic.c_str(), qos));
  if (ret == 0) {
    topicSub_.push_back(topic);
  }
  return ret;
}
void MosquittoIO::on_subscribe(int mid, int qos_count, const int *granted_qos) {
  std::cout << "Subscribed" << "\ngranted_qos: " << *granted_qos << std::endl;
}
int MosquittoIO::HandleError(int error_code) {
  if (error_code != mosq_err_t::MOSQ_ERR_SUCCESS) {
    std::cerr << "Error in Mosquitto!: " << mosqpp::strerror(error_code) << std::endl;
  }
  return error_code;
}

} // namespace gramsballoon::pgrams