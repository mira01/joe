#include <czmq.h>

int main () {

    zsock_t *server = zsock_new_router ("inproc://fmq");
    zsock_t *client = zsock_new_dealer ("inproc://fmq");

    zmsg_t *msg = zmsg_new ();
    zmsg_addstr (msg, "HELLO");
    zmsg_addstr (msg, "/etc/passwd");

    zmsg_send (&msg, client);
    if (!msg)
        zsys_debug ("msg == NULL");

    zmsg_t *msg2 = zmsg_recv (server);
    zmsg_print (msg2);

    char *command = zmsg_popstr (msg2);
    zsys_info (command);
    zstr_free (&command);
    zmsg_destroy (&msg2);

    zsock_destroy (&server);
    zsock_destroy (&client);

}
