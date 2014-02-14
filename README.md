telnet
======


to compile server:
g++ server.cpp -lpthread -o server

to compile client:
g++ client.cpp -o client


example of use:
===============

User A console
`piotrek@piotrek-desktop:~/Kodzenie/telnet/telnet$ g++ server.cpp -lpthread -o server && ./server`
`Here is the message: -host`
`add host OK: 16777343`
`Here is the message: 127.0.0.1`
`Here is the message: ls`

`Here is the message: mkdir directory`

`Here is the message: ls`

`Here is the message: `
`Here is the message: `

User B console
`piotrek@piotrek-desktop:~/Kodzenie/telnet/telnet$ ./client -host`
`Please write password`
`pass`
`Please write password again`
`pass`
`Now you are hosting`
`ls`
`mkdir directory`
`ls`

User C console
`piotrek@piotrek-desktop:~/Kodzenie/telnet/telnet$ ./client 127.0.0.1`
`ls`
`client`
`client.cpp`
`client.cpp~`
`README.md`
`server`
`server.cpp`
`server`
`mkdir directory`

`ls`
`client`
`client.cpp`
`client.cpp~`
`directory`
`README.md`
`server`
`server.cpp`
`server.cpp~`


