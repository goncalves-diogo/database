#!/bin/bash


echo "[0] compiling server"
g++ ../src/server.cpp -o server -lpthread -std=c++17

echo "[1] compiling client get"
g++ ../src/get.cpp -o get -std=c++17

echo "[2] compiling client put"
g++ ../src/put.cpp -o put -std=c++17

#echo "[3] creating database file"
#touch database

echo "[4] inicialize server"
./server &

#echo "[5] begin test"
#for (( i = 0; i < 1024; i++ )); do
   #./put 1 abc 
   #./get 1
#done
