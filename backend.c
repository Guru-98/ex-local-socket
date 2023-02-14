#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>

#include "cmn.h"

typedef struct node {
	struct node * next;
	int data;
} lnode;

// This handles the addition of the node at the end of the list
int addnode(lnode *hnode, int data){
	lnode *newnode;

	//traverse to the end of the ll
	while(hnode != NULL && hnode->next != NULL){
		hnode = hnode->next;
	}

	//create the new node
	newnode = (lnode *)malloc(sizeof(lnode));
	if (newnode == NULL)
		return -ENOMEM;
	memset(newnode, 0, sizeof(lnode));

	//add the new node to ll
	newnode->data = data;
	hnode->next = newnode;

	return 0;
}

// This handles the cmd to print the list from the Fend 
int printlist(lnode *hnode){
	while(hnode != NULL){
		fprintf(stdout, "%d", hnode->data);
		if(hnode->next != NULL)
			fprintf(stdout, " -> ");
		hnode->next = hnode;
	}
	return 0;
}

int main(){
	int  ret;
	struct sockaddr_un sname;	// socket name
	int msock;			// main socket
	int dsock;			// data socket
	char sbuf[SBUF_SIZE];		// socket buffer

	lnode head;			// list head
	memset(&head, 0 , sizeof(head));	

	// create a unix socket
	msock = socket(AF_UNIX, SOCK_SEQPACKET, 0);
	if (msock == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	// bind the socket to a file
	memset(&sname, 0, sizeof(sname));
	sname.sun_family = AF_UNIX;
	strncpy(sname.sun_path, SOCKET_NAME, sizeof(sname.sun_path)-1);

	ret = bind(msock, (const struct sockaddr *) &sname, sizeof(sname));	
	if (ret == -1) {
		perror("bind");
		exit(EXIT_FAILURE);
	}
	
	// listen on the sock
	ret = listen(msock, 5);
	if (ret == -1){
		perror("listen");
		exit(EXIT_FAILURE);
	}

	// eventloop
	while(1){
		dsock = accept(msock, NULL, NULL);
		if (dsock == -1) {
			perror("accept");
			exit(EXIT_FAILURE);
		}

		ret = read(dsock, &sbuf, sizeof(sbuf));	
		if (ret == -1) {
			perror("read");
			exit(EXIT_FAILURE);
		}

		switch (sbuf[0]) {	// first byte will be the opt from Fend
			case 1:
				// add node to the list
				addnode(&head, sbuf[1]); // sec byte will be the data
			break;
			case 2:
				// print the ll
				printlist(&head);
			break;
		}
		close(dsock);
	}

	close(msock);

	return 0;
}
