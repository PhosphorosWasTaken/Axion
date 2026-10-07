#include "../Include/vfs.h"

// The actual pointer to the file on the storage media
struct Inode {
    uint64_t start_addr; // Address to the start of the contents
    uint64_t end_addr;   // Address to the end of the contents

    const char* name;   // Name of the file

    int type;           // file (0) or folder (1)
}

Inode inode_table[1024]; // List of each files inodes

void read(const char* name) {
    
}
