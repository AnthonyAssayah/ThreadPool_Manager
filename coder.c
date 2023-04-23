
#include "codec.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int key = 0;
    int enc = 0;
    int dec = 0;
    char c;
    for (int i = 1; i < argc; i++){

        if (strcmp(argv[i], "-e") == 0){
            enc = 1;
        }
        else if (strcmp(argv[i], "-d") == 0){
            dec = 1;
        }
        else if (i == 1){
            key = atoi(argv[1]);
        }
        else if (strcmp(argv[i], "-h") == 0){
            printf("coder key -d/-e < input_file > output_file\n");
            return 0;
        }
        else{
            printf("Invalid input\n");
            printf("coder key -d/-e < input_file > output_file\n");
            return 1;
        }
    }
    if (enc == 1 && dec == 1)
    {
        printf("You can't encrypt and decrypt at the same time\n");
        return 1;
    }   
    if (enc == 0 && dec == 0)
    {
        printf("You must choose to encrypt or decrypt\n");
        return 1;
    }

#define MAX_SIZE 1000000

    int cnt = 0;
    char data[MAX_SIZE];
    memset(data, 0, MAX_SIZE);

    while ((c = getchar()) != EOF)
    {
        data[cnt] = c;
        cnt++;
    }



    if (enc == 1)
    {
        data[cnt] = '\0';
        encrypt(data, key);
    }
    else
    {
        data[cnt] = '\0';
        decrypt(data, key);
    }

    


    printf("%s", data);
    return 0;
}