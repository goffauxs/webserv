# webserv
42 Project - Webserv

## About
This project is a NGINX-like HTTP Web server. 

### How it works
It is built upon [Berkeley Sockets](https://en.wikipedia.org/wiki/Berkeley_sockets) using a `socket(), bind(), listen()` workflow to wait for new client connections, `accept()` to establish a new socket with each client, and `select()` to suspend while waiting for the client sockets to be ready to be read from or written to.
We then use the result of `select()` to `recv()` (receive) a request from the client or `send()` a response to the client.

### Configuration
The configuration of the server is done using a `.conf` file as a parameter to the program. This configuration file is very similar in format to that of NGINX, using the same or similar directives. An example configuration can be found [here](https://github.com/goffauxs/webserv/blob/main/default.conf), containing at least one example of each possible directive.

## Installation
Compiled using `make all`
