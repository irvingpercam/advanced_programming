/*---------------------------------------
* Programación avanzada: Actividad 2
*
* Fecha: 13-03-2020
*
* Autor: Irving Alain Aguilar Pérez - A1703171
*
* Autor: Javier Mendez Martínez - A1703446
*
*----------------------------------------*/
/* [START] C Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <limits.h>
/* [END] C Libraries */
/* [START] DECLARING FUNCTIONS */
void checkString(char*, char*, int*, int);
void searchDirs(char*, char*, char*, char*);
void list(char* dir_name, int recursive, char* program, char *str);
/* [END] DECLARING FUNCTIONS */
/* [START] MAIN FUNCTION */
int main(int argc, char* argv[])
{
    char dir_name[PATH_MAX + 1];
    char *directory;
    int recursive;
    char *str;
    if(argc > 4 || argc < 2){
        printf("usage: %s string initial_directory [-r] \n", argv[0]);
		return -2;
    }
    getcwd(dir_name, PATH_MAX);
    directory = dir_name;
    directory = argv[2];
    recursive = 0;
    str = argv[1];
    if (argc == 3)
    {
     recursive = 0;   
    }
    else if(argc == 4){
        if (strcmp(argv[3], "-r") == 0)
        {
            recursive = 1;
        }
        else
        {
            printf("usage: %s string initial_directory [-r] \n", argv[0]);
            return -4;
        }
    }
	/* CALLING LIST FUNCTION TO START SEARCHING FOR THE STRING GIVEN AS PARAMETER */
    list(directory, recursive, argv[0], str);
    return 0;
}
/* [END] MAIN FUNCTION */
/* [START] LIST FUNCTION 

This function iterates between directories.
If the recursive option is given (1), it makes a recursive listing.

*/

void list(char *dir_name, int recursive, char *program, char *str) {
	char path[PATH_MAX + NAME_MAX + 1];
	DIR *dir;
	struct dirent *direntry;
	struct stat info;
	if ( (dir = opendir(dir_name)) == NULL ) {
		perror(program);
		exit(-3);
	}
    while((direntry = readdir(dir)) != NULL) {
		if(strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0) {
            searchDirs(direntry->d_name, dir_name, program, str);
        }
	}
	if(recursive){
        rewinddir(dir);
		while((direntry = readdir(dir)) != NULL) {
			if(strcmp(direntry->d_name, ".") != 0 && strcmp(direntry->d_name, "..") != 0) {
				sprintf(path, "%s/%s", dir_name, direntry->d_name);
				lstat(path, &info);
				if((info.st_mode & S_IFMT) == S_IFDIR) {
					list(path, recursive, program, str);
				}
			}
		}
	}
    closedir(dir);
}
/* [END] LIST FUNCTION */
/* [START] CHECKSTRING FUNCTION

This function compares the string given with a file string.

*/
void checkString(char *str, char *name, int *check, int strl){
    if (strncmp(str, name, strlen(str))){
        *check = 0;
    }
    else{
        *check = 1;
    }
}
/* [END] CHECKSTRING FUNCTION */
/* START SEARCHDIRS FUNCTION 

This function is used to find whether a string exists in a file.
In case the string exists, it prints the name of the file where the string was found.
Also it prints the path where the file is located, the last access and the last modification datetime.

*/
void searchDirs(char *name, char *directory, char *program, char *str) {
	struct stat info;
	int file = 0, check = 0;
	char filename[PATH_MAX + NAME_MAX + 1];
	sprintf(filename, "%s/%s", directory, name);
	lstat(filename, &info);
	switch(info.st_mode & S_IFMT) {
		case S_IFDIR : break;
		case S_IFLNK : break;
		default		 : file = 1; break;
	}	
	/* CALLING CHECKSTRING, IN ORDER TO COMPAFRE STRINGS */
	checkString(str, name, &check, strlen(str));
	if(file && check){
		printf("Name: %s\n", name);
		printf("Path: %s\n", directory);
		printf("Last access: %s\n", ctime(&info.st_atime));
		printf("Last modification: %s\n\n", ctime(&info.st_mtime));
	}
}
/* [END] SEARCHDIRS FUNCTION */