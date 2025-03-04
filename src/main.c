#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/syslimits.h>
#include <unistd.h>

struct Files {
    char* file;
};

int init_directory(const char** dir_path) {
    struct stat s;
    if (stat(*dir_path, &s) == 0) {
        if (S_ISDIR(s.st_mode)) {
            rmdir(*dir_path);
            mkdir(*dir_path, 0755);
        } else {
            printf("Path exists but is not directory.\n");
            return 0;
        }
    } else {
        if (errno == ENOENT) {
            mkdir(*dir_path, 0755);
        } else {
            perror("Failed to stat.\n");
            return 0;
        }
    }
    return 1;
}

int populate_source(const char** dir_path, const char* source_file_names[], int* file_names_length) {
    int is_ok = EXIT_SUCCESS;
    for (int i = 0; i < *file_names_length; i++) {
        size_t path_length = strlen(*dir_path) + strlen(source_file_names[i]);

        char* file_path = malloc(path_length);
        if (file_path == NULL) {
            fprintf(stderr, "Error: Memory allocation failed for %s\n", source_file_names[i]);
            is_ok = EXIT_FAILURE;
            continue;
        }

        snprintf(file_path, path_length, "%s/%s", *dir_path, source_file_names[i]);
        FILE* fp = fopen(file_path, "w+");
        if (!fp) {
            perror("File opening failed.\n");
            is_ok = EXIT_FAILURE;
            continue;
        }
        fputs("test_string\n", fp);
        rewind(fp);

        int c;
        while ((c = fgetc(fp)) != EOF) {
            putchar(c);
        }

        if (ferror(fp)) {
            puts("I/O error when reading.");
        } else if (feof(fp)) {
            puts("End of file successfully reached.");
        }

        fclose(fp);
        free(file_path);
    }

    if (is_ok == EXIT_FAILURE) {
        fprintf(stderr, "Memory allocations failed.\n");
    }
    return 0;
}

int main() {
    const char* SOURCEDIRECTORY = "/Users/tymalik/Docs/Git/construct/c_construct/src/data";
    const char* DESTDIRECTORY = "/Users/tymalik/Docs/Git/construct/c_construct/src/dest";
    int is_source_initialized = init_directory(&SOURCEDIRECTORY);
    assert((is_source_initialized == 1) && "Source must be initialized.");
    int is_dest_initialized = init_directory(&DESTDIRECTORY);
    assert((is_dest_initialized == 1) && "Dest must be initialized.");

    const char* filenames[] = {
        "one.txt", "two.txt", "three.txt", "four.txt", "five.txt",
        "six.txt", "seven.txt", "eight.txt", "nine.txt", "ten.txt",
        "eleven.txt", "twelve.txt", "thirteen.txt", "fourteen.txt", "fifteen.txt",
        "sixteen.txt", "seventeen.txt", "eighteen.txt", "nineteen.txt", "twenty.txt",
        "twenty_one.txt", "twenty_two.txt", "twenty_three.txt", "twenty_four.txt", "twenty_five.txt",
        "twenty_six.txt", "twenty_seven.txt", "twenty_eight.txt", "twenty_nine.txt", "thirty.txt",
        "thirty_one.txt", "thirty_two.txt", "thirty_three.txt", "thirty_four.txt", "thirty_five.txt",
        "thirty_six.txt", "thirty_seven.txt", "thirty_eight.txt", "thirty_nine.txt", "forty.txt",
        "forty_one.txt", "forty_two.txt", "forty_three.txt", "forty_four.txt", "forty_five.txt",
        "forty_six.txt", "forty_seven.txt", "forty_eight.txt", "forty_nine.txt", "fifty.txt"
    };
    int filenames_length = sizeof(filenames) / sizeof(filenames[0]);
    assert(filenames_length > 0 && "filenames cannot be empty");
    populate_source(&SOURCEDIRECTORY, filenames, &filenames_length);

    return 0;
}
