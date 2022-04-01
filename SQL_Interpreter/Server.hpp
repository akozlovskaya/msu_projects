#ifndef __SERVER__ 
#define __SERVER__

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string>

#include "RDBMSTable.hpp"
#include "Parser.hpp"
#include "Query.hpp"

using std::string;

enum { S_BUF = 1024 };

class Server{
    int sock_fd;
    struct sockaddr_un addr;
public:
    Server(const string &);
    ~Server();

    void start(int);

    void server_listen(int);
    int server_accept();
    
    void server_send(int, const string &);
    void server_send(int, const list<string> &);
    void server_recv(int, string &);
    void server_recv(int, vector<Poliz_elem> &);
    void server_recv(int, Query &);
    void server_recv(int, Poliz_elem &);
    void server_shutdown(int, int);
    void server_close (int);
};

#endif  /* __Server__ */
