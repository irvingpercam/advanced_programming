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
int maxNum(int a, int b)
{
    return a > b ? a : b;
}
int minNum(int a, int b)
{
    return a < b ? a : b;
}
void findMinmax(char *f_name, char *in_dir) {
    int b, n, max = 0, min = INT_MAX;
    char file_path[PATH_MAX + NAME_MAX + 1];
	sprintf(file_path, "%s/%s", in_dir, f_name);
    int origin = open(file_path, O_RDONLY);
    while((b = read(origin, &n, sizeof(int))) != 0)
    {
        max = maxNum(max, n);
        min = minNum(min, n);
    }
    close(origin);
    printf("%s - min = %d - max = %d\n", file_path, max, min);
}
void list(char *dir_name, char *exec_file) {
    DIR *in_dir;
	char dir_path[PATH_MAX + NAME_MAX + 1];
	struct stat buff;
    struct dirent *dir_files = readdir(in_dir);
	printf("directory: %s:\n", dir_name);
	while(dir_files) {
            sprintf(dir_path, "%s/%s", dir_name, dir_files->d_name);
            lstat(dir_path, &buff);
            if ( (buff.st_mode & S_IFMT) == S_IFREG){
                findMinmax(dir_files->d_name, dir_name);
            }
	}
    rewinddir(in_dir);
    while (dir_files) {
        if (strcmp(dir_files->d_name, ".") != 0 &&
            strcmp(dir_files->d_name, "..") != 0) {
            sprintf(dir_path, "%s/%s", dir_name, dir_files->d_name);
            lstat(dir_path, &buff);
            if ( (buff.st_mode & S_IFMT) == S_IFDIR) {
                puts("\n");
                list(dir_path, exec_file);
            }
        }
    }
    closedir(in_dir);
	puts("\n");
}
int main(int argc, char * argv[]){
    DIR *in_dir = opendir(argv[1]);
    char dir_name[PATH_MAX +1];
    getcwd(dir_name, PATH_MAX);
    struct stat buff;
    stat(argv[1], &buff);
    if (argc != 2) {
        printf("usage: %s directory \n", argv[0]);
        return -2;
    }
    else{
        if (access(argv[1], F_OK) == 0 && !(S_ISDIR(buff.st_mode)))
        {
            printf("%s: Not a directory\n", argv[0]);
        }
        else if (S_ISDIR(buff.st_mode))
        {
            if(in_dir == NULL){
                printf("%s: No such file or directory \n", argv[0]);
                return -3;
            }
            else{
                listDirectory(argv[1], argv[0]);
            }
        }
        else
        {
            printf("%s: No such file or directory \n", argv[0]);
            return -3;
        }
    }
}