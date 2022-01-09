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
#include "filesys.h"
#include "PCB.h"

#define BUFFER_MAX_LENGTH 255
#define FILE_MAX_LENGTH 32

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

/*  Input:   None
    Output:  int value
    Purpose: Read an int number from console */
int SysReadNum()
{
	char c;
	int res = 0;
	int sign = 1;
	bool isValid = true;

	// skip blank space and LF character
	c = kernel->synchConsoleIn->GetChar();
	while (c == ' ' || c == '\n')
		c = kernel->synchConsoleIn->GetChar();

	if (c == '-') // check negative
	{
		sign = -1;
		c = kernel->synchConsoleIn->GetChar();
	}

	while (c != ' ' && c != '\n')
	{
		if (isValid) // if number is valid
		{
			if (c >= '0' && c <= '9')
			{
				res *= 10;
				res += sign * (c - '0');
				if ((sign > 0 && res < 0) || (sign < 0 && res > 0)) // overflow
					isValid = false;
			}
			else
				isValid = false;
		}
		c = kernel->synchConsoleIn->GetChar();
	}

	if (isValid)
		return res;
	return 0;
}

/*	Input: int value
	Output: None
	Purpose: Print an int number to console */
void SysPrintNum(int number)
{
	// in case the input is 0
	if (number == 0)
	{
		// print '0' to the console and return
		kernel->synchConsoleOut->PutChar('0');
		return;
	}

	// in case the input is a negative number, print minus sign to console
	if (number < 0)
	{
		kernel->synchConsoleOut->PutChar('-');
	}

	int digitCount = 0; // stores number of digits
	int digits[10];		// array stores each digit
						// (int type has up to 10 digits: -2,147,483,648 -> 2,147,483,647)

	// store each digit from right to left to the digits array and count the number of digits
	// if number is a negative number, each digit will have a minus sign
	int numberCopy = number;
	while (numberCopy != 0)
	{
		digits[digitCount] = numberCopy % 10;
		numberCopy /= 10;
		digitCount++;
	}

	// print digits array to console in reverse order
	if (number > 0)
	{
		for (int i = 0; i < digitCount; i++)
		{
			// (0 -> 9) + '0' = ('0' -> '9')
			kernel->synchConsoleOut->PutChar(digits[digitCount - 1 - i] + '0');
		}
	}
	else
	{
		for (int i = 0; i < digitCount; i++)
		{
			// -(0 -> -9) + '0' = ('0' -> '9')
			kernel->synchConsoleOut->PutChar(-digits[digitCount - 1 - i] + '0');
		}
	}
}

/*  Input:   None
    Output:  a character (char)
    Purpose: Read a char from console */
char SysReadChar()
{
	return kernel->synchConsoleIn->GetChar();
}

/*  Input:   a character 
    Output:  None
    Purpose: Print a character to console */
void SysPrintChar(char c)
{
	kernel->synchConsoleOut->PutChar(c);
}

/*  Input:   None
    Output:  a random value (int)
    Purpose: Create random int value */
int SysRandomNum()
{
	RandomInit((unsigned int)time(NULL));
	return RandomNumber();
}

/*  Input:   - address of buffer to store string in user mode (char*)
			 - max length of string
    Output:  - None
    Purpose: Read a string from console and store at provided address */
int SysReadString(char *virtAddr, int length)
{
	if (length < 0) // invalid length
		return -1;

	char *buffer = new char[length + 1];
	if (buffer == NULL) // cannot allocate
		return -1;

	// get char from console and put into buffer
	int i = -1;
	while (i < length)
	{
		char c = kernel->synchConsoleIn->GetChar();
		if (c != '\n')
			buffer[++i] = c;
		else
			break;
	}
	buffer[i + 1] = 0;							// mark end of string
	System2User((int)virtAddr, length, buffer); // copy to user memory
	delete[] buffer;
	return i + 1;
}

/*	Input: Address of buffer stores string in user space
	Output: None
	Purpose: Print a string to console */
