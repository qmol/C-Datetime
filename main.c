#include "hash_set.h"
#include "datetime_util.h"

#define MAX_LINE_LENGTH 256

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input_file> <output_stream>\n", argv[0]);
        return 1;
    }
    
    FILE* input_stream = fopen(argv[1], "r");
    if (!input_stream) {
        printf("Error: Cannot open input file '%s'\n", argv[1]);
        return 1;
    }
    
    hash_set* dt_hset = hash_set_create();

    if (!dt_hset) {
        printf("Error: Memory allocation failed\n");
        fclose(input_stream);
        return 1;
    }
    
    char line[MAX_LINE_LENGTH];
    char dt_str_buffer[MAX_LINE_LENGTH];
    char dt_str_norm[25];
    printf("Processing datetime values...\n");
    while (fgets(line, MAX_LINE_LENGTH-1, input_stream)) {
        sscanf(line, "%255s", dt_str_buffer);

        FunctionStatus rstat = normalize_iso8601(dt_str_buffer, dt_str_norm);
        if (rstat == RET_SUCCESS) {
            hash_set_insert(dt_hset, dt_str_norm);
        } else {
            printf("Warning: Invalid datetime format '%s' (error code: %d)\n", dt_str_buffer, rstat);
        }

    }
    printf("\n\nUnique valid datetime values:\n");
    fclose(input_stream);
    
    // Write unique valid datetime values to output file
    FILE* output_stream = fopen(argv[2], "w");
    if (!output_stream) {
        printf("Error: Cannot create output file '%s'\n", argv[2]);
        hash_set_destroy(dt_hset);
        return 1;
    }

    hash_set_print(output_stream, dt_hset);
    fclose(output_stream);
    
    hash_set_destroy(dt_hset);
    
    return 0;
}
