#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int pid;
  int status;

  pid = fork();
  if(pid < 0){
    printf("fork falló\n");
    exit(1);
  }

  if(pid == 0){
    // hijo
    char *addr = sbrk(0); // Dirección actual del heap
    sbrk(4096); // Reservar una página
    addr[0] = 'Z'; // Escribir valor inicial
    // Proteger contra lectura
    if (mrdprotect(addr, 1) < 0) {
      printf("mrdprotect falló\n");
      exit(1);
    }
    // Escritura aún permitida
    addr[0] = 'A';
    // Intento de lectura debería provocar fallo (muerte del hijo)
    char c = addr[0];
    printf("Valor leído: %c (esto NO debería imprimirse)\n", c);
    // Si llega aquí, la protección no funcionó
    munrdprotect(addr, 1);
    exit(1);
  } else {
    // padre
    if(wait(&status) <= 0){
      printf("wait falló\n");
      exit(1);
    }
    if(status != 0){
      printf("Test pasado: hijo terminó con estado %d\n", status);
      exit(0);
    } else {
      printf("Test falló: hijo terminó con estado 0\n");
      exit(1);
    }
  }
}
