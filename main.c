#include <stdio.h>
#include <stdlib.h>


unsigned char unshift(unsigned char c, int to_shift){
    unsigned char current = c;
    c = current << 8 - to_shift | current >> to_shift;
    return c;
}


int get_file_size(FILE* file){
    int size;
    fseek(file, 0, 2);
    size = ftell(file);
    fseek(file, 0, 0);

    return size;
}


void decode(unsigned char *content, int size, int seed){
    int rand1, rand2;
    srand(seed);
    for (int i = 0; i < size; i++){
        rand1 = rand();
        content[i] = content[i] ^ (unsigned char)rand1;
        rand2 = rand() & 7;
        content[i] = unshift(content[i], rand2);
        printf("%c", content[i]);
    }
    printf("\n");
    return;
}


int main(){

    int seed = 1658229288;
    FILE *inputFile = fopen("flag.enc", "rb");
    int size = get_file_size(inputFile);
    unsigned char *content = (unsigned char *)malloc(size);
    fread(content, 1, size, inputFile);
    fclose(inputFile);

    decode(content, size, seed);

}