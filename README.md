# http-webserver
webserver that parses http requests

1. Problem statement
To build a multi-threaded web-server which interacts with a web browser, han- dles http requests and serves content and web pages from a file system.

2. files

Makefile - to be run directly using make, compiles the code and generates all the required output files and executables.

The code files - contain all the codes corresponding to the webserver. Including programs to create socket, bind socket, listen for and accept connections, and the http parser and handler

File system for website - contains all the content making up the web pages : html, images, javascript, organized according to the requirements of the web pages.


3. steps to run and test the webserver
-unzip the folder, cd src and run make
-run the server as ./multi_threaded_server <port no> - this will start the server on the given port number
-from any browser,typelocalhost:<portno>orlocalhost:<portno>/files/main.html
-It should open the main page in the browser, and from there, upon clicking the links, the website should navigate to corresponding pages.
-In this website, there is one link to a second webpage, and the second webpage has image links to three more pages, and all of them have links to go back to the homepage. Also, there are a couple of links on each page that serve pages from outside the website.
-If an inexistent file name is entered, eg. localhost:<portno>/somefile, it returns an error page, saying file not found
theservercanbeconnectedtosimultaneouslyfrommorethanonebrowsers at a time.
-to stop the server, close the terminal or terminate the program using ctrl+c.




4. working of the code

main.c - In the main program, we are taking the port number passed from the command line, and calling get_listen_socket function with this port number as an argument. this function returns a socket descriptor, s. then we call service_listen_socket with the socket descriptor s.

get_listen_socket.c - In this function, we create a socket to use for listening. we need to bind it our ipv6 address. As we need a service that resides at a particular port, but which does not care what local address is chosen, we can bind it to its port and leave the local address unspecified (set to in6addr_any, a variable with a constant value in <netinet/in.h>). and we specify the port number to the one passed to this function from main. We then bind the socket to this address structure, start listening on the socket, and return the socket descriptor.

service_listen_socket_multithread.c-Callsmake_printable_address with the addresses calls service_listen_socket with in a multithreaded fashion. In the service_listen_socket program, sockets are accepted in the listening state, waiting for connections to arrive. When a new con- nection arrives, it returns them and updates the address structure with the address of the caller.

service_client_socket.c - This is the part of the code where the web- server and http parser is written.
• A list of accepted extensions is declared
• The incoming requests on the socktet are read, and broken down. HTTP request structure is <method> <requested file> <http ver- sion> and other details on the next line.
The first line of the request is taken and parsed further to break down and get the individual fields of the request.
If the file location part of the request is null, we return the homepage.
• If the file does not exist in the server file structure, the response is generated with code 404.
• if the file is found, it is opened, read and the contents are sent back to the browser and displayed, and a response with code 200 is sent.
• This keeps repeating with all the incoming requests. When the first Html page is being opened by the browser, it also sends requests for the images, css files, bootstrap files and js files and they are returned to the browser.
• If the links on the homepage are clicked, those requests are sent to the webserver again and the same process continues.
As part of the file system that makes up the web server, there is an index page - main.html which has a link to the second web page - new.html which has image links to three other web pages. Images, bootstrap files,
javascript files are also present in the file system and are served as and
when required, and sending the requests for these files are handled by the
html files and responses are handled by the webserver in the service_client_socket.c program.
