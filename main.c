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

unsigned char decode_char(char c, int* rands, int position){
    int rand1 = (unsigned char)rands[position];
    int rand2 = (char)rands[position + 1];
    c = unshift(c, rand2);
    c = c ^ rand1;
    return c;
}

void decode_string(unsigned char *content, int size, int *rands, int start_pos){
    int rand1, rand2;
    int j = 0;
    for (int i = start_pos; i < size; i++){
        content[i] = decode_char(content[i], rands, j);
        printf("%c", content[i]);
        j += 2;
    }
    printf("\n");
    return;
}

void init_rand_int_array(int seed, int* array){
    srand(seed);
    for (int i = 0; i < 100; i+=2){
        array[i] = (unsigned char)rand();
        array[i + 1] = (char)rand() & 7;
    }
    return;
}

int validate_decoding(char c_to_decode, char expected, int *randoms, int position){
    if (decode_char(c_to_decode, randoms, position) == expected){
        return 1;
    }
    else {
        return 0;
    }
}

int get_seed(unsigned char c1, unsigned char c2, unsigned char c3, int seed){
    char decoded_char, decoded_char2;
    int randoms[100];
    unsigned int final_seed = 0;
    for (unsigned int i = seed - 100000; i < seed + 100000; i++){
        srand(i);
        init_rand_int_array(i, randoms);
        if (validate_decoding(c1, 'H', randoms, 0)) {
            if (validate_decoding(c2, 'T', randoms, 2)){
                if (validate_decoding(c3, 'B', randoms, 6)){
                    final_seed = i;
                    break;
                }
            }
        }
    }

    if (final_seed == 0){
        return 0;
    }

    return final_seed;
}

int main(){

    // I think this is the correct seed but it doesn't work for me... 
    int seed = 1658229288;
    int found_seed = 0;
    int start_pos;
    FILE *inputFile = fopen("flag.enc", "rb");
    int size = get_file_size(inputFile);
    unsigned char *content = (unsigned char *)malloc(size);
    fread(content, 1, (size_t)size, inputFile);
    fclose(inputFile);
    int rand_nums[100];

    for (int x = 0; x < size; x++){
        found_seed = get_seed(content[x], content[x + 1], content[x + 2], seed);
        if (found_seed != 0){
            printf("Start pos: %d\n", x);
            start_pos = x;
            break;
        }
    }
    if (found_seed != 0){
        printf("The seed that has been found is: %d\n", found_seed);
        init_rand_int_array(found_seed, rand_nums);

        decode_string(content, size, rand_nums, start_pos);
        return 0;
    } else {
        printf("Found no matching seed...\n");
        return 1;
    }

}