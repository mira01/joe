#include <czmq.h>

static void makeReady(
    zmsg_t **msg_,
    zframe_t* id_) {
  *msg_ = zmsg_new();
  zmsg_add(*msg_, id_);
  zmsg_addstr(*msg_, "READY");
}

static void makeError(
    zmsg_t **msg_,
    zframe_t* id_) {
  *msg_ = zmsg_new();
  zmsg_add(*msg_, id_);
  zmsg_addstr(*msg_, "ERROR");
}

int main () {

    zsys_init();

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
    zframe_t* routing_id_ = zmsg_pop(msg2);
    char *command_ = zmsg_popstr(msg2);
    zsys_info(command_);
    zmsg_t *response_;
    if(strcmp(command_, "HELLO") == 0) {
      makeReady(&response_, routing_id_);
    }
    else
      makeError(&response_, routing_id_);
    zmsg_send(&response_, server);
    zstr_free(command_);

    zmsg_t *msg3_ = zmsg_recv(client);
    zmsg_print(msg3_);
    char *id2_ = zmsg_popstr(msg3_);
    zstr_free(&id2_);
    char *code_ = zmsg_popstr(msg3_);
    zsys_info(code_);
    zstr_free(&code_);

    zstr_free (&command_);
    zmsg_destroy (&msg2);

    zsock_destroy (&server);
    zsock_destroy (&client);

}
