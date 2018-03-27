#ifndef PROXY_HPP
#define PROXY_HPP

#include <thread>
#include <atomic>

#include <queue>

extern "C"
{
  #include <sys/socket.h>
  #include <sys/types.h>
  #include <arpa/inet.h>
  
  #include <unistd.h>
  #include <string.h>
  
  #include <poll.h>
}

class Proxy
{
public:
  Proxy();
  Proxy( std::string, int );
  Proxy( const struct sockaddr_in );
  
  bool operator()();
  
  void start( const struct sockaddr_in );
  
  enum CALLBACK_TYPES
  {
    ON_SENT = 0, //Packet Sent by Client
    ON_RECEIVED =1, // Packet Received by DM Server
    ON_INJECT = 2, // Packet has been injected in the TCP Stream
    ON_STOP = 3,
    ON_ERROR = 4
  };
  void setCallback(void (*)(unsigned char*, size_t), enum CALLBACK_TYPES);
  
  void inject( const unsigned char*, size_t );
  void inject( const std::string );
  
  ~Proxy();
  
protected:
  void _proxy_loop();
  
private:
  std::thread *_loop;
  std::atomic<bool> _run;
  std::queue<std::string> _inject;
  
  void _sendviaCmdStr(int);
  
  int _proxyfd, _clientfd, _serverfd;
  
  std::array< std::function<void(unsigned char*, size_t)>, 5> _callbacks;
};

#endif //PROXY_HPP