int SysPrintString(int virtAddr)
{
	// copy buffer from user memory space to system memory space
	char *sysBuffer = User2System(virtAddr, BUFFER_MAX_LENGTH);

	// return if system does not have enough memory
	if (sysBuffer == NULL)
		return -1;

	// print each character in buffer to console
	int index = 0;
	int count = 0;
	while (sysBuffer[index] != 0)
	{
		kernel->synchConsoleOut->PutChar(sysBuffer[index]);
		index++;
		count++;

		// if system buffer is full but the string is not ended
		if (index == BUFFER_MAX_LENGTH)
		{
			// de-allocate system buffer
			delete[] sysBuffer;
			sysBuffer = NULL;

			// copy next BUFFER_MAX_LENGTH characters from user memory space to system memory space
			virtAddr += BUFFER_MAX_LENGTH;
			sysBuffer = User2System(virtAddr, BUFFER_MAX_LENGTH);

			// return if system does not have enough memory
			if (sysBuffer == NULL)
				return count;

			// reset index from 0
			index = 0;
		}
	}
	// de-allocate system buffer
	delete[] sysBuffer;
	return count;
}

/*	Input: Address of buffer stores file name in user space
	Output: 0 if successful, otherwise -1
	Purpose: Create an empty file */
int SysCreateFile(int virtAddr)
{
	char *name = User2System(virtAddr, FILE_MAX_LENGTH + 1);
	if (strlen(name) == 0)
	{
		DEBUG(dbgSys, "Invalid file name!");
		return -1;
	}
	if (name == NULL)
	{
		DEBUG(dbgSys, "Not enough system memory!");
		delete[] name;
		return -1;
	}
	if (!kernel->fileSystem->Create(name, 0))
	{
		DEBUG(dbgSys, "Create file unsuccessfully!");
		delete[] name;
		return -1;
	}
	DEBUG(dbgSys, "Create file successfully!");
	delete[] name;
	return 0;
}

/*	Input: 	Address of buffer stores file name in user space
			Type 0 for read and write mode, 1 for read only mode
	Output: 0 if successful, otherwise -1
	Purpose: Open a file */
int SysOpen(int virAddr, int type)
{
	if (type != 0 && type != 1){
		DEBUG(dbgSys, "Invalid type!");
		return -1;
	}

	char *name = User2System(virAddr, BUFFER_MAX_LENGTH);
	if (name == NULL || strlen(name) == 0)
	{
		DEBUG(dbgSys, "Invalid file name!");
		return -1;
	}

	PCB *curr = kernel->pTab->getCurrentPCB();
	int id = curr->filemap->FindAndSet();
	if (id == -1)
	{
		DEBUG(dbgSys, "Not enough file descriptor!");
		delete[] name;
		return -1;
	}

	FILE *f = fopen(name, type == 0 ? "ab+" : "rb");
	if (f == NULL)
	{
		DEBUG(dbgSys, "File does not exist!");
		curr->filemap->Clear(id);
		delete[] name;
		return -1;
	}

	curr->fileTable[id] = f;
	delete[] name;
	return id;
}

/*	Input: File's ID in file table
	Output: 0 if successful, otherwise -1
	Purpose: Close a file */
int SysClose(int id)
{
	if (id <= 1 || id >= MAX_FILE)
		return -1;

	PCB *curr = kernel->pTab->getCurrentPCB();
	if (!curr->filemap->Test(id))
	{
		DEBUG(dbgSys, "File descriptor does not exist!");
		return -1;
	}

	curr->filemap->Clear(id);
	fclose(curr->fileTable[id]);
	curr->fileTable[id] = NULL;
	return 0;
}

/*	Input: 	Address store the buffer in user mode
			Maximum number of characters read
			ID of file read
	Output: Actual number of characters read
	Purpose: Read from a file or console */
