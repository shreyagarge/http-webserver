#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <memory.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <assert.h>

#include "service_client_socket.h"


#define buffer_size 9999


struct {
  char *ext;
  char *filetype;
} extensions [] = {
  {"gif", "image/gif" },  
  {"jpg", "image/jpeg"}, 
  {"jpeg","image/jpeg"},
  {"png", "image/png" },  
  {"zip", "image/zip" },  
  {"gz",  "image/gz"  },  
  {"tar", "image/tar" },  
  {"htm", "text/html" },  
  {"html","text/html" },  
  {"php", "image/php" },  
  {"cgi", "text/cgi"  },  
  {"asp","text/asp"   },  
  {"jsp", "image/jsp" },  
  {"xml", "text/xml"  },  
  {"js","text/js"     },
  {"css","test/css"   }, 
  {"map","css/map"},
  {0,0} };

int
service_client_socket (const int s, const char *const tag) {
  char buffer[buffer_size];
  //size_t bytes;
  long retrieved;
  int i;
  printf ("new connection from %s\n", tag);
  retrieved = read(s,buffer,buffer_size);
  if(retrieved<0){
    printf("failed to read the request\n\n");
  }
    /*printf("\n\n\n\n**************\n");
    puts(buffer);
    printf("**************\n\n\n");*/
  if(retrieved>0 && retrieved<buffer_size){
    buffer[retrieved]=0;
  }
  else buffer[0]=0;

  for(i=0;i<retrieved;i++){
    if(buffer[i]=='\r' || buffer[i]=='\n'){
      buffer[i]='*';
    }
  }


  if(strncmp(buffer,"GET ",4)&&strncmp(buffer,"get ",4)&&!strcmp(buffer,"")){
    printf("only GET operation supported! \n \n");
  }

  for(i=4;i<buffer_size;i++){
    if(buffer[i] == ' ') { 
      buffer[i] = 0;
      break;
    }
  }
  int j;
  for(j=0;j<i-1;j++)  
    if(buffer[j] == '.' && buffer[j+1] == '.')
      printf("Parent directory (..) path names not supported in the URL\n");

  if( !strncmp(&buffer[0],"GET /\0",6) || !strncmp(&buffer[0],"get /\0",6) ) 
    (void)strcpy(buffer,"GET /files/main.html");

  int buflen=strlen(buffer);
  char * path;
  path = (char *)0;

  for(i=0;extensions[i].ext != 0;i++) {
    int len = strlen(extensions[i].ext);
    if( !strncmp(&buffer[buflen-len], extensions[i].ext, len)) {
      path =extensions[i].filetype;
      break;
    }
  }

  if(path==0) printf("unsupported file extension type! \n");
  int fd;
  if((fd=open(&buffer[5],O_RDONLY))==-1){
    if(strcmp(&buffer[5],"")){
    printf("failed to open file %s \n",&buffer[5]);
    (void)write(s,"HTTP/1.1 404 not found\n", 23);  
    close(s);
  }
  }

  (void)sprintf(buffer,"HTTP/1.1 200 OK\r\nContent-Type: %s\r\n\r\n", path);
  /*printf("#########\n");
  puts(buffer);
  printf("#########\n\n");*/
  (void)write(s,buffer,strlen(buffer));
  //prints out everything from the files into the browser
  while ( (retrieved = read(fd, buffer, buffer_size)) > 0 ) {
    (void)write(s,buffer,retrieved);
  }

  /* bytes == 0: orderly close; bytes < 0: something went wrong 
  if (bytes != 0) {
    perror ("read");
    return -1;
  }*/
  //printf ("connection from %s closed\n", tag);
  close (s);
  return 0;
}

