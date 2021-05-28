#!/bin/bash


echo "[0] compiling server"
g++ ../src/server.cpp -o server -lpthread

echo "[1] compiling client get"
g++ ../src/get.cpp -o get

echo "[2] compiling client put"
g++ ../src/put.cpp -o put

echo "[3] inicialize server"
./server 
