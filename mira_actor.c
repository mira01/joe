#include <czmq.h>

void handle_error(zsock_t *server, zframe_t *routing_id, const char* err_message);

static
void s_actor1 (zsock_t *pipe, void *args)
{
    char *name = strdup ((char*) args);
    zpoller_t *poller = zpoller_new (pipe, NULL);

    zsock_t *server = zsock_new_router("tcp://192.168.1.184:7777");
    zpoller_add(poller, server);

    // to signal to runtime it should spawn the thread
    zsock_signal (pipe, 0);
    zsys_debug ("%s:\tstarted", name);

    while (!zsys_interrupted) {

        void *which = zpoller_wait (poller, -1);

        if (!which)
            break;

        if (which == pipe) {
            zmsg_t *msg = zmsg_recv (pipe);
            char *command = zmsg_popstr (msg);
            zsys_info ("Got API command=%s", command);
            if (streq (command, "$TERM")) {
                zstr_free (&command);
                zmsg_destroy (&msg);
                break;
            }
            zstr_free (&command);
            zmsg_destroy (&msg);
        }
        else if (which == server){

            zmsg_t *msg = zmsg_recv (server);
            zmsg_print (msg);
            zframe_t *routing_id = zmsg_pop (msg);
            char *first_chunk = zmsg_popstr (msg);
            char *second_chunk = zmsg_popstr (msg);
            

            if (streq(first_chunk, "HELLO")){
                if (second_chunk == NULL || streq(second_chunk, "")){
                    handle_error(server, routing_id, "PROTOCOL VIOLATION: no filename");
                }
                else{
                    zmsg_t *response = zmsg_new ();
                    zmsg_add (response, routing_id);
                    zmsg_addstr (response, "READY");
                    zmsg_send (&response, server);
                }
            }
            else{
                handle_error(server, routing_id, "PROTOCOL VIOLATION: no HELLO");
            }

            zstr_free(&first_chunk);
            zstr_free(&second_chunk);
            zmsg_destroy (&msg);
        }
    }
    zpoller_remove(poller, server);
    zsock_destroy (&server);
    zpoller_destroy (&poller);
    zstr_free (&name);
}

void s_client_actor (zsock_t *pipe, void *args)
{
    char *name = strdup ((char*) args);
    zpoller_t *poller = zpoller_new (pipe, NULL);

    zsock_t *client = zsock_new_dealer("tcp://192.168.1.184:7777");
    zpoller_add(poller, client);

    // to signal to runtime it should spawn the thread
    zsock_signal (pipe, 0);
    zsys_debug ("%s:\tstarted", name);

    while (!zsys_interrupted) {

        void *which = zpoller_wait (poller, -1);

        if (!which)
            break;

        if (which == pipe) {
            zmsg_t *msg = zmsg_recv (pipe);
            char *command = zmsg_popstr (msg);
            zsys_info ("Got API command=%s", command);
            if (streq (command, "$TERM")) {
                zstr_free (&command);
                zmsg_destroy (&msg);
                break;
            }
            else if (streq (command, "START")) {
                zmsg_t *msg1 = zmsg_new ();
                zmsg_addstr (msg1, "HELLO");
                zmsg_addstr (msg1, "/etc/passwd");
                zmsg_send (&msg1, client);
            }
            else if (streq (command, "ERR2")) {
                zmsg_t *msg1 = zmsg_new ();
                zmsg_addstr (msg1, "HELLO");
                zmsg_send (&msg1, client);
            }
            else if (streq (command, "ERR1")) {
                zmsg_t *msg1 = zmsg_new ();
                zmsg_addstr (msg1, "NOHELLO");
                zmsg_send (&msg1, client);
            }
            zstr_free (&command);
            zmsg_destroy (&msg);
        }
        else if (which == client){

            zsys_debug ("recv on client");
            zmsg_t *client_response = zmsg_recv (client);
            zmsg_print (client_response);
            zmsg_destroy (&client_response);
        }
    }
    
    zpoller_remove(poller, client);
    zsock_destroy (&client);
    zpoller_destroy (&poller);
    zstr_free (&name);
}

void handle_error(zsock_t *server, zframe_t *routing_id, const char* err_message){
    zmsg_t *response = zmsg_new ();
    zmsg_add (response, routing_id);
    zmsg_addstr (response, "ERROR");
    zmsg_addstr (response, err_message);
    zmsg_send (&response, server);
}

int main () {

    zactor_t *server = zactor_new (s_actor1, "actor1");
    zactor_t *client = zactor_new(s_client_actor, "client");
    zstr_sendx(client, "START", NULL);
    zstr_sendx(client, "ERR1", NULL);
    zstr_sendx(client, "ERR2", NULL);

    zclock_sleep (250);
    zactor_destroy (&client);
    zactor_destroy (&server);

}