int SysRead(int virAddr, int charcount, int id)
{
	if (id == 0)
		return SysReadString((char *)virAddr, charcount);
		
	if (charcount < 0 || id <= 1 || id >= MAX_FILE)
		return -1;

	PCB *curr = kernel->pTab->getCurrentPCB();
	if (!curr->filemap->Test(id))
	{
		DEBUG(dbgSys, "File descriptor does not exist!");
		return -1;
	}

	char *buffer = new char[charcount + 1];
	if (buffer == NULL)
	{
		DEBUG(dbgSys, "Not enough system memory!");
		return -1;
	}

	int count = fread(buffer, 1, charcount, curr->fileTable[id]);
	if (count == 0)
	{
		DEBUG(dbgSys, "End of file!");
		delete[] buffer;
		return -2;
	}

	System2User((int)virAddr, count, buffer);
	delete[] buffer;
	return count;
}

/*	Input: 	Address store the buffer in user mode
			Maximum number of characters written
			ID of file read
	Output: Actual number of characters written
	Purpose: Write to a file or console */
int SysWrite(int virAddr, int charcount, int id)
{
	if (id == 1)
		return SysPrintString(virAddr);

	if (charcount < 0 || id <= 1 || id >= MAX_FILE)
		return -1;

	PCB *curr = kernel->pTab->getCurrentPCB();
	if (!curr->filemap->Test(id))
	{
		DEBUG(dbgSys, "File descriptor does not exist!");
		return -1;
	}

	char *string = User2System(virAddr, charcount);
	if (string == NULL)
	{
		DEBUG(dbgSys, "Not enough system memory!");
		return -1;
	}

	int count = fwrite(string, 1, charcount, curr->fileTable[id]);
	delete[] string;
	return count;
}

/*	Input: 	Address store program's name in user mode
	Output: Program's ID or -1 if unsuccessful
	Purpose: Execute a program in a new thread */
int SysExec(int virAddr)
{
	char *fileName = User2System(virAddr, BUFFER_MAX_LENGTH);
	if (fileName == NULL)
	{
		return -1;
	}

	int pid = kernel->pTab->ExecUpdate(fileName);
	return pid;
}

/*	Input: 	ID of program 
	Output: exitcode = 0 if successul, otherwise -1 or error code
	Purpose: Wait and block a process */
int SysJoin(int id)
{
	return kernel->pTab->JoinUpdate(id);
}

/*	Input: 	exitcode of joined program
	Output: exitcode of joined program
	Purpose: exit the joined process */
int SysExit(int ec)
{
	if (ec == 0)
	{
		ec = kernel->pTab->ExitUpdate(ec);
		kernel->currentThread->FreeSpace();
		kernel->currentThread->Finish();
	}
	return ec;
}

/*	Input: 	Address store semaphore's name in user mode
			Value of semaphore
	Output: 0 if successful, otherwise -1
	Purpose: create a semaphore */
int SysCreateSemaphore(int virtAddr, int semval)
{
	char *name = User2System(virtAddr, BUFFER_MAX_LENGTH);
	if (name == NULL || strlen(name) == 0)
	{
		return -1;
	}

	return kernel->semTab->Create(name, semval);
}

/*	Input: 	Address store semaphore's name in user mode
	Output: 0 if successful, otherwise -1
	Purpose: Release the waiting process */
int SysSignal(int virtAddr)
{
	char *name = User2System(virtAddr, BUFFER_MAX_LENGTH);
	if (name == NULL || strlen(name) == 0)
	{
		return -1;
	}

	return kernel->semTab->Signal(name);
}

/*	Input: 	Address store semaphore's name in user mode
	Output: 0 if successful, otherwise -1
	Purpose: Wait a process */
int SysWait(int virtAddr)
{
	char *name = User2System(virtAddr, BUFFER_MAX_LENGTH);
	if (name == NULL || strlen(name) == 0)
	{
		return -1;
	}

	return kernel->semTab->Wait(name);
}

/*	Input: 	None
	Output: ID of the current process
	Purpose: Get ID of the current process */
int SysGetCurrentProcessId(){
	return kernel->currentThread->processID;
}

#endif /* ! __USERPROG_KSYSCALL_H__ */
