#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

// TODO: Take care of space complexity in regards of searched filesystem size...

char* gnu_getcwd ()
{
  size_t size = 100;

  while (1)
    {
      char *buffer = (char *) malloc (size);
      if (getcwd (buffer, size) == buffer)
        return buffer;
      free (buffer);
      if (errno != ERANGE)
        return 0;
      size *= 2;
    }
}

void search(const char* start_dir, const char* target_file_name)
{
    DIR* dir_stream = opendir(start_dir);
    if (dir_stream == NULL)
    {
        // Uncomment next line if you want errors printed.
        //printf("Error opening requested directory...\n");
        return;
    }

    struct dirent* curr_dir_entry;

    while ((curr_dir_entry = readdir(dir_stream)))
    {
        // If curr dir entry name is target -> print it
        if (strcmp(curr_dir_entry->d_name, target_file_name) == 0)
        {
           printf("%s/%s\n", start_dir ,curr_dir_entry->d_name);
        }

        // If curr dir entry is DT_DIR -> recursive search inside
        if (curr_dir_entry->d_type == DT_DIR)
        {
            size_t curr_dir_name_len = strlen(curr_dir_entry->d_name) + 1;
            char* curr_dir_name = calloc(curr_dir_name_len, sizeof(*curr_dir_name));
            snprintf(curr_dir_name, curr_dir_name_len, "%s", curr_dir_entry->d_name);
            if (strcmp(curr_dir_name, ".") != 0 && strcmp(curr_dir_name, "..") != 0)
            {
                size_t total_len = strlen(start_dir) + strlen(curr_dir_name) + 2;
                char* next_start_dir = calloc(total_len,sizeof(*next_start_dir));
                snprintf(next_start_dir, total_len, "%s/%s", start_dir, curr_dir_name);

                search(next_start_dir, target_file_name);
                free(next_start_dir);
            }
            free(curr_dir_name);
        }
    }

    closedir(dir_stream);
}

int main(int argc, char** argv)
{
    if (argc < 2 || argc > 3)
    {
        printf("usage: ./find [target file name]\n");
        printf("       ./find [start directory] [target file name]\n");
        return EXIT_FAILURE;
    }
    if (argc == 2) search(gnu_getcwd(), argv[1]);
    if (argc == 3) search(argv[1], argv[2]);

    return EXIT_SUCCESS;
}
