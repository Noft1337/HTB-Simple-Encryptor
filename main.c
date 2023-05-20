#include <stdio.h>
#include <stdlib.h>


int get_file_size(FILE* file){
    int size;
    fseek(file, 0, 2);
    size = ftell(file);
    fseek(file, 0, 0);

    return size;
}

unsigned char unshift(unsigned char c, signed char to_shift){
    unsigned char current = c;
    c = current << 8 - to_shift | current >> to_shift;
    return c;
}

void init_rand_int_array(int seed, int* array){
    srand(seed);
    for (int i = 0; i < 100; i+=2){
        array[i] = (unsigned char)rand();
        array[i + 1] = (char)rand() & 7;
    }
    return;
}

int get_seed(FILE *file){ 
    unsigned int final_seed;
    fread(&final_seed, sizeof(unsigned int), 1, file);
    return final_seed;
}

void decode2(unsigned char *content, int size, unsigned int seed){
    unsigned char r1;
    char r2;
    unsigned char current_c;
    printf("Flag: ");
    srand(seed);
    for (int i = 4; i < size; i++){
        r1 = (unsigned char)rand();
        r2 = (char)rand() & 7;
        current_c = unshift(content[i], r2);
        current_c = current_c ^ r1;
        printf("%c", current_c);
    }
    printf("\n");
    return;
}

int main(){

    FILE *inputFile = fopen("flag.enc", "rb");

    if (!inputFile){
        printf("Couldn't open 'flag.enc'\n");
        return -1;
    }

    unsigned int seed = get_seed(inputFile);
    int size = get_file_size(inputFile);
    unsigned char *content = (unsigned char *)malloc(size);
    fread(content, 1, (size_t)size, inputFile);
    fclose(inputFile);
    int rand_nums[100];

    printf("The seed that has been found is: %d\n", seed);
    decode2(content, size, seed);

    return 0;

}