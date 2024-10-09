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

int32_t width_from_start_byte(char start_byte) {
    unsigned char unsigned_start_byte = start_byte;
    
    unsigned int start_masks[] = {0b1000000, 0b11100000, 0b11110000, 0b11111000};
    unsigned int start_patterns[] = {0b0000000, 0b11000000, 0b11100000, 0b11110000};

    for (int i=0; i<4; i++) {
        if ((start_byte & start_masks[i]) == start_patterns[i]) return i+1;
    }

    if (unsigned_start_byte < 128) {
        return 1;
    }
    
    return -1;
}

int32_t utf8_strlen(char str[]) {
    int index = 0;
    int ignore_bytes_count = 0;

    while (str[index] != 0) {
        int bytes_taken_up = width_from_start_byte(str[index]);
        printf("%d ", bytes_taken_up);
        if (bytes_taken_up > 1) ignore_bytes_count += bytes_taken_up - 1;      
        index++;
    }

    return index - ignore_bytes_count;
}

int32_t codepoint_index_to_byte_index(char str[], int32_t cpi) {
    int index = 0;
    int bytes_count = -1;

    while (str[index] != 0) {
        if (index >= cpi+1) break;
        int bytes_taken_up = width_from_start_byte(str[index]);
        if (bytes_taken_up >= 1) bytes_count += bytes_taken_up;
        index++;
    }

    return bytes_count;
}

void utf8_substring(char str[], int32_t cpi_start, int32_t cpi_end, char result[]) {
    int result_index = 0;
    int byte_index = 0;

    for (int i=cpi_start; i<cpi_end; i++) {
        int char_width = width_from_start_byte(str[i]);
        int byte_index = codepoint_index_to_byte_index(str, i);

        for (int j=0; j<char_width; j++) {
            result[result_index] = str[byte_index - char_width + 1];
            result_index++;
        }
    }

    result[result_index] = '\0';
}

int main() {
    char result[17];
    char str[] = {"🦀🦮🦮🦀🦀🦮🦮"};
    utf8_substring(str, 3, 7, result);
    printf("String: %s\nSubstring: %s", str, result); // these emoji are 4 bytes long

    return 0;
}