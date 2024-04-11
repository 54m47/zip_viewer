#include <stdlib.h>
#include <string.h>
#include <cstdio>
#include <minizip-ng/unzip.h>
#include "../include/ziputils.h"


bool open_zip_file(const char *zip_file_name, zip_file_info **files, size_t *num_files)
{
    unzFile zip_file = unzOpen(zip_file_name);
    if (zip_file == NULL)
    {
        return false;
    }

    unz_global_info global_info;
    if (unzGetGlobalInfo(zip_file, &global_info) != UNZ_OK)
    {
        unzClose(zip_file);
        return false;
    }

    *num_files = global_info.number_entry;
    *files = (zip_file_info *)malloc(sizeof(zip_file_info) * (*num_files));
    if (*files == NULL)
    {
        unzClose(zip_file);
        return false;
    }

    for (unsigned long i = 0; i < *num_files; ++i)
    {
        unz_file_info file_info;
        char file_name[256];
        if (unzGetCurrentFileInfo(zip_file, &file_info, file_name, sizeof(file_name), NULL, 0, NULL, 0) != UNZ_OK)
        {
            free_zip_file_info_array(*files, i);
            unzClose(zip_file);
            return false;
        }

        (*files)[i].file_name = strdup(file_name);
        (*files)[i].uncompressed_size = file_info.uncompressed_size;
        (*files)[i].compressed_size = file_info.compressed_size;

        if (i + 1 < *num_files)
        {
            if (unzGoToNextFile(zip_file) != UNZ_OK)
            {
                free_zip_file_info_array(*files, i + 1);
                unzClose(zip_file);
                return false;
            }
        }
    }

    unzClose(zip_file);
    return true;
}

bool extract_file(const char *zip_file_name, const char *file_name, const char *output_path)
{
    unzFile zip_file = unzOpen(zip_file_name);
    if (zip_file == NULL)
    {
        return false;
    }

    if (unzLocateFile(zip_file, file_name, 0) != UNZ_OK)
    {
        unzClose(zip_file);
        return false;
    }

    if (unzOpenCurrentFile(zip_file) != UNZ_OK)
    {
        unzClose(zip_file);
        return false;
    }

    char output_file_path[256];
    snprintf(output_file_path, sizeof(output_file_path), "%s/%s", output_path, file_name);

    FILE *output_file = fopen(output_file_path, "wb");
    if (output_file == NULL)
    {
        unzCloseCurrentFile(zip_file);
        unzClose(zip_file);
        return false;
    }

    char buffer[4096];
    int read_bytes;
    do
    {
        read_bytes = unzReadCurrentFile(zip_file, buffer, sizeof(buffer));
        if (read_bytes > 0)
        {
            fwrite(buffer, 1, read_bytes, output_file);
        }
    } while (read_bytes > 0);

    fclose(output_file);
    unzCloseCurrentFile(zip_file);
    unzClose(zip_file);

    return true;
}

void free_zip_file_info_array(zip_file_info *files, size_t num_files)
{
    for (size_t i = 0; i < num_files; ++i)
    {
        free(files[i].file_name);
    }
    free(files);
}