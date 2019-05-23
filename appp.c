#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

int fff(int a){
     char buf[256] = {0};
  char command[] = "uptime | sed 's/.*up \\([^,]*\\), .*/\\1/'";
  scanf("%s", buf);
    
  char greeting_text[128];


  strcpy(greeting_text, "Hello, dear ");
  strcat(greeting_text, buf);

  printf("%s\n", greeting_text);
  printf("This computer has been running for ");
  fflush(stdout);
  //printf("%s\n", command);
  system(command);
  fflush(stdout);


}

int main(int argc, char** argv)
{
    int b=8;
    fff(b);

  return 0;
}