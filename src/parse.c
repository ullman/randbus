#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <zlib.h>
#include "parse.h"
#include "prometheus.h"


int open_serial(char *device)
{
  struct termios serial_config;
  int serial_fd;

  //Serial config
  memset (&serial_config, 0 , sizeof(serial_config));
  cfsetispeed(&serial_config, B115200);

  serial_config.c_lflag = ICANON;

  serial_config.c_oflag &= ~OPOST;
  serial_config.c_cflag &= ~PARENB;
  serial_config.c_cflag &= ~CSIZE;
  serial_config.c_cflag |= CS8 | CREAD | CLOCAL;
  serial_config.c_iflag &= ~(IXON | IXOFF | IXANY);
  serial_config.c_iflag |= IGNCR;

  //open serial
  serial_fd = open (device, O_RDONLY);
  if (serial_fd == -1){
    perror("Error opening serial");
    return 1;
  }
  tcsetattr (serial_fd, TCSANOW, &serial_config);
  tcflush (serial_fd, TCIOFLUSH);

  return serial_fd;
}


int watch_package(FILE *serial_fd)
{
  int c;
  int i;
  char line_buf[1025];
  int n=0;
  char package_size[5];
  while((c = getc(serial_fd)) != EOF)
    {
      line_buf[n] = c;
      if(!strncmp(line_buf, "[R^][SIZE]",10))
        {
          memset(line_buf, 0, 1025);
          c = getc(serial_fd);
          if (c == '['){
            for (i = 0; i < 6; i++)
              {
                c = getc(serial_fd);
                if (c ==']')
                  {
                    break;
                  }
                package_size[i] = c;

              }
          }

          break;
        }
      if (c == '\n')
        {
          n=0;
        }
      else n++;
    }
  return atoi(package_size);
}

int get_packet(FILE* serial_fd, int packet_length, char *packet)
{
  int i;
  int c;
  for(i=0; i < packet_length+1; i++)
    {
      c = getc(serial_fd);
      packet[i] = c;
    }
  packet[i] = '\0';

  //
  return 1;

}


int verify_packet(char *packet)
{
  long packet_crc;
  long calc_crc;
  packet_crc = 0;
  calc_crc = 1;
  if (strncmp(packet, "[R^][CRC32]",11))
    {
      packet_crc = atol(&packet[13]);
      calc_crc = crc32(0, (const void*)&packet[25] ,strlen(&packet[25]));
    }
  if (packet_crc == calc_crc)
    {
      return 0;
    }
  else {
    printf("Checksum failed\n");
  }
return 1;
}


int parse_packet(char *packet)
{
  char sensor[100];
  char value[100];
  int s;
  int v;
  int l;
  int p_status;
  s=0;
  l=5;
  while(l < strlen(packet)) //for each log line
    {
      while(packet[l]!=']')
        {
          sensor[s] = packet[l];
          l++;
          s++;
        }
      sensor[s]='\0';
      l+=2;
      while(packet[l]!=']')
        {
          value[v] = packet[l];
          l++;
          v++;
        }
      value[v]='\0';

      p_status = log_prometheus (sensor, atof(value));
      if(p_status){
        fprintf(stderr, "error exporting variable to prometheus\n");
      }

      //reset
      l+=7;
      s=0;
      v=0;
      memset(sensor,0,100);
      memset(value,0,100);
    }
  return 0;
}
