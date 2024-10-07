#include <stdio.h>
#include <stdint.h>

int32_t is_ascii(char str[]) {
    int index = 0;
    
    while (str[index] != 0) {
        unsigned char current_char = str[index];
        if (current_char > 127) return 0;
        index++;
    }
    
    return 1;
}

int32_t capitalize_ascii(char str[]) {
    int index = 0;
    int updated_count = 0;

    while (str[index] != 0) {
        if (str[index] >= 97 && str[index] <= 122) {
            str[index] -= 32;
            updated_count++;
        }
        index++;
    }

    return updated_count;
}



int main() {



    return 0;
}