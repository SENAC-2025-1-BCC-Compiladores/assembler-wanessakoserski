#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


typedef enum {
  NOP = 0x00,
  STA = 0x10,
  LDA = 0x20,
  ADD = 0x30,
  OR = 0x40,
  AND = 0x50,
  NOT = 0x60,
  JMP = 0x80,
  JN = 0x90,
  JZ = 0xA0,
  HLT = 0xF0
} Operation;

typedef enum {
  SECTION_NONE = 0,   
  SECTION_VALUES = 1, 
  SECTION_STEPS = 2   
} SectionState;


void write_binary_file(const char *filename, uint8_t *memory) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Failed to open file for writing");
        exit(EXIT_FAILURE);
    }

    // write the header (0x03 0x4E 0x44 0x52)
    uint8_t header[] = {0x03, 0x4E, 0x44, 0x52};
    fwrite(header, sizeof(uint8_t), 4, file);

    // writes the contents of memory (512 bytes, since each byte is followed by 00)
    fwrite(memory, sizeof(uint8_t), 512, file);

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Expected: %s <input_file.txt> <output_file.mem>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *input_filename = argv[1];
    const char *output_filename = argv[2];

    FILE *file = fopen(input_filename, "r");
    if (!file) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    // initialize memory with zeros
    uint8_t memory[512] = {0}; 
    char line[256];
    SectionState section = SECTION_NONE;
    int pc = 0;      

    while (fgets(line, sizeof(line), file)) {
        // remove \n in the end of the line
        line[strcspn(line, "\n")] = '\0';

        // checks if the line indicates a section
        if (strstr(line, ".values")) {
            section = SECTION_VALUES;
            continue;
        } else if (strstr(line, ".steps")) {
            section = SECTION_STEPS; 
            pc = 0;     
            continue;
        }


        if (section == SECTION_VALUES) {
            int address, value;
            if (sscanf(line, "SET %d, %d", &address, &value) == 2) {
                if (address >= 0 && address < 256) {
                    memory[address * 2] = (uint8_t)value; 
                    memory[address * 2 + 1] = 0x00;       
                }
            }
        }


        else if (section == SECTION_STEPS) {
            char opcode[4];
            int address;
            if (sscanf(line, "%3s %d", opcode, &address) == 2) {
                Operation op;
                if (strcmp(opcode, "NOP") == 0) {
                    op = NOP;
                } else if (strcmp(opcode, "STA") == 0) {
                    op = STA;
                } else if (strcmp(opcode, "LDA") == 0) {
                    op = LDA;
                } else if (strcmp(opcode, "ADD") == 0) {
                    op = ADD;
                } else if (strcmp(opcode, "OR") == 0) {
                    op = OR;
                } else if (strcmp(opcode, "AND") == 0) {
                    op = AND;
                } else if (strcmp(opcode, "NOT") == 0) {
                    op = NOT;
                } else if (strcmp(opcode, "JMP") == 0) {
                    op = JMP;
                } else if (strcmp(opcode, "JN") == 0) {
                    op = JN;
                } else if (strcmp(opcode, "JZ") == 0) {
                    op = JZ;
                } else if (strcmp(opcode, "HLT") == 0) {
                    op = HLT;
                } else {
                    continue;
                }

                memory[pc++] = (uint8_t)op;
                memory[pc++] = 0x00; 

                // operation is not NOP, NOT or HLT, store the address
                if (op != NOP && op != NOT && op != HLT) {
                    memory[pc++] = (uint8_t)address;
                    memory[pc++] = 0x00; 
                }
            } else if (strcmp(line, "HLT") == 0) {
                memory[pc++] = (uint8_t)HLT;
                memory[pc++] = 0x00; 
            }
        }
    }
    

    fclose(file);

    write_binary_file(output_filename, memory);

    printf("Binary file generated successfully: %s\n", output_filename);

    return EXIT_SUCCESS;
}
