// -----------------------------------------------------------------------------
/**
 * \file	stream.c
 * \author	Constantin MASSON
 * \date	June 19, 2016
 *
 * \brief	Stream management
 * \note	C Library for the Unix Programming Project
 */
// -----------------------------------------------------------------------------

#include "stream.h"


// ----------------------------------------------------------------------------
// File management
// ----------------------------------------------------------------------------

int64_t bulk_read(int fd, char *buf, size_t buffsize){
	int		c;
	size_t	len=0;
	do {
		c = TEMP_FAILURE_RETRY(read(fd, buf, buffsize));
		if(c<0) {return c;}
		if(c==0) {return len;} //EOF
		buf			+=c; //Forward pointed address by c
		len			+=c;
		buffsize	-=c;
	} while(buffsize>0);
	return len;
}

int64_t bulk_write(int fd, char *buf, size_t buffsize){
	int		c;
	size_t	len=0;
	do{
		c = TEMP_FAILURE_RETRY(write(fd,buf,buffsize));
		if(c<0) {return c;}
		buf			+=c; //Forward pointed address by c
		len			+=c;
		buffsize	-=c;
	}while(buffsize>0);
	return len ;
}

int append_to_file(char *filename, char *buf, size_t len){
	int fd, flags, perms;
	flags = O_WRONLY|O_APPEND|O_CREAT;
	perms = S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP;

	if((fd = TEMP_FAILURE_RETRY(open(filename, flags, perms)) )){
		perror("Open file");
		return -1;
	}
	if(bulk_write(fd, buf, len)<0){
		perror("Write");
		return -1;
	}
	if(TEMP_FAILURE_RETRY(close(fd))<0){
		perror("Close file");
		return -1;
	}
	return 1;
}

