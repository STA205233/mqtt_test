#include "MosquittoIO.hh"
#include <iostream>
#include <string>
#include <thread>
using namespace gramsballoon::pgrams;
int main(int argc, char **argv) {
  if (argc != 3 && argc != 5) {
    std::cout << argv[0] << " host port (username) (password)" << std::endl;
    return -1;
  }
  std::string host = argv[1];
  int port = std::atoi(argv[2]);
  int i = 0;
  MosquittoIO<std::vector<uint8_t>> mosq("test_sub", host, port);
  if (argc == 5) {
    std::string username = argv[3];
    std::string password = argv[4];
    mosq.username_pw_set(username.c_str(), password.c_str());
  }
  mosq.setVerbose(4);
  mosq.Connect();
  mosq.Subscribe("test/test1");
  while (true) {
    mosq.loop();
    const auto &pay = mosq.getPayload();
    if (i > 10) {
      break;
    }
    for (const auto &p: pay) {
      std::cout << "Received mid: " << p->mid << std::endl;
      std::cout << "Received Length: " << p->payloadlen << std::endl;
      std::cout << "Received topic: " << p->topic << std::endl;
      if (p->payloadlen == 0) {
        continue;
      }
      std::cout << "Received message: " << static_cast<int>((*p->payload)[0]) << std::endl;
      std::cout << "qos: " << p->qos << std::endl;
      i++;
    }
    mosq.ClearPayload();
  }
  mosq.HandleError(mosq.loop());
  const auto &pay = mosq.getPayload();
  for (const auto &p: pay) {
    std::cout << "Received mid: " << p->mid << std::endl;
    std::cout << "Received Length: " << p->payloadlen << std::endl;
    std::cout << "Received topic: " << p->topic << std::endl;
    if (p->payloadlen == 0) {
      continue;
    }
    std::cout << "Received message: " << static_cast<int>((*p->payload)[0]) << std::endl;
    std::cout << "qos: " << p->qos << std::endl;
  }
  i++;
  mosq.ClearPayload();
  return 0;
}