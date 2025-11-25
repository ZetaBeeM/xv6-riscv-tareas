#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int n;
    int i;
    if (argc < 2) {
        printf("Uso: demo <cantidad_de_procesos>\n"); // Se debe especificar la cantidad de procesos que quiere el usuario.
        exit(1);
    }
    else {
        if (atoi(argv[1]) < 10){ // Si bien está a elección del usuario cuantos procesos ejecutar en la demo, las instrucciones dicen 10 mínimo, entonces cualquier número
            // menor a 10 se cambiará a 10.
            n = 10;
        }
        else{
            n = atoi(argv[1]); // Si la cantidad de procesos es mayor a 10, se usa esa.
        }
        printf("Iniciando demo con %d procesos\n", n);
    }

    for (i = 0; i < n; i++) {
        int pid = fork(); // Se crean los procesos con fork.
        if (pid < 0) { // Se revisa la creación de estos para avisar si alguno falla.
            printf("Fork falló\n");
        exit(1);
        }
        if (pid == 0) { // Si funciona, empieza el proceso.
            int tickets = 50 * (i + 1); // Asignamos la cantidad de tickets acorde a las instrucciones.
            settickets(tickets); // Usamos settickets() para asignarle estos al proceso actual.

            printf("Proceso %d (pid=%d) con %d tickets ejecutandose.\n", i, getpid(), tickets); // Indicamos cual proceso se esta ejecutando, su pid, y su cantidad de tickets.
            volatile int j; // Como estamos haciendo que cada proceso no haga nada 100 millon de veces (es instruccion simple y funciona para probar esto) ocupamos volatile para que ejecute cada una de las iteraciones.
            for (j = 0; j < 100000000; j++) {
            }
            printf("Proceso %d (pid=%d) terminó.\n", i, getpid()); // Imprimimos los procesos que han terminado para saber en que orden terminan.
            exit(0);
        }
    }

    for (i = 0; i < n; i++) { // Esperamos a que terminen todos los procesos.
        wait(0);
    }

  printf("Terminaron todos los procesos.\n");
  exit(0);
}
