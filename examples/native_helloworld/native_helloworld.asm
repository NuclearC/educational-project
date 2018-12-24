
section .data
  helloWorld: db 'Hello World!', 10
  helloWorldLen: equ $-helloWorld

section .text
  global _start
  global _exit

_exit:
  mov eax, 1 ; exit system call
  mov ebx, 0 ; exit with error code 0
  int 80h ; call the kernel

_start:
  mov eax, 4 ; 'write' system call
  mov ebx, 1 ; stdout file descriptor
  mov ecx, helloWorld ; string to write
  mov edx, helloWorldLen ; length of the string to write
  int 80h ; call the kernel

  jmp _exit


