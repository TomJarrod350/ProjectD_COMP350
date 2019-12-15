bcc -ansi -c -o kernel_c.o kernel.c
as86 kernel.asm -o kernel_asm.o
ld86 -o kernel -d kernel_c.o kernel_asm.o
./loadFile kernel
as86 userlib.asm -o userlib_asm.o
bcc -ansi -c -o shell_c.o shell.
ld86 -o shell -d shell_c.o userlib_asm.o
./loadFile shell
