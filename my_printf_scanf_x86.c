#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <math.h>

int saveInt(char *buffer, int *d, int size){
    int minusCorrector = 0;
    if(buffer[0] == '-')
        minusCorrector++;

    *d=0;
    for(int i=0+minusCorrector; i<size-1; i++){
        if(!(buffer[i] >= '0' && buffer[i] <= '9')){
            if(minusCorrector)
                *d*=-1;

            return i+1;
        }
       *d*=10;
           
       *d+=buffer[i]-'0';
    }
    if(minusCorrector)
        *d*=-1;
    
    return 0;
}

int saveString(char *buffer, char *s, int size){
    for(int i=0; i<size; i++){
        if(buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\0'){
            s[i] = '\0';
            return i+1;
        }
            

        s[i]=buffer[i];
    }

    return 0;
}

int saveBin(char *buffer, int *d, int size){
    int minusCorrector = 0;
    if(buffer[0] == '-')
        minusCorrector++;

    *d=0;
    for(int i=0+minusCorrector; i<size-1; i++){
        if(buffer[i]!='0' && buffer[i]!='1'){
            if(minusCorrector)
                *d*=-1;
            return i+1;
        }
        *d*=2;

        *d+=buffer[i]-'0';  
    }
    if(minusCorrector)
        *d*=-1;

    return 0;
}

int saveHex(char *buffer, int *d, int size){
    int minusCorrector = 0;
    if(buffer[0] == '-')
        minusCorrector++;

    *d=0;
    for(int i=0+minusCorrector; i<size-1; i++){
        if(buffer[i]>='0' && buffer[i]<='9'){
            *d+=buffer[i]-'0';
        }
        else if(buffer[i]>='A' && buffer[i]<='F'){
            *d+=buffer[i]-55;
        }
        else if(buffer[i]>='a' && buffer[i]<='f'){
            *d+=buffer[i]-87;
        }else{
            if(minusCorrector)
                *d*=-1;
            *d/=16;
            return i+1;
        }
        *d*=16;
    }
    if(minusCorrector)
        *d*=-1;

    return 0;
}

int getIntSize(int d, int sys){
 int size=0;
 while(d>0){
     size++;
     d=d/sys;
 }
 return size;
}

int getPow(int sys, int size){
    int pow=1;
    for(int i=0; i<size-1; i++){
        pow*=sys;
    }
    return pow;
}

void showInt(int d, int sys){
    char buffer[32];
    int minusCorrector = 0;
    if(d<0){
         buffer[0]='-';
         minusCorrector++;
         d*=-1;
    }
    if(d>0){
        int size = getIntSize(d, sys);
        int pow = getPow(sys,size);
        for(int i=0+minusCorrector; i<size+minusCorrector; i++){
        if(sys==16 && d/pow>9){
            buffer[i]=d/pow + 55;
        }
        else{
            buffer[i]=d/pow + '0';
        }    
            d=d-(d/pow)*pow;
            pow=pow/sys;
        }
        write(1, &buffer[0], size+minusCorrector);
    }
    else if(d==0){
        buffer[0]='0';
        write(1, &buffer[0], 1);
    }
}

void showString(char *var){
    int i=0;
    while(var[i]!='\0'){
        write(1, &var[i], 1);
        i++;
    }
}

int getOutputSize(const char *out){
    int i=0;
    while(out[i]!='\0')
        i++;
    return i;
}

void myscanf(const char *input, ...){
    char *p = (char*)&input;
    p+=sizeof(char*);

    char *buffer = malloc(32*sizeof(char*));
    int size = read(0, buffer, 32);
    int i=0;
    while(input[i]!='\0'){
        if(input[i]=='%'){
            if(input[i+1]=='d'){
                int *d = (int*)*((int*)p);
                buffer += saveInt(buffer, d, size);  
                p+=sizeof(int*);
            }
            else if(input[i+1]=='s'){
                char *s = (char*)*((char**)p);
                buffer += saveString(buffer, s, size);
                p+=sizeof(char**);
            }
            else if(input[i+1]=='b'){
                int *b = (int*)*((int*)p);
                buffer += saveBin(buffer, b, size);
                p+=sizeof(int*);
            }
            else if(input[i+1]=='x'){
                int *x = (int*)*((int*)p);
                buffer += saveHex(buffer, x, size);  
                p+=sizeof(int*);
            }
            i++;
        }
        i++;
    }
}

void myprintf(char *output, ...){
    char *p = (char*)&output;
    p+=sizeof(char*);
    int i=0;
    while(output[i]!='\0'){
        if(output[i]=='%'){
            if(output[i+1]=='d'){
                int d = *((int*)p);
                p+=sizeof(int);
                showInt(d, 10);
            }
            else if(output[i+1]=='s'){
                char *s = *((char**)p);
                p+=sizeof(char**);
                showString(s);
            }
            else if(output[i+1]=='b'){
                int d = *((int*)p);
                p+=sizeof(int);
                showInt(d, 2);
            }
            else if(output[i+1]=='x'){
                int d = *((int*)p);
                p+=sizeof(int);
                showInt(d, 16);
            }
            i++;
        }
        else{
            write(1, &output[i], 1);
        }
        i++;
    }
}

int main(){
    int b, x, d;
    char s[32];
    myscanf("%b %s %x %d", &b, s, &x, &d);
    myprintf("%b | %x | %s | %d \n", b, x, s, d);
    return 0;
}
