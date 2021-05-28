#include <cstring>
#include <fstream>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

using namespace std;

#define MAX_THREADS 64


char* getDatabase(long long int);
bool putDatabase(long long int, string);

void *task1(void *);

static int connFd;

int main(int argc, char *argv[]) {
  int pId, portNo, listenFd;
  socklen_t len;
  bool loop = false;
  struct sockaddr_in svrAdd, clntAdd;

  pthread_t threadA[64];
  portNo = 5000;

  // create socket
  listenFd = socket(AF_INET, SOCK_STREAM, 0);

  if (listenFd < 0) {
    cerr << "Cannot open socket" << endl;
    return 0;
  }

  bzero((char *)&svrAdd, sizeof(svrAdd));

  svrAdd.sin_family = AF_INET;
  svrAdd.sin_addr.s_addr = INADDR_ANY;
  svrAdd.sin_port = htons(portNo);

  // bind socket
  if (bind(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd)) < 0) {
    cerr << "Cannot bind" << endl;
    return 0;
  }

  listen(listenFd, 5);

  len = sizeof(clntAdd);

  int noThread = 0;

  while (true) {
    cout << "Listening" << endl;

    // this is where client connects. svr will hang in this mode until client
    connFd = accept(listenFd, (struct sockaddr *)&clntAdd, &len);

    if (connFd < 0) {
      cerr << "Cannot accept connection" << endl;
      return 0;
    } else
      cout << "Connection successful" << endl;

    pthread_create(&threadA[noThread], NULL, task1, NULL);
    noThread++;
  }

  for (int i = 0; i < noThread; i++) {
    pthread_join(threadA[i], NULL);
  }
}

void *task1(void *dummyPt) {
  cout << "Thread No: " << pthread_self() << endl;
  // PUT/GET (3 bytes)
  // key (8 bytes)
  // value (1024 bytes)
  char toGet[1035];
  bzero(toGet, 1036);
  read(connFd, toGet, 1035);

  string tester(toGet);

  vector<string> v;
  std::stringstream ss(tester);
  while (ss.good()) {
    string substr;
    getline(ss, substr, ',');
    v.push_back(substr);
  }
  string op = v[0];
  long long int key = stoll(v[1]);

  char toSend[1033];
  if (op == "PUT") {
    string value = v[2];
    bool status = putDatabase(key, value);
    bzero(toSend, 1033);
    if (status)
      strcpy(toSend, "OK");
    else
      strcpy(toSend, "ERRO");
    write(connFd, toSend, strlen(toSend));
  }
  if (op == "GET") {
    char *value = getDatabase(key);
    bzero(toSend, 1033);
    strcpy(toSend, value);
    write(connFd, toSend, strlen(toSend));
  }

  cout << "\nEnding job" << endl;
  close(connFd);
}

char* getDatabase(long long int key) {

    char* empty;
    strcpy(empty, ",,");
    return empty;
}
bool putDatabase(long long int key, string value) {
    return true;
}
