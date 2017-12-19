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

static struct module_data_t
{
int fd;
bool enabled;
Elf *elf;
Dwfl *dwfl;
Dwarf *dwarf;
__pid_t pid;
} module_data;

static void _used _constructor
_init(void)
	{
	memset(&module_data, 0, sizeof(struct module_data_t));

	// malloc here
	static char *debuginfo_path;
	static const Dwfl_Callbacks proc_callbacks =
			{
					.find_debuginfo = dwfl_standard_find_debuginfo,
					.debuginfo_path = &debuginfo_path,
					.find_elf = dwfl_linux_proc_find_elf,
					.section_address = dwfl_offline_section_address
			};


	module_data.dwfl = dwfl_begin(&proc_callbacks);

	module_data.enabled = (module_data.dwfl != NULL);

	if (module_data.enabled)
		{
		module_data.pid = getpid();
		module_data.fd = open(_get_exe_path(), O_RDONLY);
		module_data.elf = elf_begin(module_data.fd, ELF_C_READ, NULL);
		module_data.dwarf = dwarf_begin_elf(module_data.elf, DWARF_C_READ, NULL);
		}
	}

static void _used _destructor
_finit(void)
	{
	if (module_data.enabled)
		{
		close(module_data.fd);
		elf_end(module_data.elf);
		dwarf_end(module_data.dwarf);
		dwfl_end(module_data.dwfl);
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

extern void
_lookupLocation()
	{
	Dwarf_Off offset = 0U, lastOffset = 0U;
	size_t hdrSize = 0U;

	while (dwarf_nextcu(module_data.dwarf, offset, &offset, &hdrSize, 0, 0, 0) == 0)
		{
		Dwarf_Die result, cu_die;
		if (dwarf_offdie(module_data.dwarf, lastOffset + hdrSize, &cu_die) == NULL)
			break;
		lastOffset = offset;

		if (dwarf_child(&cu_die, &result) != 0)
			continue;

		do
			{
			switch (dwarf_tag(&result))
				{
				case DW_TAG_entry_point:
				case DW_TAG_inlined_subroutine:
				case DW_TAG_subprogram:
					_handleDwarfFunction(&result);
					break;
				default:
					break;
				}
			} while (dwarf_siblingof(&result, &result) == 0);

		}
	}


void
ybtl_test_dwarf()
	{
	_lookupLocation();
	}