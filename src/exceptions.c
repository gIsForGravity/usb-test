/**
 * @file exceptions.c
 * @author Gavin Tantleff
 * @brief implements behavior as described in exceptions.h
 * @version 0.1
 * @date 2023-01-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "exceptions.h"

jmp_buf *ex_exception_buffer = NULL;

void throw(enum ex_exception_type e) {
    longjmp(*ex_exception_buffer, e);
}
