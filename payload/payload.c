#include <stdio.h> 

int main(int argc, char *argv[]){ 

    FILE *fp;
    char buff[255];

    fp = fopen("test.txt", "w");
    fputs("10", fp);
    fclose(fp);


    return 0;
} 
