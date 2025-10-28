# Tarea 2 TICS312 - Implementación de Lottery Scheduling en XV6

Lottery Scheduling es un algoritmo de planificacion de procesos probabilístico que consiste en repartir una cierta cantidad de tickets a cada proceso y hacer una lotería donde se elige aleatoriamente un número dentro de rango de la suma de tickets totales de procesos existentes y listos para ejecutarse. De esta forma, el proceso que tenga el ticket correspondiente será ejecutado, y luego se repetirá el algoritmo para elegir otro.

##  1. Funcionamiento y lógica de la implementación

La implementación realizada en esta tarea consiste en añadir un campo de tickets y otro de run_slices a todas estructuras proc, indicativo de los tickets usados en este scheduler y run_slices para registrar la cantidad de veces que se ha ejecutado un proceso. Luego, se les asigna un número default de tickets a cada proceso, y al momento de la planificación, la CPU hace lo siguiente:

1. Inicializar las variables de tipo int "total", "lotto", y "actual", con el fin de poder registrar, en orden, la cantidad de tickets total, el número del ticket elegido, y un registro de cuantos tickets se han contado, actualizandose en cada ejecución del bucle que cuenta los tickets de cada proceso RUNNABLE.

2. Al fin de cada time quantum (o la inicialización de una CPU), se les asigna 0 a total y actual, ya que se necesita calcular la cantidad total de tickets.

3. Un bucle for recorre la lista de procesos, sumando la cantidad de tickets en procesos RUNNABLE a total, y si el proceso tiene menos de un ticket (o no tiene), se les asigna un ticket.

4a. En el caso de que se encuentren cero tickets, implicando que no hay procesos RUNNABLE en esta CPU, se continúa al proximo time quantum con continue para no bloquear el scheduler, repitiendo los pasos 1-3.

4b. Si hay una cantidad valida de tickets disponibles, a lotto se le asigna generadortickets(total). Al generar un número al azar con do_rand() y luego calcular el módulo de este número y total, esto da como resultado un número en el rango de [1, total].

5. Nuevamente se recorre la lista de procesos con un bucle for, sumandole a actual la cantidad de tickets de estos si se encuentran en estado runnable. De esta forma, cuando actual >= lotto se empezará a ejecutar el proceso que lo llevó a ese valor, ya que este será correspondiente al proceso que tiene el ticket elegido por rand().

6. El proceso de ejecución será igual al existente en xv6, con la adición de una linea que incrementa el campo run_slices por uno para indicar la cantidad de ejecuciones de este proceso.

##  2. Explicación de las modificaciones realizadas (archivos y cambios clave)

Se realizaron modificaciones en los siguientes archivos:

- sysproc.c

Creación de system call sys_settickets(), cuyo proposito es modificar los tickets de un proceso, y asignar 1 a estos si se elige un número de tickets menor a 1.

- syscall.c/syscall.h

Definimos SYS_settickets como la system call número 22 en syscall.h, y en syscall.c definimos el prototipo sys_settickets() e introducimos un puntero que dirije SYS_settickets a sys_settickets().

- usys.pl/user.h

En user.h definimos la función int settickets(int n) y añadimos entry("settickets") en usys.pl para que el usuario pueda acceder a esta system call.

- proc.c/proc.h

En proc.h se realizan modificaciones a la estructura proc, introduciendo un campo int "tickets" que indica la cantidad de tickets de un proceso, y otro int "run_slices" que registra la cantidad de veces que se ha ejecutado un proceso.

En proc.c, se modifica la función allocproc() para asignarles 100 tickets a procesos previamente UNUSED y cero run_slices. Similarmente, se modifican las funciones freeproc(struct proc *p) y kfork() para darles los tickets y run_slices apropiados (0/0 para procs UNUSED y 100/0, el valor defecto de procesos nuevos en el caso de kfork). También se implementa la función do_rand (tomada de grind.c) que genera un número al azar y generadortickets, que encuentra el módulo de este número y la cantidad total de tickets para poder elegir uno al azar dentro del rango posible.

El cambio principal hecho a proc.c viene en la forma de las modificaciones hechas a scheduler() con el fin de cambiar el Round-Robin de XV6 por Lottery Scheduling. Esto incluye la creación de las variables total, lotto, y actual cuyo proposito se encuentra detallado en la primera sección del README. También se introducen bucles for para poder realizar operaciones relacionadas a los tickets como calcular el total de estos, elegir uno al azar, y encontrar el proceso que tiene el ticket elegido para ejecutarlo.

- pruebalottery.c/demo.c (mismo archivo solo incluído dos veces para seguir el nombre indicado por las instrucciones)

Esta demo crea una cantidad de procesos indicada por el usuario (uso: pruebalottery <número de procesos>) y crea estos usando fork, dandoles una cantidad progresivamente mayor de tickets a cada proceso creado, (50 * (i + 1), donde i es el i-esimo proceso creado) y dandoles la instrucción de no hacer nada 100 millones de veces. Luego imprime cuales terminaron y sus pid, y espera a que todos terminen para imprimir que todos terminaron.

## 3. Dificultades encontradas y soluciones implementadas

El mayor problema durante el desarollo de esta tarea fue familiarizarse con todos los distintos archivos que se debían cambiar y el syntax que se debía seguir, y esto fue remediado por leer la amplia cantidad de documentación existente sobre XV6 en internet.

Otros problemas incluyeron:

- Crear una función que eligiera al azar un número dentro de un rango: Esto fue solucionado con copiar la implementación de do_rand() de que ya se encuentra en grind.c, y usar el módulo del número generado y la cantidad total de tickets para generar un número que esté dentro del rango de tickets.

- La system call no recibía el número de tickets que introduciamos ni el PID del proceso cuyos tickets queriamos modificar: Se ocupó argint ya que xv6 no permite pasar argumentos del usuario al kernel directamente.

- La creación de la demo y que cosas iban a hacer los procesos que pudiera demostrar el funcionamiento correcto del scheduler: Los procesos hicieron la acción más simple posible - nada, por 100 millones de veces.

- La demo, dado que ejecutaba procesos que hacian nada 100 millones de veces, causaba que se cerrara WSL: Se ejecutó make qemu CPUS=1 en vez de make qemu.
## 4. Posibles problemas con Lottery Scheduling

El principal problema con esta implementación de Lottery Scheduling es no incluir una reasignación automatica de tickets. Si bien esto hace la programación de procesos justa ya que por defecto todos van a tener la misma cantidad de tickets, esto puede causar que procesos de baja prioridad sean elegidos sobre aquellos que sean más importantes en el funcionamiento del sistema operativo por tener la misma probabilidad de ser elegidos.

Otro problema de este es el caso que un proceso malicioso ocupe settickets para aumentar su cantidad de tickets a un punto donde sea elegido por la lotería la mayoría de las veces (ticket inflation).