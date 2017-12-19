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
#include "ybtl_types.h"

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

static void
read_cu_list(Dwarf_Debug dbg)
    {

    }

static struct module_data_t
    {
    int fd;
    Dwarf_Debug dbg;
    Dwarf_Handler errhand;
    Dwarf_Ptr errarg;
    Dwarf_Error error;
    } module_data;

static void _used _constructor
_init(void)
    {
    memset(&module_data, 0, sizeof(struct module_data_t));

    module_data.fd = open(_get_exe_path(), O_RDONLY);

    // malloc here
    dwarf_init(module_data.fd, DW_DLC_READ, module_data.errhand, module_data.errarg, &module_data.dbg,
               &module_data.error);
    }

static void _used _destructor
_finit(void)
    {
    close(module_data.fd);
    dwarf_finish(module_data.dbg, &module_data.error);
    }

void
ybtl_test_dwarf()
    {

    }