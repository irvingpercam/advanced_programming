#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#define BF_SIZE 512
#define ARG_SIZE 256
void pwd()
{
    char cwd[PATH_MAX];
    char username[1024], hostname[1024];
    username[1023] = '\0';
    hostname[1023] = '\0';
    gethostname(hostname, 1023);
    getlogin_r(username, 1023);
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
       printf("%s@%s:%s (mini_shell) $ > ", username, hostname, cwd);
    }
    else
    {
        printf("mini_shell > ");
    }
}
int exec_cmd(char *cmd_input)
{
    int i = 0;
    char *cmds[ARG_SIZE], *cmd;

    cmd = strtok(cmd_input," ");
    if(cmd == NULL)
    {
        printf("Please insert a command!...\n");
    }
    else
    {
        while(cmd != NULL)
        {
            cmds[i] = cmd;
            i++;
            cmd = strtok(NULL, " ");
        }
        cmds[i] = NULL;
        if (execvp(cmds[0], cmds) == -1) {
            perror("Can't execute command!\n");
            return -3;
        }        
    }
    return 0;
}
void run(char *cmd_input)
{
    int pid, count = 0;
    char *cmd;
    cmd = strtok(cmd_input, "\n");
    cmd = strtok(cmd_input, ";");
    while (cmd != NULL)
    {
        if ((pid = fork()) == 0)
        {
            exec_cmd(cmd);
            exit(0);
        }
        else
        {
            count++;
            cmd = strtok(NULL, ";");
        }
        while (count > 0)
        {
            wait(NULL);
            count--;
        }
    }
}
void interactive_mode()
{
    /* [START] INTERACTIVE MODE */
    char line[BF_SIZE], cwd[PATH_MAX];
    pwd();
    while(fgets(line, BF_SIZE, stdin))
    {
        if(line[0] == '\n')
        {
            pwd();
            continue;
        }
        else if(strcmp(line,"quit\n") == 0 || strcmp(line,"CTRL-D\n") == 0)
        {
            break;
        }
        run(line);
        printf("\n\n");
        pwd();
    }
    exit(0);
    /* [END] INTERACTIVE MODE */
}
//funcion para el modo batch
void batch(char *b_path)
{
    char buff[BF_SIZE], username[1024];
    username[1023] = '\0';
    getlogin_r(username, 1023);
    FILE *fp = fopen(b_path, "r");
    //mientras tenga comandos en el buffer
    while(fgets(buff, sizeof(buff), fp) != NULL)
    {
        //aqui se termina
        if(strcmp(buff, "quit\n") == 0)
        {
            break;
        }
        //para que se parezca al modo interactivo 
        fputs(username, stdout);
        fputs(buff,stdout);
        //corremos los comandos del buffer
        run(buff);
        printf("\n\n");
    }
    fputs("\n", stdout);
    fclose(fp);
    exit(0);
}
void menu(int mode, char *hostname)
{
    if (mode == 0)
    {
        printf("========== Advanced Programming 2020 UNIX Shell 1.0 ==========\n\n");
        sleep(2);
        printf("Stating service on %s...\n\n", hostname);
        sleep(2);
        printf("Mode: Interactive\n\n");
    }
    else
    {
        printf("========== Advanced Programming 2020 UNIX Shell 1.0 ==========\n\n");
        sleep(2);
        printf("Stating service on %s...\n\n", hostname);
        sleep(2);
        printf("Mode: Batch\n\n");
    }
    
}
int main(int argc, char *argv[])
{
    int mode = 0, cmdfile, batch_file;
    struct stat buff;
    char hostname[1024];
    hostname[1023] = '\0';
    gethostname(hostname, 1023);
    if (argc > 2)
    {
        fprintf(stderr, "usage: %s [file]", argv[0]);
        return -1;
    }
    else if (argc == 1)
    {
        menu(mode, hostname);
        interactive_mode();
    }
    else
    {
        if((batch_file = open(argv[1], O_RDONLY)) < 0)
        {
            fprintf(stderr, "%s: the file %s does not exist!\n",argv[0], argv[1]);
            return -2;
        }
        stat(argv[1], &buff);
        if(S_ISREG(buff.st_mode))
        {
            mode = 1;
            menu(mode, hostname);
            batch(argv[1]);
        }else
        {
            fprintf(stderr, "%s: received different file format with %s\n", argv[0], argv[1]);
        }
    }
    return 0;
}
