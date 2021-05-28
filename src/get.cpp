#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <vector>
#include <chrono>
using namespace std::chrono;
using namespace std;

int main(int argc, char *argv[]) {
  int listenFd, portNo;
  bool loop = false;
  struct sockaddr_in svrAdd;
  struct hostent *server;

  if (argc < 2) {
    cerr << "Syntax : ./get <key>" << endl;
    return 0;
  }

  portNo = 5000;

  // create client skt
  listenFd = socket(AF_INET, SOCK_STREAM, 0);

  // Server configuration
  if (listenFd < 0) {
    cerr << "Cannot open socket" << endl;
    return 0;
  }
  server = gethostbyname("localhost");
  if (server == NULL) {
    cerr << "Host does not exist" << endl;
    return 0;
  }
  bzero((char *)&svrAdd, sizeof(svrAdd));
  svrAdd.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&svrAdd.sin_addr.s_addr,
        server->h_length);
  svrAdd.sin_port = htons(portNo);
  int checker = connect(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd));
  if (checker < 0) {
    cerr << "Cannot connect!" << endl;
    return 0;
  }

  char op[] = "GET";
  char test[1036];
  char s[11];

  bzero(test, 1036);
  bzero(s, 11);

  strcpy(s, op);
  strcat(s, ",");
  strcat(s, argv[1]);

  auto start = high_resolution_clock::now();

  write(listenFd, s, strlen(s));
  read(listenFd, test, 1036);

  cout << duration_cast<microseconds>(high_resolution_clock::now() - start ).count() << endl;

  cout << "Message:" << test << endl;

  close(listenFd);
}
