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
    int byte_index = 0;
    int ignore_bytes_count = 0;

    while (str[byte_index] != 0) {
        int bytes_taken_up = width_from_start_byte(str[byte_index]);
        if (bytes_taken_up > 1) ignore_bytes_count += bytes_taken_up - 1;      
        byte_index++;
    }

    return byte_index - ignore_bytes_count;
}

int32_t codepoint_index_to_byte_index(char str[], int32_t cpi) {
    int byte_index = 0;
    int codepoint_index = 0;

    while (str[byte_index] != 0) {
        if (codepoint_index == cpi) break;

        int bytes_taken_up = width_from_start_byte(str[byte_index]);
        
        byte_index += bytes_taken_up;
        codepoint_index++;
    }

    return byte_index;
}

void utf8_substring(char str[], int32_t cpi_start, int32_t cpi_end, char result[]) {
    int result_index = 0;

    if (cpi_start > cpi_end || cpi_end < 0 || cpi_start < 0) return;

    for (int i=cpi_start; i<cpi_end; i++) {
        int byte_index = codepoint_index_to_byte_index(str, i);
        int bytes_taken_up = width_from_start_byte(str[byte_index]);

        for (int j=0; j<bytes_taken_up; j++) {
            result[result_index] = str[byte_index + j];
            result_index++;
        }
    }

    result[result_index] = 0;
}

int32_t codepoint_at(char str[], int32_t cpi) {
    int byte_index = codepoint_index_to_byte_index(str, cpi);
    int bytes_taken_up = width_from_start_byte(str[byte_index]);

    int code_point = 0;

    if (bytes_taken_up == 1) {
        code_point = str[byte_index];        
    } else if (bytes_taken_up == 2) {
        int first_byte = str[byte_index] & 0b00011111;
        int second_byte = str[byte_index + 1] & 0b00111111;

        code_point = (first_byte << 6) | second_byte;
    } else if (bytes_taken_up == 3) {
        int first_byte = str[byte_index] & 0b00001111;
        int second_byte = str[byte_index + 1] & 0b00111111;
        int third_byte = str[byte_index + 2] & 0b00111111;

        code_point = (first_byte << 12) | (second_byte << 6) | third_byte;
    } else if (bytes_taken_up == 4) {
        int first_byte = str[byte_index] & 0b00000111;
        int second_byte = str[byte_index + 1] & 0b00111111;
        int third_byte = str[byte_index + 2] & 0b00111111;
        int fourth_byte = str[byte_index + 3] & 0b00111111;

        code_point = (first_byte << 18) | (second_byte << 12) | (third_byte << 6) | fourth_byte;
    }

    return code_point;
}

int32_t is_animal_emoji_at(char str[], int32_t cpi) {
    int code_point = codepoint_at(str, cpi);

    return (code_point >= 128000 && code_point <= 128063) || (code_point >= 129408 && code_point <= 129454);  
}

void utf8_analyze() {
    char input[100];
    
    printf("%s", "Enter a UTF-8 encoded string: ");
    fgets(input, 100, stdin);

    printf("Valid ASCII: %s\n", is_ascii(input) ? "true" : "false");
    capitalize_ascii(input);
    printf("Uppercased ASCII: \"%s\"\n", input);
    
    int index = 0;
    while (input[index] != 0) index++;
    printf("Length in bytes: %d\n", index);
    
    printf("Number of code points: %d\n", utf8_strlen(input));


}

int main() {
    utf8_analyze();

    return 0;
}
