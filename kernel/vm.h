#define SBRK_EAGER 1
#define SBRK_LAZY  2

// Protecciones de lectura: marcar/regenerar permiso de lectura en páginas de usuario
int mrdprotect(void *addr, int len);
int munrdprotect(void *addr, int len);
