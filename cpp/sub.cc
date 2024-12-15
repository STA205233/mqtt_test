#include "MosquittoIO.hh"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
using namespace gramsballoon::pgrams;
int main(int argc, char **argv) {
  if (argc != 3) {
    std::cout << argv[0] << " host port" << std::endl;
    return -1;
  }
  std::string host = argv[1];
  int port = std::atoi(argv[2]);
  int i = 0;
  MosquittoIO mosq("test_sub", host, port);
  mosq.Connect();
  mosq.Subscribe("test/test1");
  mosq.loop_start();
  while (true) {
    const auto &pay = mosq.getPayload();
    for (const auto &p: pay) {
      std::cout << "Received mid: " << p->mid << std::endl;
      std::cout << "Received Length: " << p->payloadlen << std::endl;
      std::cout << "Received topic: " << p->topic << std::endl;
      if (p->payloadlen == 0) {
        continue;
      }
      std::cout << "Received message: " << static_cast<char *>(p->payload.get())[0] << std::endl;
      std::cout << "qos: " << p->qos << std::endl;
    }
    mosq.ClearPayload();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  MosquittoIO::HandleError(mosq.loop_stop(true));
}