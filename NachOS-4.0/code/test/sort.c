#include "syscall.h"

int main(){
    int n;
    int a[100];
    char ord; // ord = i for increasing order, ord = d for decreasing order
    int i, j, temp;

    PrintString("Enter number of elements (0<n<=100): ");
    n = ReadNum();
    // handle n>100
    while (n>100 || n<=0){
        PrintString("Invalid number!\n");
        PrintString("Enter number of elements (0<n<=100): ");
        n = ReadNum();
    }

    PrintString("Enter elements: ");
    for (i=0; i<n; i++){
        a[i] = ReadNum();
    }

    // choose order
    PrintString("Choose the order to sort: \n");
    PrintString("i for increasing order\n");
    PrintString("d for decreasing order\n");
    PrintString("Enter the order: ");
    ord = ReadChar();

    // bubble sort in increasing order
    if (ord == 'i'){
        for (i=0; i<n; i++)
            for (j=0; j<n-i-1; j++)
                if (a[j] > a[j+1]){
                    temp = a[j];
                    a[j] = a[j+1];
                    a[j+1] = temp;
                }
    }
    // bubble sort in decreasing order
    else if (ord == 'd'){
        for (i=0; i<n; i++)
            for (j=0; j<n-i-1; j++)
                if (a[j] < a[j+1]){
                    temp = a[j];
                    a[j] = a[j+1];
                    a[j+1] = temp;
                }
    }
    // handle invalid order
    else{
        PrintString("Invalid order!\n");
        Halt();
        return 0;
    }

    // print result
    PrintString("Sorted array: ");
    for (i=0; i<n; i++){
        PrintNum(a[i]);
        PrintChar(' ');
    }
    PrintString("\n");

    Halt();
    return 0;
}