/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__
#define __USERPROG_KSYSCALL_H__

#include "kernel.h"
#include "synchconsole.h"

/*  Input:  - User space address (int)        
            - Limit of buffer (int) 
    Output: - Buffer (char*) 
    Purpose: Copy buffer from User memory space to System memory space */
char *User2System(int virtAddr, int limit)
{
	int i; // index
	int oneChar;
	char *kernelBuf = new char[limit + 1]; //need for terminal string

	if (kernelBuf == NULL)
		return kernelBuf;

	memset(kernelBuf, 0, limit + 1);

	for (i = 0; i < limit; i++)
	{
		kernel->machine->ReadMem(virtAddr + i, 1, &oneChar);
		kernelBuf[i] = (char)oneChar;
		if (oneChar == 0)
			break;
	}

	return kernelBuf;
}

/*  Input:  - User space address (int)        
            - Limit of buffer (int)        
            - Buffer (char[]) 
    Output: - Number of bytes copied (int) 
    Purpose: Copy buffer from System memory space to User memory space */
int System2User(int virtAddr, int len, char *buffer)
{
	if (len < 0)
		return -1;

	if (len == 0)
		return len;

	int i = 0;
	int oneChar = 0;

	do
	{
		oneChar = (int)buffer[i];
		kernel->machine->WriteMem(virtAddr + i, 1, oneChar);
		i++;
	} while (i < len && oneChar != 0);

	return i;
}

void SysHalt()
{
	kernel->interrupt->Halt();
}

int SysAdd(int op1, int op2)
{
	return op1 + op2;
}

// Input: mot so nguyen integer
// Output: khong
// Chuc nang: In so nguyen len man hinh console
void SysPrintNum(int number)
{
	// truong hop input la so 0
	if (number == 0)
	{
		// in ra man hinh so 0
		kernel->synchConsoleOut->PutChar('0');
	}
	
	// neu la so am, in dau tru ra man hinh va doi thanh so duong
	if (number < 0)
    {
        kernel->synchConsoleOut->PutChar('-');
        number = -number;
    }

	int digitCount = 0;	// bien dem so chu so
	int digits[10];		// mang luu cac chu so (kieu int co toi da 10 chu so -2,147,483,648 -> 2,147,483,647)

	// luu tung chu so tu phai qua trai vao mang digits va dem so chu so
	while (number != 0)
    {
        digits[digitCount] = number % 10;
        number /= 10;
        digitCount++;
    }

	// in cac chu so ra man hinh theo thu tu nguoc lai
	for(int i = 0; i < digitCount; i++)
	{
		// (0 -> 9) + '0' = ('0' -> '9')
		kernel->synchConsoleOut->PutChar(digits[digitCount - 1 - i] + 48);	// '0' = 48
	}
}

// Input: dia chi vung nho user
// Output: khong
// Chuc nang: In chuoi len man hinh console
void SysPrintString(int virtAddr)
{
	// sao chep buffer co do dai 255 tu vung nho user sang vung nho system 
	char *sysBuffer = User2System(virtAddr, 255);

	// ket thuc neu system khong du vung nho
	if(sysBuffer == NULL)
		return;
	
	// in tung ky tu trong buffer len man hinh console
	int index = 0;
    while (sysBuffer[index] != 0)
    {
        kernel->synchConsoleOut->PutChar(sysBuffer[index]);
        index++;

        // neu system buffer da day nhung chuoi chua ket thuc
        if (index == 255)
        {
			// thu hoi system buffer
            delete[] sysBuffer;
			sysBuffer = NULL;

			// sao chep 255 ky tu tiep theo tu vung nho user sang vung nho system
            virtAddr += 255;
            sysBuffer = User2System(virtAddr, 255);

            // ket thuc neu system khong du vung nho
            if (sysBuffer == NULL)
                return;

			// dat index lai tu 0
            index = 0;
        }
	}
	// thu hoi system buffer
	delete[] sysBuffer;
}

#endif /* ! __USERPROG_KSYSCALL_H__ */
