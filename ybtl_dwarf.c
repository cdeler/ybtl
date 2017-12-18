//
// Created by cdeler on 12/18/17.
//

#include <libdwarf.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <fcntl.h>

#include "ybtl_dwarf.h"

static char *
_get_exe_path()
    {
    static char dest[PATH_MAX];

    if (!*dest)
        {
        char path[PATH_MAX];

        memset(dest, 0, sizeof(dest)); // readlink doesn't make null term string

        pid_t pid = getpid();
        sprintf(path, "/proc/%d/exe", pid);
        readlink(path, dest, PATH_MAX);
        }

    return dest;
    }

void
ybtl_test_dwarf()
    {
    int fd = -1;
    Dwarf_Error err;
    Dwarf_Debug dbg;
    Dwarf_Handler errhand = 0;
    Dwarf_Ptr errarg = 0;
    Dwarf_Error error;
    int rc;

    do
        {
        fd = open(_get_exe_path(), O_RDONLY);

        if (!fd)
            break;

        rc = dwarf_init(fd, DW_DLC_READ, errhand, errarg, &dbg, &error); // ? == DW_DLV_OK

        rc = dwarf_finish(dbg, &error);

        } while (0);

    if (fd)
        close(fd);
    }