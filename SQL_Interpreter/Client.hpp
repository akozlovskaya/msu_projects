#ifndef __CLIENT__
#define __CLIENT__

#include <sys/socket.h>
#include <sys/un.h>
#include <string>
#include <vector>
#include <iostream>

#include "Parser.hpp"
#include "Query.hpp"

using std::string;
using std::vector;
using std::cout;
using std::endl;

enum { C_BUF = 1024 };

class Client {
    int sock_fd;
    struct sockaddr_un addr;
public:
    Client(const string &);
    
    void client_connect();
    void client_send(const string &);
    void client_send(const Poliz_elem &);
    void client_send(const Query &);
    void client_send(const vector<Poliz_elem> &);
    string client_recv();
    void client_recv(list<string> &);
    bool processing();
};

#endif /* __CLIENT__ */
