#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    FILE* file;
    file=fopen("msg.txt","w");
    for(int i=0;i<256;i++)
        fprintf(file,"a");
    fprintf(file,"ls");
    fclose(file);
    return 0;
}