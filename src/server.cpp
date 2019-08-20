#include "user.h"
#include <iostream>
#include <windows.h>
#include <string>
#include <sstream>

#define BUFSIZE 512

using namespace std;

void sendMessage(HANDLE pipe, char buf[]);


int main()
{

	BOOL result;
	char buffer[BUFSIZE];


	cout << "Creating an instance of a named pipe..." << endl;

	// pipe creation
	HANDLE pipe = CreateNamedPipe(
		TEXT("\\\\.\\pipe\\my_pipe"), // name of the pipe
		PIPE_ACCESS_DUPLEX, 
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, 
		1, // # of instances
		BUFSIZE, 
		BUFSIZE,
		0, // wait time
		NULL // use default security attributes
	);

	if (pipe == NULL || pipe == INVALID_HANDLE_VALUE) {
		cout << "Failed to create outbound pipe instance. Error: " << GetLastError() << endl;
		system("pause");
		return 1;
	}

	cout << "Awaiting client connection..." << endl;
	
	// This call blocks until a client process connects to the pipe
	result = ConnectNamedPipe(pipe, NULL);
	if (!result) {
		cout << "Failed to make connection on named pipe. Error: " << GetLastError() << endl;
		CloseHandle(pipe); // close the pipe
		system("pause");
		return 1;
	}
	strcpy_s(buffer, BUFSIZE-1, "****PIPE CONNECTED****" );

	sendMessage(pipe, buffer);
	memset(buffer, 0, BUFSIZE);

	bank user;

	while(1)
	{
		// The read operation will block until there is data to read
		DWORD numBytesRead = 0;
		result = ReadFile(
			pipe,
			buffer, 
			BUFSIZE-1, 
			&numBytesRead, 
			NULL // not using overlapped IO
		);

		if (result) {
			buffer[numBytesRead / sizeof(char)] = '\0'; // null terminate the string
			cout << "Number of bytes read: " << numBytesRead << endl;
			cout << "Message: " << buffer << endl;
		}
		else {
			cout << "Pipe data read failed. Error: " << GetLastError() << endl;
		}


		if (strncmp("end", buffer, 3) == 0 ) {
			break;
		}
		else if (strncmp("balance", buffer, 7) == 0) {
			sprintf_s(buffer, BUFSIZE-1, "balance: %d", user.getBalance());
			sendMessage(pipe, buffer);
		}
		else if (strncmp("add", buffer, 3) == 0) {
			int amount;
			istringstream stream(buffer);
			string s1;

			stream >> s1 >> amount;
			user.deposit(amount);
			sprintf_s(buffer, BUFSIZE-1, "added %d", amount);
			sendMessage(pipe, buffer);
		}

		else {//echo back to client (DEBUG)

			Sleep(5000);// test for synchronous client connection

			sendMessage(pipe, buffer);
		}


	}

	// Close the pipe (automatically disconnects client too)
	CloseHandle(pipe);

	cout << "Done." << endl;

	system("pause");
	return 0;
}


void sendMessage(HANDLE pipe, char buf[]) 
{
	cout << "Sending data to pipe..." << endl;

	// This call blocks until a client process reads all the data
	DWORD numBytesWritten = 0;
	bool result = WriteFile(
		pipe, 
		buf, 
		strlen(buf) * sizeof(char), 
		&numBytesWritten, 
		NULL // not using overlapped IO
	);

	if (result) {
		cout << "Number of bytes sent: " << numBytesWritten << endl;
	}
	else {
		cout << "Failed to send data. Error: " << GetLastError() << endl;
	}
}
