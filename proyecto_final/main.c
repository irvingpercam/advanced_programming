/*---------------------------------------
* Programación avanzada: Proyecto final
*
* Fecha: 03-06-2020
*
* Autor: Irving Alain Aguilar Pérez - A01703171
*
*
* Referencias: Ejercicios de https://github.com/Manchas2k4/advanced_programming/tree/master/
*----------------------------------------*/
#include "shell.h"
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
