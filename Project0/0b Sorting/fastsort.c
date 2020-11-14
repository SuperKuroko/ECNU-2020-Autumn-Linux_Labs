#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "sort.h"

void
usage(char *prog) 
{
    fprintf(stderr, "usage: %s <inputfile> <outputfile>\n", prog);
    exit(1);
}

int cmp_rec_t(const void*_a, const void* _b)
{
    rec_t* a = (rec_t*)_a;
    rec_t* b = (rec_t*)_b;
    return a->key > b->key;
}

int
main(int argc, char *argv[])
{
    if(argc != 3) usage(argv[0]);
    // arguments
    char *inFile = argv[1];
    char *outFile = argv[2];
    int fd = open(inFile, O_RDONLY);
    if (fd < 0) {
	perror("open");
	exit(1);
    }
    rec_t r;
    int count=0,i,rc;
    while (1) {
	rc = read(fd, &r, sizeof(rec_t));
	if (rc == 0) 
	    break;
	if (rc < 0) {
	    perror("read");
	    exit(1);
	}
	count++;
    }
    (void) close(fd);
   
   
    rec_t *array;
    array = (rec_t*) malloc (sizeof(rec_t)*count);
    if(array == NULL){
    	perror("No Memory!");
    	exit(1);
    }
    
    fd = open(inFile, O_RDONLY);
    for(i = 0;i < count;i++)	
	read(fd, &array[i], sizeof(rec_t));
    (void) close(fd);
    
    qsort(array,count,sizeof(array[0]),cmp_rec_t);
    
    fd = open(outFile, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
    if (fd < 0) {
	perror("open");
	exit(1);
    }

    for (i = 0; i < count; i++) {
	rc = write(fd, &array[i], sizeof(rec_t));
	if (rc != sizeof(rec_t)) {
	    perror("write");
	    exit(1);
	}
    }
    (void) close(fd);
    free(array);
    return 0;
}

