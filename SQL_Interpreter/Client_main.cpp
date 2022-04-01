#include "Client.hpp"

int main ()
{
    const string HELLO_STR = "Welcome!\nTo exit, enter \"q\" on a separate line.\n";
    const string REC_STR = "Enter your request ...";

    string filename;
    filename = "amasvsvosmd";

    cout << HELLO_STR << endl;

    try {
        Client my_client(filename);
        my_client.client_connect();
        bool working = true;
        while(working) {
            cout << REC_STR << endl;
            working = my_client.processing();
        }

    } catch (...) {
        cout << Query::system_error << endl;
        exit(1);
    }

	return 0;
}