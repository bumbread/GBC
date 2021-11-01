call vcvars64

set clang_settings=-march=haswell -maes -O0 -Werror -Wall -Wno-unused-function -Wno-multichar -g -gcodeview -D_CRT_SECURE_NO_WARNINGS

if not exist build\ mkdir build
clang %clang_settings% assembler/main.c -o build/gbc_asm.exe
