//
// Created by cdeler on 12/18/17.
//

#pragma once
#ifndef YBTL_YBTL_TYPES_H
#define YBTL_YBTL_TYPES_H

/*
 * ISO/IEC 9899:2011, section §5.2.4.1 Translation limits said
	— 63 significant initial characters in an internal identifier or a macro name
        (each universal character name or
        extended source character is considered a single character)
	— 31 significant initial characters in an external identifier
        (each universal character name specifying a short identifier
        of 0000FFFF or less is considered 6 characters,
        each universal character name specifying
        a short identifier of 00010000 or more is considered 10 characters,
        and each extended source character is considered the same number
        of characters as the corresponding universal character name, if any)
 */
#define STACK_WALKER_IDENTEFER_NAME_MAX_LENGTH (64U)
#define STACK_WALKER_MAX_DEPTH (40U)

#define _unused __attribute__((unused))
#define _used __attribute__((used))
#define _constructor __attribute__((constructor))


#endif //YBTL_YBTL_TYPES_H
