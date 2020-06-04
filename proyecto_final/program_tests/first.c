#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
char cmd[512];
char *l1;
int readFile(char* filename, char *buffer) {
    FILE *file = fopen(filename, "r");
    fseek(file, 0L, SEEK_END);
    long ptr = ftell(file);
    rewind(file);
    if (buffer != NULL)
    {
        fread(buffer, ptr, 1, file);
        fclose(file);
        return 1;
    }
}
int lotes(char *argv) {
    int fd;
    /* Validar archivo */
    if ((fd = open(argv, O_RDONLY)) < 0 ) {
        perror("open");
        return -2;
    }
    else if ((readFile(argv, cmd)) == 1) {
            puts("Success!\n");
            l1 = strtok(cmd, ";");
            printf("%s\n", l1);
        
    }
}
void interactive(){
    l1 = strtok(cmd, ";");
    printf("%s\n", l1);
    execlp("l1", "l1", NULL);
    
}
int main(int argc, char *argv[]) {   
    switch (argc) {
    /* [START] INTERACTIVE MODE */
    case 1:
        while(1) {
            printf("mini-shell> ");
            fgets(cmd, 511, stdin);
            cmd[strcspn(cmd, "\n")] = '\0';
            interactive();
        }
        break;
    /* [END] INTERACTIVE MODE */
    /* [START] BATCH MODE */
    case 2:
        lotes(argv[1]);
        break;
    /* [END] BATCH MODE */
    /* [START] Usage errors handler */
    default:
        fprintf(stderr, "usage: %s\n", argv[0]);
        fprintf(stderr, "usage: %s [batchFile]\n", argv[0]);
        return -2;
        break;
    /* [END] Usage error handler */
    }
    return 0;
}
