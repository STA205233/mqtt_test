#ifndef GB_MosquittoIO_hh
#define GB_MosquittoIO_hh 1
#include "mosquittopp.h"
#include <string>
#include <vector>

namespace gramsballoon::pgrams {
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
  int Publish(const std::string &message, const std::string &topic);
  template <typename T>
  int Publish(const T &message, const std::string &topic);
  template <typename T>
  int Publish(const std::vector<T> &message, const std::string &topic);
  void on_connect(int rc) override;
  void on_disconnect(int rc) override;
  void on_publish(int mid) override;
  void on_message(const struct mosquitto_message *message) override;
  void on_subscribe(int mid, int qos_count, const int *granted_qos) override;
  //void on_unsubscribe(int mid) override;
  const std::vector<std::shared_ptr<mosquitto_message>> &getPayload() const { return payLoad_; }
  std::string getHost() const { return host_; }
  int getPort() const { return port_; }
  static int HandleError(int error_code);

private:
  using mosqpp::mosquittopp::connect;
  using mosqpp::mosquittopp::disconnect;
  using mosqpp::mosquittopp::publish;
  std::vector<std::shared_ptr<mosquitto_message>> payLoad_;
  std::string host_;
  std::vector<std::string> topicSub_;

  int port_;
  int keepAlive_;
};
template <typename T>
int MosquittoIO::Publish(const T &message, const std::string &topic) {
  return HandleError(publish(NULL, topic.c_str(), sizeof(T), &message, 0));
}
template <>
int MosquittoIO::Publish<std::string>(const std::string &message, const std::string &topic) {
  return HandleError(publish(NULL, topic.c_str(), message.size(), message.c_str(), 0));
}
template <typename T>
int MosquittoIO::Publish(const std::vector<T> &message, const std::string &topic) {
  int ret = 0;
  for (const auto &m: message) {
    ret &= Publish(m, topic);
  };
}
} // namespace gramsballoon::pgrams

#endif //GB_MosquittoIO_hh