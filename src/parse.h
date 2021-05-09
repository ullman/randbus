#ifndef PARSE_H
#define PARSE_H

typedef struct packet_struct {
  int a;
} packet_struct;

int open_serial(char *device);

int watch_package(FILE *serial_fd);

int get_packet(FILE *serial_fd, int packet_length, char *packet);

int verify_packet(char *packet);

int parse_packet(char *packet);

#endif /* PARSE_H */
