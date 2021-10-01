# distributed-systems
Assignments for distributed systems I. Systems Engineering at UNLPAM.

# TP 1: Sockets Programming
- Ej 1: Simple echo server. Client sends content and server handles the request answering with the same content to the client.
- Ej 2: Client/Server File System with cache.
    > client: ./myFS "server address" "server port" "file to retreive" "local storage path"

    > server: 
    - 512MB cache using std::map C++ class.
    - Once the server receives a request, it checks if the requested file is cached, if so, it will return the file's content from the cache. If the file is not cached and the file's size is bigger than 512MB the server reads the file's content from the ./files directory and sends the content back to the client. If the file's size is less than 512MB, the server will remove elements from the cache until there's enough space to insert the requested file into the cache, and then sends back the content to the client.

# TP 2: Process Communication - RPC and rpgen
- Ej 1: A RPC service calculator. The client makes a request for an operation and the server answers with the processed calculation. The operations are sum, subtract, multiply, divide and compare equals for two given numbers.
    > client: ./client "host" "operand 1" "operation " "operand 2"
        
        operation = [ + | - | x | / | = ]
        operand1, operand2 = number
- Ej 2: A string functions service. The client connects to a local server and the server process one of these three functions for a given string: 
    - length including spaces
    - length excluding spaces
    - number of words

    > client: ./client "function" "string"
        
        function = [ 
            LwS = Length with Spaces | 
            LWS = Length Without Spaces | 
            NW = Number of Words
        ]