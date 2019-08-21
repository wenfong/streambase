# streambase

This is a simple server/client communication program using windows named pipes. Some simple functionalities are implemented to demonstrate server and client communication.

## Setup
To start, run the server first and then the client. 

## Description
At start, the client prompts the user to choose either an aynchronous connection or a synchonous connection.

Asynchronous client connection
```
The asynchronous client connection starts a thread to communicate with the server.
Some functionalities can be added on the main thread while it waits for the server with the help of mutexes or semaphores.
```

Synchronous client connection
```
Communication with the server just runs in the main loop.
The client must wait for the server to respond before continuing.
```

The user has the option to type three commands:
```
balance - typing "balance" allows the user to check their balance
add (x) - typing "add" allows the user to add (x) to their balance
end - typing "end" ends the program (both client and server)
````
For simplicity, these commands are taken under the assumption that the user enters all commands perfectly. 
Typing anything else will result in the server echoing back the message with a 5 second delay for asynchronous and synchronous tests.
