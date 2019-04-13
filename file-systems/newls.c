//
// Created by Louis on 2019-04-12.
//

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <memory.h>

const char * getFileType(int d_type) {
    switch(d_type) {
        case DT_BLK:
            return "b: block device";
        case DT_CHR:
            return "c: character device";
        case DT_DIR:
            return "d: directory";
        case DT_FIFO:
            return "p: pipe";
        case DT_LNK:
            return "l: soft link";
        case DT_REG:
            return "f: regular file";
        case DT_SOCK:
            return "s: domain socket";
        default:
            return "u: unknown";
    }
}

int count(struct dirent *entry , char *path) {
    int file_count = 0;

    char subdir[1024];
    sprintf(subdir, "%s/%s", path, entry->d_name);

    DIR *dir = opendir(subdir);

    // In the case we try to open a restricted dir we
    // we will return to avoid a segmentation fault
    if(dir == NULL){
        return 0;
    }

    struct dirent *newEntry;

    while((newEntry = readdir(dir)) != NULL) {
        if ((!strcmp(newEntry->d_name,".")) | (!strcmp(newEntry->d_name, "..")))
            continue;

        if (newEntry->d_type == DT_REG) {
            file_count++;
        } else if (newEntry->d_type == DT_DIR){
            file_count += count(newEntry, subdir);
        }
    }

    return file_count;
}

int main(int argc, char *argv[]) {
    char *path;

    if (argc < 2) {
        path = "./";
    } else {
        path = argv[1];
    }

    DIR *dir = opendir(path);
    struct dirent *entry;

    int local_file_count = 0;

    printf("%-20s %-25s %-15s %-15s\n", "Name", "Type", "Inode", "Size (Bytes)");
    printf("---------------------------------------------------------------------\n");
    while((entry = readdir(dir)) != NULL) {
        local_file_count++;
        struct stat file_st;
        fstatat(dirfd(dir), entry->d_name, &file_st, 0);

        char fileType[100];

        if (entry->d_type == DT_DIR && strcmp(entry->d_name,".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char numOfFiles[15];
            sprintf(numOfFiles, "(%d Files)", count(entry, path));
            sprintf(fileType, "%s %s", getFileType(entry->d_type), numOfFiles);
        } else {
            sprintf(fileType, "%s", getFileType(entry->d_type));
        }

        printf("%-20s %-25s %-15lu %-15lu\n", entry->d_name, fileType, (unsigned long) entry->d_ino, (unsigned long) file_st.st_size);
    }

    printf("\nFiles in this directory: %d\n", local_file_count);

    return 1;
}