#ifndef GB_MosquittoIO_hh
#define GB_MosquittoIO_hh 1
#include "mosquittopp.h"
#include <iostream>
#include <string>
#include <variant>
#include <vector>

namespace gramsballoon::pgrams {
namespace mqtt {
template <typename V>
struct mosquitto_message {
  int mid;
  int qos;
  bool retain;
  std::string topic;
  std::shared_ptr<V> payload;
  int payloadlen;
};
} // namespace mqtt
template <typename V>
class MosquittoIO: public mosqpp::mosquittopp {
public:
  MosquittoIO(const char *id, const std::string &host, int port) : mosquittopp(id) {
    HandleError(threaded_set(true));
    host_ = host;
    port_ = port;
    keepAlive_ = 60;
  }
  virtual ~MosquittoIO();
  void Connect();
  int Publish(const V &message, const std::string &topic, int qos = 0);
  int Publish(const std::vector<V> &message, const std::string &topic, int qos = 0);
  int Subscribe(const std::string &topic, int qos = 0);
  void on_connect(int rc) override;
  void on_disconnect(int rc) override;
  void on_publish(int mid) override;
  void on_message(const struct mosquitto_message *message) override;
  void on_subscribe(int mid, int qos_count, const int *granted_qos) override;
  const std::vector<std::shared_ptr<mqtt::mosquitto_message<V>>> &getPayload() const { return payLoad_; }
  std::string getHost() const { return host_; }
  int getPort() const { return port_; }
  static int HandleError(int error_code);
  int setVerbose(int verbose) { verbose_ = verbose; }
  void ClearPayload() { payLoad_.clear(); }

private:
  using mosqpp::mosquittopp::connect;
  using mosqpp::mosquittopp::disconnect;
  using mosqpp::mosquittopp::publish;
  std::vector<std::shared_ptr<mqtt::mosquitto_message<V>>> payLoad_;
  std::string host_;
  std::vector<std::string> topicSub_;

  int port_;
  int keepAlive_;
  int verbose_ = 0;
};
template <typename V>
int MosquittoIO<V>::Publish(const V &message, const std::string &topic, int qos) {
  return HandleError(publish(NULL, topic.c_str(), sizeof(V), &message, qos));
}
template <>
int MosquittoIO<std::string>::Publish(const std::string &message, const std::string &topic, int qos) {
  std::cout << "Publishing message: " << message << std::endl;
  return HandleError(publish(NULL, topic.c_str(), strlen(message.c_str()), message.c_str(), qos));
}
template <typename V>
int MosquittoIO<V>::Publish(const std::vector<V> &message, const std::string &topic, int qos) {
  int ret = 0;
  for (const auto &m: message) {
    ret &= Publish(m, topic, qos);
  };
}
template <typename V>
MosquittoIO<V>::~MosquittoIO() {
  HandleError(disconnect());
  HandleError(mosqpp::lib_cleanup());
}
template <typename V>
void MosquittoIO<V>::Connect() {
  HandleError(connect(host_.c_str(), port_, keepAlive_));
}
template <typename V>
void MosquittoIO<V>::on_connect(int rc) {
  if (verbose_ < 2) {
    return;
  }
  if (rc == 0) {
    std::cout << "Connected" << std::endl;
  }
  else {
    std::cout << "Connection failed: error code" << mosqpp::strerror(rc) << std::endl;
  }
}
template <typename V>
void MosquittoIO<V>::on_disconnect(int rc) {
  if (verbose_ < 2) {
    return;
  }
  if (rc == 0) {
    std::cout << "Disconnected" << std::endl;
  }
  else {
    std::cout << "Disconnection failed: error code" << mosqpp::strerror(rc) << std::endl;
  }
}
template <typename V>
void MosquittoIO<V>::on_publish(int mid) {
  if (verbose_ < 2) {
    return;
  }
  std::cout << "Published message with id: " << mid << std::endl;
}
template <typename V>
void MosquittoIO<V>::on_message(const struct mosquitto_message *message) {
  std::shared_ptr<mqtt::mosquitto_message<V>> m_sptr = std::make_shared<mqtt::mosquitto_message<V>>();
  m_sptr->mid = message->mid;
  m_sptr->qos = message->qos;
  m_sptr->retain = message->retain;
  m_sptr->topic = std::string(message->topic);
  m_sptr->payloadlen = message->payloadlen;
  V temp = *static_cast<V *>(message->payload);
  m_sptr->payload = std::make_shared<V>(temp);
  payLoad_.push_back(m_sptr);
  if (verbose_ < 3) {
    return;
  }
  std::cout << "Received topic: " << m_sptr->topic << std::endl;
  std::cout << "Received message: " << *m_sptr->payload << std::endl;
  std::cout << "Received length: " << m_sptr->payloadlen << std::endl;
}
template <>
void MosquittoIO<std::string>::on_message(const struct mosquitto_message *message) {
  std::shared_ptr<mqtt::mosquitto_message<std::string>> m_sptr = std::make_shared<mqtt::mosquitto_message<std::string>>();
  m_sptr->mid = message->mid;
  m_sptr->qos = message->qos;
  m_sptr->retain = message->retain;
  m_sptr->topic = std::string(message->topic);
  m_sptr->payloadlen = message->payloadlen;
  std::string temp(static_cast<char *>(message->payload));
  m_sptr->payload = std::make_shared<std::string>(temp);
  payLoad_.push_back(m_sptr);
  if (verbose_ < 3) {
    return;
  }
  std::cout << "Received topic: " << m_sptr->topic << std::endl;
  std::cout << "Received message: " << *m_sptr->payload << std::endl;
  std::cout << "Received length: " << m_sptr->payloadlen << std::endl;
}
template <typename V>
void MosquittoIO<std::vector<V>>::on_message(const struct mosquitto_message *message) {
  std::shared_ptr<mqtt::mosquitto_message<V>> m_sptr = std::make_shared<mqtt::mosquitto_message<std::vector<V>>>();
  m_sptr->mid = message->mid;
  m_sptr->qos = message->qos;
  m_sptr->retain = message->retain;
  m_sptr->topic = std::string(message->topic);
  m_sptr->payloadlen = message->payloadlen;
  V temp = *static_cast<V *>(message->payload);
  m_sptr->payload = std::make_shared<std::vector<V>>(temp);
  payLoad_.push_back(m_sptr);
  if (verbose_ < 3) {
    return;
  }
  std::cout << "Received topic: " << m_sptr->topic << std::endl;
  std::cout << "Received message: " << static_cast<int>((*m_sptr->payload)[0]) << std::endl;
  std::cout << "Received length: " << m_sptr->payloadlen << std::endl;
}
template <typename V>
int MosquittoIO<V>::Subscribe(const std::string &topic, int qos) {
  const int ret = HandleError(subscribe(NULL, topic.c_str(), qos));
  if (ret == 0) {
    topicSub_.push_back(topic);
  }
  return ret;
}
template <typename V>
void MosquittoIO<V>::on_subscribe(int mid, int qos_count, const int *granted_qos) {
  if (verbose_ < 2) {
    return;
  }
  std::cout << "Subscribed" << "\ngranted_qos: " << *granted_qos << std::endl;
}
template <typename V>
int MosquittoIO<V>::HandleError(int error_code) {
  if (error_code != mosq_err_t::MOSQ_ERR_SUCCESS) {
    std::cerr << "Error in Mosquitto!: " << mosqpp::strerror(error_code) << std::endl;
  }
  return error_code;
}
} // namespace gramsballoon::pgrams

#endif //GB_MosquittoIO_hh