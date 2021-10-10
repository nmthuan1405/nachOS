#include "syscall.h"

int main(){
    int n = ReadNum();
    int a[100];
    char ord; // ord = i for increasing order, ord = d for decreasing order
    int i, j, temp;
    for (i=0; i<n; i++){
        a[i] = ReadNum();
    }
    ord = ReadChar();
    if (ord == 'i'){
        for (i=0; i<n; i++)
            for (j=0; j<n-i-1; j++)
                if (a[j] > a[j+1]){
                    temp = a[j];
                    a[j] = a[j+1];
                    a[j+1] = temp;
                }
    }
    else if (ord == 'd'){
        for (i=0; i<n; i++)
            for (j=0; j<n-i-1; j++)
                if (a[j] < a[j+1]){
                    temp = a[j];
                    a[j] = a[j+1];
                    a[j+1] = temp;
                }
    }

    for (i=0; i<n; i++){
        PrintNum(a[i]);
        PrintChar(' ');
    }

    return 0;
}