#include "Server.hpp"

int main()
{
	string filename;
    filename = "amasvsvosmd";

	Server my_server(filename);
    my_server.server_listen(5);
    int sd;
    sd = my_server.server_accept();
    while(true) {
        my_server.start(sd);
    }

    my_server.server_shutdown(sd, 2);
    my_server.server_close(sd);

    return 0;
}