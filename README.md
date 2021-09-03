# S2DCS
 A lightweight distributed compute server-client application.



# Quick Overview
There is a server setup that accepts only three clients

These three clients are given a binary executable sent from the server

The three clients each compile and execute the same binary file and produce a textfile

The clients then read the textfile and send out the value from the textfile back to the server

The server recieves the values and sums them up which concludes the program



# About the folders...
/c1 /c2 /c3 all have the same files and a downloads folder to reperesent a real life scenario where a user has downloaded my client to do some calculations at home

/payload is the executable file and code of a very simple calculation that you are able to peek at

/serverfolder contains the c file of the server and contains the payload so that it may distribute it from a server to the respective clients



# How To Test This Out
     1. Use the make file to compile and ready everything up

     2. open up respective terminals in each of the client folders and server folder.
     
     3. execute the server first --> ./server <IP_ADDRESS> <PORT> e.g. ./server 127.0.0.1 8080
     
     4. rinse and repeat for clients --> ./client <IP_ADDRESS> <PORT> e.g. ./client 127.0.0.1 8080
     
     5. client should immediately execute code, display the return result, and terminate e.g. 10
     
     6. server should recieve the value and start to sum up the returns from the multiple clients
     
     7. to terminate serverside simply ctrl + c
     
