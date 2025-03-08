[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/FLG6_3H5)

- Wanessa Koserski dos Santos

## Observações

Este código converte um arquivo de texto com instruções e valores em um arquivo binário .mem. O arquivo de texto contém duas seções:

- .values: Define valores em posições de memória.

- .steps: Define instruções como LDA, ADD, STA, HLT, etc.

O que o código faz:
- Lê o arquivo de texto e processa as seções .values e .steps.

- Gera um arquivo binário com cabeçalho 0x03 0x4E 0x44 0x52 e o conteúdo da memória.

Compilar:
- gcc -o assembler assembler.c
- ./assembler assembly.txt binary.mem
