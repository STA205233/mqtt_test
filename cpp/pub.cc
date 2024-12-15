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
  MosquittoIO mosq("test", host, port);
  mosq.Connect();
  mosq.loop_start();
  while (true) {
    std::cout << "Enter message: ";
    std::string input;
    std::cin >> input;
    if (input == "exit") {
      std::cout << "Exiting..." << std::endl;
      break;
    }
    mosq.Publish<std::string>(input, "test/test1");
    i++;
  }
  MosquittoIO::HandleError(mosq.loop_stop(true));

  return 0;
}