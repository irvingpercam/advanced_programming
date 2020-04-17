/*
 *
 * ITESM - QRO
 * Programacion avanzada
 * Jose carlos pacheco Sanchez
 * Guillermo Carsolio González
 *
 *
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <limits.h>

void get_info(char *, char *, char *);
void list(char *,char *, int, char *);
int dir_file(char *);


void get_info(char *name, char *directory, char *program) {
    int i;
    struct stat info;
    char filename[PATH_MAX + NAME_MAX + 1];

    sprintf(filename, "%s/%s", directory, name);
    lstat(filename, &info);
    printf("Name: %s\n", name);
    printf("Path: %s\n", directory);
    printf("Last access: %s", ctime(&info.st_atime));
    printf("Last modification: %s\n", ctime(&info.st_mtime));

}

int main(int argc, char* argv[]) {

    char dir_name[PATH_MAX + 1];
    char *directory;
    int recursive;
    char *cadena;
    if (argc >= 5 || argc < 3) {
        fprintf(stderr, "usage: %s string initial_directory [-r]\n", argv[0]);
        return -2;
    }

    getcwd(dir_name, PATH_MAX);
    directory = dir_name;
    recursive = 0;

    if (argc == 4) {
        if (strcmp(argv[3], "-r") == 0) {
            recursive = 1;
        } else {
            fprintf(stderr, "usage: %s string initial_directory [-r]\n", argv[0]);
            return -4;
        }
    }
    directory = argv[2];
    if(dir_file(directory) == -1){
        fprintf(stderr, "%s: No such file or directory\n", argv[0]);
        return -3;
    } else if(dir_file(directory) != 0){
        fprintf(stderr, "%s: Not a directory\n", argv[0]);
        return -3;
    }

    cadena = argv[1];
    list(cadena, directory, recursive, argv[0]);
    return 0;
}


int dir_file(char *name){
    struct stat info;
    if(lstat(name, &info) < 0){
        return -1;
    }
    if(S_ISDIR(info.st_mode)){
        return 0;
    } else {
        return -2;
    }
}

int busqueda(char *cadena, char *filename){
    char str[NAME_MAX + 1], search[NAME_MAX + 1];
    sprintf(str, "%s%s", filename, " ");
    sprintf(search, "%s%s", cadena, " ");
    int count1 = 0, count2 = 0, i, j, flag;
    while (str[count1] != 32)
        count1++;
    while (search[count2] != 32)
        count2++;
    for (i = 0; i <= count1 - count2; i++){
        for (j = i; j < i + count2; j++){
            flag = 1;
            if (str[j] != search[j - i]){
                flag = 0;
                break;
            }
        }
        if (flag == 1) break;
    }
    if (flag == 1)
        return 0;
    else
        return -1;
}

void list(char *cadena, char *dir_name, int recursive, char *program) {
    char path[PATH_MAX + NAME_MAX + 1];
    DIR *dir;
    struct dirent *direntry;
    struct stat info;

    if ( (dir = opendir(dir_name)) == NULL ) {
        //perror(program);
        fprintf(stderr, "%s: string initial_directory [-r]\n", program);
        exit(-2);
    }
    dir = opendir(dir_name);

    while( (direntry = readdir(dir)) != NULL) {
        if (strcmp(direntry->d_name, ".") != 0 &&
            strcmp(direntry->d_name, "..") != 0) {
            if(busqueda(cadena, direntry->d_name) == 0) get_info(direntry->d_name, dir_name, program);
        }
    }
    if (recursive) {
        rewinddir(dir);
        while ( (direntry = readdir(dir)) != NULL ) {
            if (strcmp(direntry->d_name, ".") != 0 &&
                strcmp(direntry->d_name, "..") != 0) {

                sprintf(path, "%s/%s", dir_name, direntry->d_name);
                lstat(path, &info);
                if ( (info.st_mode & S_IFMT) == S_IFDIR) {
                    list(cadena, path, recursive, program);
                }
            }
        }
    }
}