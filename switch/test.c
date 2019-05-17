#include<stdio.h>
#include <time.h>
#include<stdlib.h>


char* random_mac_adress(){
    int i, tp;
    int j=0;
    srand(time(NULL) + getpid());
    char* mac=malloc(20);
    for (i = 0; i < 6; i++) {
        tp = rand() % 256;
        j=j+sprintf(mac+j,"%s%X%s", tp < 16 ? "0" : "", tp, i < 5 ? ":" : "\n");
    }
    printf("%s\n", mac);
    return mac;
}
int main(){
    char* s =random_mac_adress();

    return 0;
}