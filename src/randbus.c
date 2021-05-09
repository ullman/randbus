/*
Copyright (C) 2021  Henrik Ullman
License: GPL Version 3
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <prom.h>
#include <promhttp.h>
#include <microhttpd.h>
#include "prometheus.h"
#include "parse.h"

volatile int run_loop = 1;

void print_manual()
{
  //TODO
}

void print_version ()
{
  //TODO
}

void catch_exit()
{
  run_loop = 0;

}

int main (int argc, char *argv[])
{
  char *device;
  int serial_fd;
  FILE *serial_f;
  int c;
  int p_size;
  char *p_array;
  char *cont_array;
  int crc_ok;
  unsigned short port=9009;

  device = NULL;

   while ((c = getopt (argc, argv, "i:p")) != -1)
    {
      switch (c)
        {
        case 'i':
          device = optarg;
          printf ("Reading input from serial device: %s\n", device);
          break;
        case 'p':
          port = strtoul (optarg,NULL,0);
          break;
        default:
          abort ();
        }
    }

  if(!device)
    {
      printf ("No serial device selected\n");
      exit(0);
    }
  printf ("press Ctrl-C to quit\n");
  signal (SIGINT, catch_exit);

  serial_fd = open_serial(device);
  if (serial_fd == 1)
    {
      exit(0);
    }

  serial_f = fdopen (serial_fd, "r");
  //init prometheus
  init_prometheus();
  init_metric_prometheus();
  promhttp_set_active_collector_registry (NULL);
  struct MHD_Daemon *daemon = promhttp_start_daemon (MHD_USE_SELECT_INTERNALLY, port, NULL, NULL);
  if(daemon == NULL)
    {
      fprintf(stderr, "Error initializing prometheus exporter\n");
      exit(1);
    }

  while(1)
    {
      p_size = watch_package (serial_f);
      p_array = malloc(p_size*sizeof(char));
      get_packet(serial_f, p_size, p_array);
      crc_ok = verify_packet(p_array);
      if (crc_ok == 0){
        cont_array = &p_array[25];
        parse_packet(cont_array);
      }

      free(p_array);
      if (run_loop == 0){
        break;
      }

    }

}
