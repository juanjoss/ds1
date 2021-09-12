# distributed-systems
Assignments for distributed systems I. Systems Engineering at UNLPAM.

# TP 1: Sockets Programming
- Ej 1: Simple echo server. Client sends content and server handles de request answering with the same content to the client.
- Ej 2: Client/Server File System with cache.
    > client: ./myFS "server address" "server port" "file to retreive" "local storage path"

    > server: 
    - 512MB cache using std::map C++ class.
    - Once the server receives a request, it checks if the requested file is cached, if so, it will return the file's content from the cache. If the file is not cached and the file's size is bigger than 512MB the server reads the file's content from the ./files directory and sends the content back to the client. If the file's size is less than 512MB, the server will remove elements from the cache until there's enough space to insert the requested file into the cache, and then sends back the content to the client.