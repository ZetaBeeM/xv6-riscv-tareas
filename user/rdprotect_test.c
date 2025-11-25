#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
  char *addr = sbrk(0);
  sbrk(4096);

  addr[0] = 'Z';

  // Proteger contra lectura
  printf("Ejecutando primer test: Revisar si mrdprotect imprime 0 (funcionamiento correcto).\n");
  if (mrdprotect(addr, 1) < 0) {
    printf("mrdprotect falló\n");
    exit(1);
  }

  
  // Escritura aún permitida
  printf("Ejecutando segundo test: Escribir en addr. Si ocurre un usertrap(), mrdprotect no funciona correctamente.\n");
  addr[0] = 'A';
  printf("Escritura permitida.\n");

  // Intento de lectura debería provocar fallo
  char c = addr[0];
  printf("Valor leído: %c (esto NO debería imprimirse)\n", c);

  // Revertir protección
  if (munrdprotect(addr, 1) < 0) {
    printf("munrdprotect falló\n");
    exit(1);
  }

  printf("Protección revertida correctamente.\n");
  exit(0);
}