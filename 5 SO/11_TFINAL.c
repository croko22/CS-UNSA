#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>

int main()
{
    DIR *dirp;
    struct dirent *dp;

    dirp = opendir("/proc");
    if (dirp == NULL)
    {
        perror("Failed to open directory \"/proc\"");
        return 1;
    }

    while ((dp = readdir(dirp)) != NULL)
    {
        if (dp->d_type == DT_DIR && atoi(dp->d_name) > 0)
        {
            char stat_path[256];
            sprintf(stat_path, "/proc/%s/stat", dp->d_name);

            FILE *fp = fopen(stat_path, "r");
            if (fp == NULL)
            {
                perror("Failed to open stat file");
                continue;
            }

            int pid;
            char comm[256];
            char state;
            fscanf(fp, "%d %s %c", &pid, comm, &state);

            printf("PID: %d, Command: %s, State: %c\n", pid, comm, state);

            fclose(fp);
        }
    }

    closedir(dirp);

    return 0;
}