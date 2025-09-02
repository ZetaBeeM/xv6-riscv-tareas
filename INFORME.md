Para instalar INFORME.md, tuve que seguir las siguientes instrucciones:

1) Debido a que ocupo Windows 10 en mi computador desktop, lo primero que tuve que hacer fue instalar WSL.
Aquí me encontré con el primer problema: Al ocupar el comando "wsl --install" en Powershell, se descargaba el distro default (Ubuntu), pero no se lograba instalar.
El error que daba era "Error code: Wsl/InstallDistro/Service/RegisterDistro/CreateVm/HCS/HCS_E_HYPERV_NOT_INSTALLED".
Al investigar este error, descubrí que tenía que ver con no tener virtualización de hardware activada, así que después de un reinicio donde entré al BIOS a activar la opción, funcionó perfecto.

2) Abrí WSL, y configuré el nombre de usuario y contraseña de Ubuntu. Luego, instalé las dependencias necesarias para el uso de QEMU.
Esto consistió en ocupar el comando sudo apt-get install para make, qemu-system-misc, bc, y gcc-riscv-linux-gnu. También instalé Git y lo configuré con mi cuenta de Github, creando un token de acceso con los permisos necesarios.

3) Creé un fork de XV6 en un repositorio llamado xv6-riscv-tareas, y luego cloné este dentro de mi instalación de Ubuntu.

4) Ejecuté el comando "make QEMU", que inició una instancia de XV6.

5) Ejecuté los siguentes comandos dentro de XV6:
"ls", que mostró una lista de los archivos.
"echo "Hola xv6"", que imprimió ese mismo mensaje en la terminal.
"cat README", que imprimió el README asociado al sistema operativo en la terminal.
El funcionamiento correcto de estos comandos me indicó que habia hecho todo correctamente.

6) Luego de hacer esto, cerré la instancia de XV6 con ctrl+a y luego x.

7) Creé un branch dentro de mi fork llamado Tarea0, y hice un commit y push.
