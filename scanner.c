#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define EICAR_SIZE 68

uint8_t eicar_signature[] = {
    0x58, 0x35, 0x4F, 0x21, 0x50, 0x25, 0x40, 0x50, 
    0x5B, 0x34, 0x5C, 0x50, 0x5A, 0x34, 0x5E, 0x40,
    0x5B, 0x35, 0x5E, 0x40, 0x5C, 0x43, 0x5D, 0x34,
    0x5B, 0x5E, 0x58, 0x35, 0x5E, 0x40, 0x5D, 0x5C,
    0x57, 0x35, 0x5C, 0x50, 0x5A, 0x34, 0x5E, 0x40,
    0x5D, 0x5C, 0x57, 0x35, 0x32, 0x24, 0x32, 0x32,
    0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
    0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
    0x32, 0x32, 0x32, 0x32, 0x5E
};

int scan_file(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Nao foi possível abrir o arquivo '%s'\n", filename);
        return -1;
    }
    
    uint8_t buffer[2048];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer), file);
    fclose(file);

    if (bytes_read < EICAR_SIZE) {
        return 0;
    }
    
    for (size_t i=0;i<=bytes_read - EICAR_SIZE;i++) {
        if (memcmp(&buffer[i], eicar_signature, EICAR_SIZE) == 0) {
            printf("Ameaça detectada! Arquivo: %s\n", filename);
            return 1;
        }
    }
   
    return 0;
}

void create_test_file() {
    FILE* file = fopen("eicar_test.txt", "wb");
    if (!file) {
        printf("Erro ao criar arquivo de teste\n");
        return;
    }
    
    fwrite(eicar_signature, 1, EICAR_SIZE, file);
    fclose(file);
    
    printf("Arquivo de teste criado: eicar_test.txt\n");
}

void create_clean_file() {
    FILE* file = fopen("clean_file.txt", "w");
    fprintf(file, "Arquivo limpo para teste.\n");
    fclose(file);
    printf("Arquivo limpo criado: clean_file.txt\n");
}

void run_self_test() {
    printf("Teste automatico\n");
    
    create_test_file();
    create_clean_file();
    
    printf("Testando detecção de EICAR\n");
    int result = scan_file("eicar_test.txt");
    if (result == 1) {
        printf("EICAR detectado!\n");
    } else {
        printf("EICAR nao detectado\n");
    }
    
    printf("Testando arquivo limpo\n");
    result = scan_file("clean_file.txt");
    if (result == 0) {
        printf("Arquivo limpo ok\n");
    } else {
        printf("Falso positivo\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc<2) {
        printf("Uso: %s test\n", argv[0]);
        return 1;
    }
    
    if (strcmp(argv[1], "test")==0) {
        run_self_test();
    }
    
    return 0;
}
