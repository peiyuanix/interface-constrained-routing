#include <stdbool.h>
#include <assert.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define ERROR_PRINT_THEN_EXIT(msg...) \
  fprintf(stderr, ##msg);       \
  exit(1);


void unbound_socket();
void address_bound_socket();
void interface_bound_socket();


int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <function>\n", argv[0]);
    fprintf(stderr, "Available functions: unbound, address_bound, interface_bound\n");
    return 1;
  }

  char *function_name = argv[1];

  if (strcmp(function_name, "unbound") == 0) {
    unbound_socket();
  } else if (strcmp(function_name, "address_bound") == 0) {
    address_bound_socket();
  } else if (strcmp(function_name, "interface_bound") == 0) {
    interface_bound_socket();
  } else {
    fprintf(stderr, "Invalid function name: %s\n", function_name);
    fprintf(stderr, "Usage: %s <function>\n", argv[0]);
    fprintf(stderr, "Available functions: unbound, address_bound, interface_bound\n");
    return 1;
  }

  return 0;
}

void unbound_socket() {
  struct sockaddr_in dest_in;
  memset(&dest_in, 0, sizeof(dest_in));
  dest_in.sin_family = AF_INET;
  dest_in.sin_port = htons(53);
  if (inet_pton(AF_INET, "114.114.114.114", &dest_in.sin_addr) != 1)
  {
    ERROR_PRINT_THEN_EXIT("fail to inet_pton: %s\n", strerror(errno));
  }

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    ERROR_PRINT_THEN_EXIT("fail to socket: %s\n", strerror(errno));
  }

  int ret = connect(sock, (struct sockaddr *)&dest_in, sizeof(dest_in));
  if (ret != 0) {
    ERROR_PRINT_THEN_EXIT("fail to connect: %s\n", strerror(errno));
  }

  sleep(5);
}

void address_bound_socket() {
  struct sockaddr_in src_in;
  memset(&src_in, 0, sizeof(src_in));
  src_in.sin_family = AF_INET;
  if (inet_pton(AF_INET, "192.168.1.236", &src_in.sin_addr) != 1)
  {
    ERROR_PRINT_THEN_EXIT("fail to inet_pton: %s\n", strerror(errno));
  }

  struct sockaddr_in dest_in;
  memset(&dest_in, 0, sizeof(dest_in));
  dest_in.sin_family = AF_INET;
  dest_in.sin_port = htons(53);
  if (inet_pton(AF_INET, "114.114.114.114", &dest_in.sin_addr) != 1)
  {
    ERROR_PRINT_THEN_EXIT("fail to inet_pton: %s\n", strerror(errno));
  }

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    ERROR_PRINT_THEN_EXIT("fail to socket: %s\n", strerror(errno));
  }

  int ret = bind(sock, (struct sockaddr *)&src_in, sizeof(src_in));
  if (ret != 0) {
    ERROR_PRINT_THEN_EXIT("fail to bind: %s\n", strerror(errno));
  }

  ret = connect(sock, (struct sockaddr *)&dest_in, sizeof(dest_in));
  if (ret != 0) {
    ERROR_PRINT_THEN_EXIT("fail to connect: %s\n", strerror(errno));
  }

  sleep(5);
}

void interface_bound_socket() {
  struct sockaddr_in src_in;
  memset(&src_in, 0, sizeof(src_in));
  src_in.sin_family = AF_INET;
  if (inet_pton(AF_INET, "192.168.1.236", &src_in.sin_addr) != 1)
  {
    ERROR_PRINT_THEN_EXIT("fail to inet_pton: %s\n", strerror(errno));
  }

  struct sockaddr_in dest_in;
  memset(&dest_in, 0, sizeof(dest_in));
  dest_in.sin_family = AF_INET;
  dest_in.sin_port = htons(53);
  if (inet_pton(AF_INET, "114.114.114.114", &dest_in.sin_addr) != 1)
  {
    ERROR_PRINT_THEN_EXIT("fail to inet_pton: %s\n", strerror(errno));
  }

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    ERROR_PRINT_THEN_EXIT("fail to socket: %s\n", strerror(errno));
  }

  const char *ifname = "ens33";
  int ret = setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, ifname, strlen(ifname));
  if (ret == -1) {
      ERROR_PRINT_THEN_EXIT("fail to setsockopt: %s\n", strerror(errno));
  }

  ret = connect(sock, (struct sockaddr *)&dest_in, sizeof(dest_in));
  if (ret != 0) {
    ERROR_PRINT_THEN_EXIT("fail to connect: %s\n", strerror(errno));
  }

  sleep(5);
}