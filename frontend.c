#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>

#include "cmn.h"

int main() {
	int ret;
	uint8_t opt;
	int data;

	char buf[2];
	struct sockaddr_un addr;
	int dsock;

	// create unix socket for IPC with Bend
	dsock = socket(AF_UNIX, SOCK_SEQPACKET, 0);
	if (dsock == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	// connect to Bend socket
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path)-1);

	ret = connect(dsock, (const struct sockaddr *) &addr, sizeof(addr));	
	if (dsock == -1) {
		fprintf(stderr, "Server is 404");
		exit(EXIT_FAILURE);
	}

	while(1){
		printf("1. Add a node\n2. Print the list\n\n%% : ");
		scanf("%hhd", &opt);
		buf[0] = opt; 

		switch(opt) {
			case 1:
				printf("\n  Enter the data:  ");
				scanf("%d", &data);
				buf[1] = data;
			break;
		}

		ret = write(dsock, &buf, 2);
		if (ret == -1) {
			perror("write");
			break; // exit from the cli menu loop
		}

		//clean the buffer
		memset(&buf, 0, sizeof(buf));
	}

	close(dsock);

	return 0;
}
