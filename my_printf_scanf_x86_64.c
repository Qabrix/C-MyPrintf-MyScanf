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
    int *rdxTemp; 
    asm ("mov %%rdx, %0\n\t" : "=r" (rdxTemp) );
    int *args[64];
    int i = 0, j = 0;
    while(input[i]!='\0'){
        if(input[i]=='%' && (input[i+1]=='d' || input[i+1]=='s' || input[i+1]=='b' || input[i+1]=='x')){
            if(j == 0)
                asm ("mov %%rsi, %0\n\t" : "=r" (args[j]) );
            else if(j == 1)
                args[j] = rdxTemp;
            else if(j == 2)
                asm ("mov %%rcx, %0\n\t" : "=r" (args[j]) );
            else if(j == 3)
                asm ("mov %%r8, %0\n\t" : "=r" (args[j]) );
            else if(j == 4)
                asm ("mov %%r9, %0\n\t" : "=r" (args[j]) );
            else
                asm ("mov %%rbp, %0\n"
                        "add %1, %0\n"
                        "mov (%0), %0\n"
                        : "=&r" (args[j])
                        : "r" ((long)(16+(j-5)*8))
                        );
            
            j++;
        }
        i++;
    }

    i=j=0;

    char *buffer = malloc(32*sizeof(char*));
    int size = read(0, buffer, 32);

    while(input[i]!='\0'){
        if(input[i]=='%'){
            if(input[i+1]=='d'){
                int *d = args[j];
                buffer += saveInt(buffer, d, size);  
                j++;
            }
            else if(input[i+1]=='s'){
                char *s = args[j];
                buffer += saveString(buffer, s, size);
                j++;
            }
            else if(input[i+1]=='b'){
                int *b =  args[j];
                buffer += saveBin(buffer, b, size);
                j++;
            }
            else if(input[i+1]=='x'){
                int *x = args[j];
                buffer += saveHex(buffer, x, size);  
                j++;
            }
            i++;
        }
        i++;
    }
}

void myprintf(char *output, ...){
    long rdxTemp;
    asm ("mov %%rdx, %0\n\t" : "=r" (rdxTemp) );
    long args[64];
    int i = 0, j = 0;
    while(output[i]!='\0'){
        if(output[i]=='%' && (output[i+1]=='d' || output[i+1]=='s' || output[i+1]=='b' || output[i+1]=='x')){
            if(j == 0)
                asm ("mov %%rsi, %0\n\t" : "=r" (args[j]) );
            else if(j == 1)
                args[j] = rdxTemp;
            else if(j == 2)
                asm ("mov %%rcx, %0\n\t" : "=r" (args[j]) );
            else if(j == 3)
                asm ("mov %%r8, %0\n\t" : "=r" (args[j]) );
            else if(j == 4)
                asm ("mov %%r9, %0\n\t" : "=r" (args[j]) );
            else
                asm ("mov %%rbp, %0\n"
                        "add %1, %0\n"
                        "mov (%0), %0\n"
                        : "=&r" (args[j])
                        : "r" ((long)(16+(j-5)*8))
                        );
            
            j++;
        }
        i++;
    }

    i=j=0;
    
    while(output[i]!='\0'){
        if(output[i]=='%'){
            if(output[i+1]=='d'){
                int d = args[j];
                showInt(d, 10);
                j++;
            }
            else if(output[i+1]=='s'){
                char *s = args[j];
                showString(s);
                j++;
            }
            else if(output[i+1]=='b'){
                int d = args[j];
                showInt(d, 2);
                j++;
            }
            else if(output[i+1]=='x'){
                int d = args[j];
                showInt(d, 16);
                j++;
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
    myprintf("%b | %x | %s | %d | %d | %d | %d | %b | %x | %s | %d \n", 10, 161, "test", 19, 21, 22, 23, b, x, s, d);

    return 0;
}
