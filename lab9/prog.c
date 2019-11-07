#include <stdio.h>
#include <string.h>
#include <windows.h>

char* getState(int s) {

	if(s == 4096) {
		return "COMMIT";
	}
	else if(s == 8192) {
		return "RESERVE";
	}
	else if(s == 65536) {
		return "FREE";
	}
	else {
		return "ERROR";
	}
}

int main() {
	struct _SYSTEM_INFO info;
	struct _MEMORY_BASIC_INFORMATION memBasic;
	SIZE_T len;
	char* chunk;
	char* state;

	// TODO
	// how much M installed
	PULONGLONG memKb;
    GetPhysicallyInstalledSystemMemory(memKb);
	printf("%lu GB of RAM installed.\n", &memKb);
    //Console.WriteLine((memKb / 1024 / 1024) + " GB of RAM installed.\n");


	GetSystemInfo(&info);

	printf("Page size: %ld\n", info.dwPageSize);

	chunk = malloc(1000000);

	VirtualQuery(chunk, &memBasic, len);

	state = getState(memBasic.State);

	printf("%s\n", state);

	free(chunk);

	VirtualQuery(chunk, &memBasic, len);

	state = getState(memBasic.State);
	
	printf("%s\n", state);
}
