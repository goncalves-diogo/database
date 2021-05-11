#!/bin/bash


echo "[0] compiling server"
g++ ../src/server.cpp -o server -lpthread

echo "[1] compiling server"
g++ ../src/client.cpp -o client

echo "[2] inicialize server on port 5000"
./server 5000
