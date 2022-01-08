#include "syscall.h"

int main() 
{
	int pingPID, pongPID;

	// PrintString("Ping-Pong test starting ...\n");
	
	pingPID = Exec("./test/ping");
	pongPID = Exec("./test/pong");
	Join(pingPID);
	Join(pongPID);
}
