//
// Created by cdeler on 12/18/17.
//

#include <elfutils/libdw.h>
#include <elfutils/libdwfl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <fcntl.h>
#include <dwarf.h>
#include <libgen.h>

#include "ybtl_dwarf.h"
#include "ybtl_types.h"


static char *
_get_exe_path(pid_t pid)
	{
	static char dest[PATH_MAX];

	if (!*dest)
		{
		char path[PATH_MAX];

		memset(dest, 0, sizeof(dest)); // readlink doesn't make null term string

		sprintf(path, "/proc/%d/exe", pid);
		readlink(path, dest, PATH_MAX);
		}

	return dest;
	}

static struct module_data_t
	{
	int fd;
	bool enabled;
	Elf *elf;
	Dwarf *dwarf;
	__pid_t pid;
	} module_data;


#define check_for_null(_rc) if (!(_rc)) break;

static void _used _constructor
_init(void)
	{
	const char *execPath;
	memset(&module_data, 0, sizeof(struct module_data_t));

	do
		{
		if (elf_version(EV_CURRENT) == EV_NONE)
			break;

		module_data.pid = getpid();
		execPath = _get_exe_path(module_data.pid);

		module_data.fd = open(execPath, O_RDONLY);
		check_for_null(module_data.fd);

		module_data.elf = elf_begin(module_data.fd, ELF_C_READ, NULL);
		check_for_null(module_data.elf);

		module_data.dwarf = dwarf_begin_elf(module_data.elf, DWARF_C_READ, NULL);
		check_for_null(module_data.dwarf);

		module_data.enabled = true;
		} while (0);
	}

static void _used _destructor
_finit(void)
	{
	if (module_data.enabled)
		{
		close(module_data.fd);
		elf_end(module_data.elf);
		dwarf_end(module_data.dwarf);
		}
	}

static void *
_get_ip()
	{
	return __builtin_return_address(0) + 4U;
	}

static const char *
_getFunctionName(Dwarf_Die *functionDie, char *buffer, size_t bufferSize)
	{
	Dwarf_Attribute da;

	*buffer = '\0';

	memset(&da, 0, sizeof(Dwarf_Attribute));
	dwarf_attr_integrate(functionDie, DW_AT_name, &da);
	const char *functionName = dwarf_formstring(&da);

	if (functionName)
		strncpy(buffer, functionName, bufferSize);

	return buffer;
	}

const char *
_getFnameById(Dwarf_Die *inputDie, size_t idx)
	{
	Dwarf_Die cuDie;
	Dwarf_Files *files;

	if (!dwarf_diecu(inputDie, &cuDie, NULL, NULL) ||
		dwarf_getsrcfiles(&cuDie, &files, NULL) != 0)
		return NULL;

	return dwarf_filesrc(files, idx, NULL, NULL);
	}

static const char *
_getSourceFileName(Dwarf_Die *functionDie, char *buffer, size_t bufferSize)
	{
	Dwarf_Attribute da;
	size_t fileIdx;

	*buffer = '\0';
	memset(&da, 0, sizeof(Dwarf_Attribute));

	dwarf_attr_integrate(functionDie, DW_AT_decl_file, &da);
	dwarf_formudata(&da, &fileIdx);

	const char *fname = _getFnameById(functionDie, fileIdx);

	if (fname)
		strncpy(buffer, fname, bufferSize);

	return fname;
	}

static size_t
_functionLine(Dwarf_Die *functionDie)
	{
	Dwarf_Attribute da;
	size_t result = 0U;

	memset(&da, 0, sizeof(Dwarf_Attribute));
	dwarf_attr_integrate(functionDie, DW_AT_decl_line, &da);

	dwarf_formudata(&da, &result);

	return result;
	}

static void
_handleDwarfFunction(Dwarf_Die *functionDie)
	{
	char functionName[256];
	char fileName[256];

	size_t functionLine = _functionLine(functionDie);
	_getSourceFileName(functionDie, fileName, sizeof(fileName));
	_getFunctionName(functionDie, functionName, sizeof(functionName));

	if (*functionName && *fileName && functionLine)
		{
		printf("Dwarf function %s, %s:%lu\n", functionName, basename(fileName), functionLine);
		}
	}

static void
_readDwarfData()
	{
	Dwarf_Off offset = 0U, lastOffset = 0U;
	size_t hdrSize = 0U;

	while (dwarf_nextcu(module_data.dwarf, offset, &offset, &hdrSize, 0, 0, 0) == 0)
		{
		Dwarf_Die childDie, cuDie;
		if (dwarf_offdie(module_data.dwarf, lastOffset + hdrSize, &cuDie) == NULL)
			break;
		lastOffset = offset;

		if (dwarf_child(&cuDie, &childDie) != 0)
			continue;

		do
			{
			switch (dwarf_tag(&childDie))
				{
				case DW_TAG_entry_point:
				case DW_TAG_inlined_subroutine:
				case DW_TAG_subprogram:
					_handleDwarfFunction(&childDie);
					break;
				default:
					break;
				}
			} while (dwarf_siblingof(&childDie, &childDie) == 0);

		}
	}

bool
ybtl_is_dwarf_enabled()
	{
	return module_data.enabled;
	}

void
ybtl_test_dwarf()
	{
	_readDwarfData();
	}