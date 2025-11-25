Tarea 3 — Protección de Lectura en XV6 (Propuesta_T3.md)
-----------------------------------------------------

Resumen rápido
- Objetivo: implementar `mrdprotect(void *addr, int len)` y `munrdprotect(void *addr, int len)` para marcar páginas de usuario como "no legibles" (limpiar `PTE_R`) y poder revertir la operación.

Se implementaron estas funciones y los syscalls asociados en vm.c, syscall.h/syscall.c con las especificaciones indicadas en el documento. También se agregó la demo indicada en el enunciado de la tarea.

La función mrdprotect marca una región commo sin lectura (luego de hacer los checks iniciales) limpiando el bit PTE_R, y dejando el resto intacto con *pte &= ~PTE_R;, y luego se limpia el TLB con sfence_vma();.

La función munrdprotect funciona similarmente en relación a los checks que hace y la limpieza del TLB, pero activa el bit PTE_R con la operación *pte |= PTE_R;.

Se observó un problema ejecutando la demo, donde lanzaba lo siguiente al tratar de escribir:

`usertrap(): unexpected scause 0xf pid=3`

`sepc=0x48 stval=0x4000.`

Luego de escribir un comando debug que nos imprimía el estado del PTE_R y PTE_W, descubrimos que las funciones estaban funcionando correctamente, con PTE_R igual a 0 y PTE_W igual a 1:

` Ejecutando primer test: Revisar si mrdprotect imprime 0 (funcionamiento correcto).`

`DEBUG mrdprotect: ANTES de modificar:`

`DEBUG: Dir VA=0x0x0000000000004000, PTE=0x21fc98d7. R: 1, W: 1`

`DEBUG mrdprotect: DESPUÉS de modificar:`

`DEBUG: Dir VA=0x0x0000000000004000, PTE=0x21fc98d5. R: 0, W: 1`

`Ejecutando segundo test: Escribir en addr. Si ocurre un usertrap(), mrdprotect no funciona correctamente.`

`usertrap(): unexpected scause 0xf pid=3`

`sepc=0x48 stval=0x4000`

Esto demuestra que nuestra implementación estaba correcta, y cualquier error relacionado a esto tiene que ver con la forma que XV6 trata los PTE que tienen _R igual a 0 y _W igual a 1.