#ifndef GB_MosquittoIO_hh
#define GB_MosquittoIO_hh 1
#include "mosquittopp.h"
#include <iostream>
#include <string>
#include <variant>
#include <vector>

namespace gramsballoon::pgrams {
namespace mqtt {
struct mosquitto_message {
  int mid;
  int qos;
  bool retain;
  std::string topic;
  std::shared_ptr<void> payload;
  int payloadlen;
};
} // namespace mqtt
class MosquittoIO: public mosqpp::mosquittopp {
public:
  MosquittoIO(const char *id, const std::string &host, int port) : mosquittopp(id) {
    HandleError(mosqpp::lib_init());
    host_ = host;
    port_ = port;
    keepAlive_ = 60;
  }
  virtual ~MosquittoIO();
  void Connect();
  template <typename T>
  int Publish(const T &message, const std::string &topic, int qos = 0);
  template <typename T>
  int Publish(const std::vector<T> &message, const std::string &topic, int qos = 0);
  int Subscribe(const std::string &topic, int qos = 0);
  void on_connect(int rc) override;
  void on_disconnect(int rc) override;
  void on_publish(int mid) override;
  void on_message(const struct mosquitto_message *message) override;
  void on_subscribe(int mid, int qos_count, const int *granted_qos) override;
  const std::vector<std::shared_ptr<mqtt::mosquitto_message>> &getPayload() const { return payLoad_; }
  std::string getHost() const { return host_; }
  int getPort() const { return port_; }
  static int HandleError(int error_code);
  void ClearPayload() { payLoad_.clear(); }

private:
  using mosqpp::mosquittopp::connect;
  using mosqpp::mosquittopp::disconnect;
  using mosqpp::mosquittopp::publish;
  std::vector<std::shared_ptr<mqtt::mosquitto_message>> payLoad_;
  std::string host_;
  std::vector<std::string> topicSub_;

  int port_;
  int keepAlive_;
};
template <typename T>
int MosquittoIO::Publish(const T &message, const std::string &topic, int qos) {
  return HandleError(publish(NULL, topic.c_str(), sizeof(T), &message, qos));
}
template <>
int MosquittoIO::Publish<std::string>(const std::string &message, const std::string &topic, int qos) {
  std::cout << "Publishing message: " << message << std::endl;
  std::cout << message.size() << std::endl;
  return HandleError(publish(NULL, topic.c_str(), strlen(message.c_str()), message.c_str(), qos));
}
template <typename T>
int MosquittoIO::Publish(const std::vector<T> &message, const std::string &topic, int qos) {
  int ret = 0;
  for (const auto &m: message) {
    ret &= Publish(m, topic, qos);
  };
}
} // namespace gramsballoon::pgrams

#endif //GB_MosquittoIO_hh