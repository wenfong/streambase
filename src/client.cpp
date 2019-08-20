#include <iostream>
#include <windows.h>


#define BUFSIZE 512


void sendMessage(HANDLE pipe, char buf[]);
void readMessage(HANDLE pipe);
DWORD WINAPI InstanceThread(HANDLE pipe);

using namespace std;

BOOL connected;

int main()
{
	BOOL async;
	char buffer[BUFSIZE], input;

	cout << "Enter 's' for synchronous or 'a' for asynchronous connection" << endl;
	cin >> input;

	switch (input) {
		case 's':
			cout << "Synchronous connection" << endl;
			async = FALSE;
			break;
		case 'a':
			cout << "Asychronous connection" << endl;
			async = TRUE;
			break;
		default:
			cout << "Default synchronous connection" << endl;
			async = FALSE;
			break;
	}


	cout << "Connecting to pipe..." << endl;

	// Open the named pipe
	HANDLE pipe = CreateFile(
		TEXT("\\\\.\\pipe\\my_pipe"),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
	);

	if (pipe == INVALID_HANDLE_VALUE) {
		cout << "Pipe connect failed. Error: " << GetLastError() << endl;
		system("pause");
		return 1;
	}
	connected = TRUE;


	//asyncronous connection
	if (async) {
		DWORD  dwThreadId = 0;
		HANDLE hThread = CreateThread(
			NULL,              // no security attribute 
			0,                 // default stack size 
			InstanceThread,    // thread proc
			(LPVOID)pipe,    // thread parameter 
			0,                 // not suspended 
			&dwThreadId      // returns thread ID 
		);
		if (hThread == NULL)
		{
			cout << "CreateThread failed. Error: " << GetLastError() << endl;
			return -1;
		}
		else CloseHandle(hThread);
	}


	//main loop
	while (1)
	{
		if (!async) {
			cout << "Reading data from pipe..." << endl;
			readMessage(pipe);
		}

		// do stuff
		


		if (!connected) {
			break;
		}

	}

	// Close pipe handle
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

void readMessage(HANDLE pipe)
{
	char buffer[BUFSIZE];
	// The read operation will block until there is data to read
	DWORD numBytesRead = 0;
	BOOL result = ReadFile(
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

	cin.getline(buffer, sizeof(buffer));
	sendMessage(pipe, buffer);
	if (strncmp("end", buffer, 3) == 0) {
		connected = FALSE;
	}
}


DWORD WINAPI InstanceThread(HANDLE pipe)
{
	while (1)
	{
		cout << "Reading data from pipe..." << endl;
		readMessage(pipe);

		if (!connected) {
			break;
		}
	}
	return 0;
}
