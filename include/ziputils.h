#ifndef ZIPUTILS_H
#define ZIPUTILS_H

#include <stdbool.h>
#include <stddef.h>

typedef struct
{
    char *file_name;
    size_t uncompressed_size;
    size_t compressed_size;
} zip_file_info;

bool open_zip_file(const char *zip_file_name, zip_file_info **files, size_t *num_files);
bool extract_file(const char *zip_file_name, const char *file_name, const char *output_path);
void free_zip_file_info_array(zip_file_info *files, size_t num_files);

#endif