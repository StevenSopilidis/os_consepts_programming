#include "stdio.h"
#include "stdlib.h"
#include "fcntl.h"
#include "unistd.h"
#include "string.h" 
#include "errno.h"

int main(int argc,char* argv[]) {
    if(argc < 3)
    {
        printf("User must provide the src and dest file to copy two and from\n");
        return -1;
    }
    char *srcFilename = argv[1];
    char *destFilename = argv[2];
    printf("Src file specified: %s\n", srcFilename);
    printf("Dest file specified: %s\n", destFilename);
    int srcFile = open(srcFilename, O_RDONLY | O_CREAT);
    if(srcFile == -1)
    {
        printf("Src file specified could not be opened, errno: {%d}\n", errno);
        return -1;
    }
    int srcFileContentSize = lseek(srcFile, 1, SEEK_END);
    printf("Content size of src file: %d\n", srcFileContentSize);
    char* buffer = (char *)malloc(srcFileContentSize + 1);
    int srcFileRead = read(srcFile, buffer, srcFileContentSize);
    if(srcFileRead == -1)
    {
        char *errmsg = strerror(errno);
        printf("Could not read from src file ------ error message: %s\n", errmsg);
        return -1;
    }
    int destFile = open(destFilename, O_WRONLY | O_CREAT);
    if(destFile == -1)
    {
        char *errmsg = strerror(errno);
        printf("could not open dest file ----- error message: %s\n", errmsg);
        return -1;
    }
    int successWrite = write(destFile, buffer, srcFileContentSize);
    if(successWrite == -1)
    {
        char *errmsg = strerror(errno);
        printf("could not write to dest file ----- error message: %s\n", errmsg);
        return -1;
    }
    return 0;
}