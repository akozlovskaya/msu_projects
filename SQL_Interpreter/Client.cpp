#include "Client.hpp"

Client::Client(const string &name)
{
    const char *filename = name.c_str();
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, filename);
    if ((sock_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        throw "Client: socket";
}

void
Client::client_connect()
{
    if (connect(sock_fd, (struct sockaddr *)&addr, sizeof addr) < 0)
        throw "Client: connect";
}

void
Client::client_send(const string &msg)
{
    const char *cmsg = msg.c_str();
    size_t len = strlen(cmsg);
    
    char len_buf[sizeof len];
    memcpy(&len_buf, &len, sizeof len); 

    if (send(sock_fd, len_buf, sizeof len, 0) < 0)
        throw "Client: send";

    char c;
    
    if (recv(sock_fd, &c, 1, 0) < 0)
        throw "Client: send";

    if (send(sock_fd, cmsg, len, 0) < 0)
        throw "Client: send";

    if (recv(sock_fd, &c, 1, 0) < 0)
        throw "Client: send";
}

void
Client::client_send(const Poliz_elem &x)
{
    Poliz_type type = x.get_poliz_type();
    long num = x.get_poliz_num();

    char type_buf[sizeof type];
    char num_buf[sizeof num];

    memcpy(&type_buf, &type, sizeof type); 
    memcpy(&num_buf, &num, sizeof num); 

    char c;

    if (send(sock_fd, type_buf, sizeof type, 0) < 0)
        throw "Client: send";
    if (recv(sock_fd, &c, 1, 0) < 0)
        throw "Client: send";
    if (send(sock_fd, num_buf, sizeof num, 0) < 0)
        throw "Client: send";
    if (recv(sock_fd, &c, 1, 0) < 0)
        throw "Client: send";
    client_send(x.get_poliz_str());
}

void
Client::client_send(const vector<Poliz_elem> &vec)
{
    size_t len = vec.size();
    char buf[sizeof len];

    memcpy(&buf, &len, sizeof len);

    char c;
    if (send(sock_fd, buf, sizeof len, 0) < 0)
        throw "Client: send";
    if (recv(sock_fd, &c, 1, 0) < 0) {
        throw "Client: send";
    }
    for (size_t i = 0; i < len; ++i) {
    	client_send(vec[i]);
    }
}

void
Client::client_send(const Query &query)
{

    Clause_type type = query.get_type();
    Condition_type cond = query.get_cond();
    bool neg = query.get_neg();


    char type_buf[sizeof type];
    char cond_buf[sizeof cond];
    char neg_buf[sizeof neg];

    char c;

    memcpy(&type_buf, &type, sizeof type); 
    memcpy(&cond_buf, &cond, sizeof cond); 
    memcpy(&neg_buf, &neg, sizeof neg);
    if (send(sock_fd, type_buf, sizeof type, 0) < 0)
        throw "Client: send";
    if (recv(sock_fd, &c, 1, 0) < 0)
        throw "Client: send";
    if (send(sock_fd, cond_buf, sizeof cond, 0) < 0)
        throw "Client: send";
    if (recv(sock_fd, &c, 1, 0) < 0)
        throw "Client: send";
    if (send(sock_fd, neg_buf, sizeof neg, 0) < 0)
        throw "Client: send";
    if (recv(sock_fd, &c, 1, 0) < 0)
        throw "Client: send";
    client_send(query.get_name());

    client_send(query.get_data());
    client_send(query.get_expr_poliz());
    client_send(query.get_cond_poliz());
    client_send(query.get_const());
}

string
Client::client_recv()
{
    size_t len;
    char len_buf[sizeof len];
    if (recv(sock_fd, &len_buf, sizeof len, 0) < 0) {
        throw "Client: recv";
    }
    memcpy(&len, len_buf, sizeof len); 
    
    if (send(sock_fd, "1", 1, 0) < 0)
        throw "Client: send";
    
    char *buf = new char[len + 1];
    if (recv(sock_fd, buf, len, 0) < 0) {
        throw "Client: recv";
    }
    if (send(sock_fd, "1", 1, 0) < 0)
        throw "Client: send";
    buf[len] = '\0';
    string ret = string(buf);
    delete [] buf;
    return ret;
}

void
Client::client_recv(list<string> &lst)
{
    size_t size;
    char size_buf[sizeof size];

    if (recv(sock_fd, &size_buf, sizeof size, 0) < 0) {
        throw "Client: recv";
    }
    if (send(sock_fd, "1", 1, 0) < 0)
        throw "Client: send";

    memcpy(&size, size_buf, sizeof size);
    
    for (size_t i = 0; i < size; ++i) {
        lst.push_back(client_recv());
    }
}

bool
Client::processing()
{
	Parser pars;
    if (pars.analyze()) {
        
        try {
            if (pars.isend()) return false;
            
            const Query & q = pars.get_query();
	        client_send(q);

            list<string> resp;
            client_recv(resp);
            
            auto it = resp.cbegin();
            if (resp.size() > 1 || resp.front() != Query::success_msg) {
                while (it != resp.cend()) {
                    cout << *it++;
                }
            }
        } catch (...) {
            cout << Query::system_error << endl;
        }
        
    } else {
        if (pars.isend()) return false;
        cout << Query::syntax_error << endl;
    }
    return true;
}