Protocol for file transfer:
 ----------                           ----------
|  Server  | _______________________ |  Client  |
 ----------                           ----------
 
 Purpose: Transfere a file from the client to the server.
 
 Server               Client
    |                    |
    |   HELLO,file_name  |     First message from the client to the server containe 2 frames,
    | <----------------- |     One with the string "HELLO" and one with the name of the file.
    |                    |
    |   READY / ERROR    |     If the server is ready, the server send one message with one frame
    | -----------------> |     with the string "READY" otherwise the string "ERROR" and close the
    |                    |     connection.
    |                    |
    
  Problems:
  1) Try to communicate using czmq pull & push functions
  2) Try to use czmq functions router (for server) and dealer (for client) on your local machine using the protocol
  3) Try to implement by teams one server and one client using the protocol
  4) Try to implement a local server&client using czmq actor model and the protocol
