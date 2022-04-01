#include "Server.hpp"

extern "C" {
    #include "table.h"
}

Server::Server(const string &name)
{
    const char *filename = name.c_str();
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, filename);
    
    if ((sock_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        throw "Server: socket";
    }
    
    unlink(filename);
    
    if (bind(sock_fd, (struct sockaddr *) &addr, sizeof addr) < 0) {
        throw "Server: bind";
    }
}

Server::~Server()
{
    shutdown(sock_fd, 2);
    close(sock_fd);
}

void
Server::server_listen(int backlog)
{
    if (listen(sock_fd, backlog) < 0) {
        throw "Server: listen";
    }
}

int
Server::server_accept()
{
    int fd;
    if ((fd = accept(sock_fd, nullptr, nullptr)) < 0) {
        throw "Server: accept";
    }
    return fd;
}

void
Server::server_send(int sd, const string &msg)
{
    const char *cmsg = msg.c_str();
    size_t len = strlen(cmsg);
    
    char len_buf[sizeof len];
    memcpy(&len_buf, &len, sizeof len);

    if (send(sd, len_buf, sizeof len, 0) < 0)
        throw "Server: send";
        
    char c;
    if (recv(sd, &c, 1, 0) < 0) {
        throw "Server: recv";
    }
    
    if (send(sd, cmsg, len, 0) < 0)
        throw "Server: send";
    
    if (recv(sd, &c, 1, 0) < 0) {
        throw "Server: recv";
    }
}

void
Server::server_send(int sd, const list<string> &lst)
{
    size_t len = lst.size();
    char len_buf[sizeof len];
    memcpy(&len_buf, &len, sizeof len);

    char c;

    if (send(sd, len_buf, sizeof len, 0) < 0)
        throw "Server: send";
    if (recv(sd, &c, 1, 0) < 0) {
        throw "Server: recv";
    }

    auto it = lst.cbegin();
    while (it != lst.cend()) {
        server_send(sd, *it);
        ++it;
    }
}

void
Server::server_recv(int sd, string &ret)
{
    size_t len;
    char len_buf[sizeof len];
    if (recv(sd, &len_buf, sizeof len, 0) < 0) {
        throw "Server: recv";
    }
    memcpy(&len, len_buf, sizeof len); 

    if (send(sd, "1", 1, 0) < 0)
        throw "Server: send";

    char *buf = new char[len + 1];
    if (recv(sd, buf, len, 0) < 0) {
        throw "Server: recv";
    }
    buf[len] = '\0';

    ret = string(buf);
    delete [] buf;

    if (send(sd, "1", 1, 0) < 0)
        throw "Server: send";
}

void
Server::server_recv(int sd, Poliz_elem &item)
{
    Poliz_type type;
    long num;
    string str;

    char type_buf[sizeof type];
    char num_buf[sizeof num];
    if (recv(sd, &type_buf, sizeof type, 0) < 0) {
        throw "Server: recv";
    }
    if (send(sd, "1", 1, 0) < 0)
        throw "Server: send";
    if (recv(sd, &num_buf, sizeof num, 0) < 0) {
        throw "Server: recv";
    }
    if (send(sd, "1", 1, 0) < 0)
        throw "Server: send";
    memcpy(&type, type_buf, sizeof type); 
    memcpy(&num, num_buf, sizeof num); 
    server_recv(sd, str);
    
    item = Poliz_elem(type, num, str);
}

void
Server::server_recv(int sd, vector<Poliz_elem> &vct)
{
    size_t size;
    char size_buf[sizeof size];
    Poliz_elem item;

    if (recv(sd, &size_buf, sizeof size, 0) < 0) {
        throw "Server: recv";
    }
    if (send(sd, "1", 1, 0) < 0)
        throw "Server: send";

    memcpy(&size, size_buf, sizeof size); 

    for (size_t i = 0; i < size; ++i) {
        server_recv(sd, item);
        vct.push_back(item);
    }
}

void
Server::server_recv(int sd, Query &query)
{
    char type_buf[sizeof query.cl_type];
    char cond_buf[sizeof query.cond_type];
    char neg_buf[sizeof query.neg];
    string str;

    if (recv(sd, &type_buf, sizeof type_buf, 0) < 0) {
        throw "Server: recv";
    }
    if (send(sd, "1", 1, 0) < 0)
        throw "Server: send";


    if (recv(sd, &cond_buf, sizeof cond_buf, 0) < 0) {
        throw "Server: recv";
    }
    if (send(sd, "1", 1, 0) < 0)
        throw "Server: send";


    if (recv(sd, &neg_buf, sizeof neg_buf, 0) < 0) {
        throw "Server: recv";
    }
    if (send(sd, "1", 1, 0) < 0)
        throw "Server: send";

    memcpy(&query.cl_type, type_buf, sizeof type_buf); 
    memcpy(&query.cond_type, cond_buf, sizeof cond_buf); 
    memcpy(&query.neg, neg_buf, sizeof neg_buf);

    server_recv(sd, query.table_name);

    server_recv(sd, query.data);
    server_recv(sd, query.expr_poliz);
    server_recv(sd, query.cond_poliz);
    server_recv(sd, query.constants);
}

void
Server::server_shutdown(int sd, int how)
{
    if (shutdown(sd, how) < 0) {
        throw "Server: shutdown";
    }
}

void
Server::server_close(int fd)
{
    if (close(fd) < 0) {
        throw "Server: close";
    }
}

void
Server::start(int sd)
{
    Query query;
    server_recv(sd, query);
    auto resp = query.execute();
    server_send(sd, resp);
}