/* SHELL LIBRARY */
#ifndef SHELL_H
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
/* [START] SHOW CURRENT DIRECTORY WITH SHELL NAME*/
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
        // IF GETTING CURRENT DIRECTORY FAILS
        printf("mini_shell > ");
    }
}
/* [START] SHOW CURRENT DIRECTORY WITH SHELL NAME*/
/* [START] EXECUTE COMMAND FUNCTION */
int exec_cmd(char *cmd_input)
{
    int i = 0;
    char *cmds[ARG_SIZE], *cmd;

    cmd = strtok(cmd_input," ");
    if(cmd == NULL)
    {
        printf("Null input, please insert a command!...\n");
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
        if (execvp(cmds[0], cmds) == 0) {
            printf("Running command...\n");
        }
        else
        {
            // IF CAN'T EXECUTE COMMAND
            perror("Can't execute command!\n");
            return -3;
        }
            
    }
    return 0;
}
/* [END] EXECUTE COMMAND FUNCTION */
/* [START] RUN COMMANDS FUNCTION */
void run(char *cmd_input)
{
    int pid, count = 0;
    char *cmd;
    cmd = strtok(cmd_input, "\n");
    cmd = strtok(cmd_input, ";");
    while (cmd != NULL)
    {
        // CREATE PROCESS
        if ((pid = fork()) == 0)
        {
            // EXECUTE COMMAND
            exec_cmd(cmd);
            sleep(1);
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
/* [START] RUN COMMANDS FUNCTION */
void interactive_mode()
{
    /* [START] INTERACTIVE MODE */
    char line[BF_SIZE], cwd[PATH_MAX];
    pwd();
    while(fgets(line, BF_SIZE, stdin))
    {
        if(line[0] == '\n')
        {
            pwd(); // SHOW CURRENT DIRECTORY
            continue;
        }
        else if(strcmp(line,"quit\n") == 0 || strcmp(line,"CTRL-D\n") == 0)
        {
            break;
        }
        run(line); //RUN LINE COMMAND
        printf("\n\n");
        pwd(); // SHOW CURRENT DIRECTORY
    }
    exit(0);
/* [END] INTERACTIVE MODE */
}
/* [START] BATCH MODE */
void batch(char *b_path)
{
    char buff[BF_SIZE], username[1024];
    username[1023] = '\0';
    getlogin_r(username, 1023);
    FILE *fp = fopen(b_path, "r");
    while(fgets(buff, sizeof(buff), fp) != NULL)
    {
        // QUIT BATCH MODE
        if(strcmp(buff, "quit\n") == 0)
        {
            break;
        }
        printf("\n");
        // EXECUTE BUFF COMMAND
        run(buff);
        printf("\n\n");
    }
    fputs("\n", stdout);
    fclose(fp);
    exit(0);
}
/* [END] BATCH MODE */
/* [START] SHELL USER MENU */
void menu(int mode, char *hostname)
{
    // FOR INTERACTIVE MODE
    if (mode == 0)
    {
        printf("========== Advanced Programming 2020 UNIX Shell 1.0 ==========\n\n");
        sleep(2);
        printf("Stating service on %s...\n\n", hostname);
        sleep(2);
        printf("Mode: Interactive\n\n");
    }
    // FOR BATCH MODE
    else
    {
        printf("========== Advanced Programming 2020 UNIX Shell 1.0 ==========\n\n");
        sleep(2);
        printf("Stating service on %s...\n\n", hostname);
        sleep(2);
        printf("Mode: Batch\n\n");
    }
}
/* [END] SHELL USER MENU */
#define SHELL_H
#endif /*SHELL*/