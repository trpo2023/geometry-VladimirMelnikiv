#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 100

void handle_error(const char* error_msg, const char* input, int pos)
{
    fprintf(stderr, "%s\n", input);
    for (int i = 0; i < pos; i++) {
        fprintf(stderr, " ");
    }
    fprintf(stderr, "^\n");
    fprintf(stderr, "Error at column %d: %s\n", pos, error_msg);
}

int check_circle(char* input)
{
    const char* prefix = "circle(";
    char* start_ptr = input;
    int prefix_len = strlen(prefix);
    if (strncmp(input, prefix, prefix_len)) {
        handle_error("expected '('", start_ptr, 6);
        return -1;
    }
    input += prefix_len;
    char* end_ptr;
    double x = strtod(input, &end_ptr);
    if (end_ptr == input || *end_ptr != ' ') {
        handle_error("expected '<double>'", start_ptr, end_ptr - start_ptr);
        return -1;
    }
    input = end_ptr + 1;
    double y = strtod(input, &end_ptr);
    if (end_ptr == input || *end_ptr != ',') {
        handle_error("expected ','", start_ptr, end_ptr - start_ptr);
        return -1;
    }
    input = end_ptr + 1;
    double radius = strtod(input, &end_ptr);
    if (end_ptr == input || *end_ptr != ')') {
        handle_error("expected ')'", start_ptr, end_ptr - start_ptr);
        return -1;
    }
    end_ptr += 1;
    if (*end_ptr != '\0') {
        handle_error("unexpected token", start_ptr, end_ptr - start_ptr);
        return -1;
    }
    printf("circle(%.1f %.1f, %.1f)\n", x, y, radius);

    const char* output_f = "OutGeometry.txt";
    FILE* file = fopen(output_f, "w");
    if (file == NULL) {
        printf("Error: can't create output file:\n");
        printf("%s\n", output_f);
        return -1;
    }
    fprintf(file, "circle(%.1f %.1f, %.1f)\n", x, y, radius);
    fclose(file);
    return 0;
}

int check_input(const char* input)
{
    if (strncmp(input, "circle", 6) == 0) {
        return check_circle((char*)input);
    }
    handle_error("expected 'circle'", input, 0);
    return -1;
}
int main(int argv, char** argc)
{
    const char* input_f = "InpGeometry.txt";
    FILE* file = fopen(input_f, "r");
    if (file == NULL) {
        printf("Error: can't open input file:\n");
        printf("%s\n", input_f);
        return 1;
    }
    char input[N];
    while (!feof(file)) {
        fgets(input, N, file);
        input[strcspn(input, "\n")] = '\0';
        if (check_input(input)) {
            return 1;
        }
    }
    fclose(file);
    return 0;
}
