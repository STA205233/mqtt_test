#include "MosquittoIO.hh"
#include <chrono>
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
  MosquittoIO<std::string> mosq("test_pub", host, port);
  if (argc == 5) {
    std::string username = argv[3];
    std::string password = argv[4];
    mosq.username_pw_set(username.c_str(), password.c_str());
  }
  mosq.Connect();
  mosq.loop_start();
  std::string input_header = "This is MQTT test. No.";
  while (true) {
    if (i > 10) {
      break;
    }
    std::string input = input_header + std::to_string(i);
    mosq.Publish(input, "test/test1");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    i++;
  }
  MosquittoIO<std::string>::HandleError(mosq.loop_stop(true));

  return 0;
}