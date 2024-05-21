# THE SERVER
## Overview
This repository contains a messaging server socket implementation in C. It allows for bidirectional communication between a server and multiple clients over a network.

## FEATURES
### PRIMARY FUNCTIONALITY
The server should do the following Basics<br>
* [x] Listen for connections.
* [x] Accept connections.
* [x] Store a list of connected clients.
* [x] Receive messages and send to clients as per their usernames.
* [ ] If a client sends 'EXIT'|| 'QUIT' it terminates the connections.
* [ ] Add some terminal GUI.

### SECONDARY FUNCTIONALITY
The following are additional features to be added.<br>
* [ ]  Add a database to store sign up information.
* [ ] Add sign up feature.
* [ ] Create a file to store conversations between two people or group chats.

## REQUIREMENTS
The following outline the configurations I employed, which proved effective in my usage and testing.<br> 
However, it's important to note that **these are not the only possible approaches** if I havent included it in the 
requirements list it is simply because I haven't tested it there.
1. **Operating system**  - Ubuntu/Pop Os (A Linux distro).
2. The compiler - GCC.

## BUILD 
Clone the repository then you can compile it using the following make commands.<br>
``NOTE:`` If you are using a different compiler like Clang make sure to edit the makefile to fit your needs.
1. `make APP` - Compiles the program.
2. `make RUN` - To run the program.

