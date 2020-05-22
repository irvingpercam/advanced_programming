#include<stdio.h>
#include<pthread.h>
 
void *fun_t(void *arg) {        
   printf("%d\n",getpid());
   pthread_exit("Bye");
}

int main(int argc, char* argv[]) {
   pthread_t pt;
   void *res_t;

   if(pthread_create(&pt,NULL,fun_t,NULL) != 0)
      perror("pthread_create");
   if(pthread_join(pt,&res_t) != 0)
      perror("pthread_join");
   printf("%d\n",getpid());
   return 0;
}