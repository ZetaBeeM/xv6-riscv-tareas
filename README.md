Tarea 3 — Protección de Lectura en XV6 (Propuesta_T3.md)
-----------------------------------------------------

Resumen rápido
- Objetivo: implementar `mrdprotect(void *addr, int len)` y `munrdprotect(void *addr, int len)` para marcar páginas de usuario como "no legibles" (limpiar `PTE_R`) y poder revertir la operación.

Qué ya se implementó
- `kernel/vm.c`: `mrdprotect` y `munrdprotect` (recorren PTEs, validan PTE_V/PTE_U, modifican `PTE_R`, llaman a `sfence_vma()`).
- `kernel/vm.h`: prototipos añadidos.
- `kernel/sysproc.c`: syscalls `sys_mrdprotect` y `sys_munrdprotect`.
- `kernel/syscall.c` y `kernel/syscall.h`: registro y corrección de números de syscall.
- `user/usys.pl` y `user/user.h`: stubs y prototipos de usuario.
- `user/rdprotect_test.c`: test de usuario (usa `fork()`/`wait`).
- `Makefile`: `_rdprotect_test` añadido a `UPROGS`.
- `Dockerfile`: imagen para compilar/ejecutar con toolchain RISC-V + QEMU.

Qué falta / problemas detectados
- `exec _rdprotect_test failed` en el shell de xv6: se corrigió la numeración de syscalls, y se añadieron diagnósticos en `kernel/exec.c`; ejecutar el test y verificar salida.
- Bloqueo de `fs.img` ("Failed to get \"write\" lock"): asegurarse de que no haya otra instancia de QEMU usando `fs.img` (usar `ps`/`pkill`).
- Confirmar en runtime que `mrdprotect` realmente impide lecturas: si el test no provoca la terminación esperada, instrumentar `trap.c` para detectar page faults por `PTE_R == 0`.
- Verificar que `user/_rdprotect_test` sea un binario RISC-V (`file user/_rdprotect_test`) y que `user/usys.S` se regenere correctamente.

Cómo reproducir (resumen rápido)
1) (opción recomendada) usar Docker:
```bash
docker build -t xv6-build .
docker run --rm -it -v "$(pwd)":/src -w /src xv6-build bash
export TOOLPREFIX=riscv64-linux-gnu-
make -j4 TOOLPREFIX=$TOOLPREFIX
pkill -f qemu-system-riscv64 || true   # si existe un bloqueo
make qemu TOOLPREFIX=$TOOLPREFIX
```
2) En el prompt de xv6 ejecutar:
```
_rdprotect_test
```
3) Si `exec` falla o el test no se comporta como se espera, pegar aquí la salida del kernel (mensajes `kexec:` o trazas del trap).

Archivos clave para revisar
- `kernel/vm.c`, `kernel/vm.h`
- `kernel/sysproc.c`, `kernel/syscall.c`, `kernel/syscall.h`
- `kernel/exec.c`, `kernel/trap.c` (instrumentar si es necesario)
- `user/usys.pl`, `user/user.h`, `user/rdprotect_test.c`

Siguientes pasos recomendados
- Ejecutar `_rdprotect_test` en QEMU y pegar la salida si falla.
- Si el test no termina como se espera, añadiré instrumentación en `trap.c` para detectar page faults causados por `PTE_R == 0`.

