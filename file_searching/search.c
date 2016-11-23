#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>



void  find(char *str, const char *name, int deep){
    struct dirent *dir;
    DIR *dirp = opendir(str);
    struct stat buff;
    printf("%s\n", str);
    char str1[200];
    if (deep == 0)
        return ;
    while ((dir = readdir(dirp)) != NULL){
        str1[0] = '\0';
        strcpy(str1, str);
        strcat(str1, "/");
        strcat(str1, dir->d_name);
        stat(str1, &buff);
        if (S_ISDIR(buff.st_mode))
            {
                if ((strcmp(dir->d_name, "..") != 0) && (strcmp(dir->d_name, ".") != 0)) {
                    find(str1, name, deep - 1);
                } 
            }
        else { 
                if (strcmp(name, dir->d_name) == 0){
                    printf("\tFIND\n");
                    printf("\t%s\n", str1);
                    return ; 
                }
        }
    }
    return;
}

int main(int argc, char **argv) {
    if (argc != 2){
        printf("To few or many arguments\n");
        return 0;
    }
    char str[200] = ".";
    find(str, argv[1], 5);
    return 0;
}
