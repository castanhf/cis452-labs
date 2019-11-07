#include <stdio.h>
#include <string.h>
#include <Windows.h>

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

	// TODO: does not understand error
	// how much M installed
	PULONGLONG memKb;
    	GetPhysicallyInstalledSystemMemory(memKb);
	printf("%lu KB of RAM installed.\n", &memKb);


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
