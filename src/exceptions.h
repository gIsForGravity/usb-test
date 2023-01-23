/**
 * @file exceptions.h
 * @author Gavin Tantleff
 * @brief Provides support for exceptions using setjmp and lngjmp
 * @version 0.1
 * @date 2023-01-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <setjmp.h>

#ifdef _cplusplus
extern "C" {
#endif

/* enums */
typedef enum ex_exception_type {
    EXCEPTION_GENERIC = 1,
    EXCEPTION_USB_GENERIC,
    EXCEPTION_USB_CONFIGURATION_BUFFER_TOO_SMALL,
} exception_type_t;

/* extern variables */
extern jmp_buf *ex_exception_buffer;

/* Function macros */
#define try(TRY, EXCEPTION_VAR_NAME, CATCH) do { \
    jmp_buf _try_buffer; \
    jmp_buf* _try_old_buffer = ex_exception_buffer; \
    ex_exception_buffer = &_try_buffer; \
    int _try_setjmp_return; \
    if (0 == (_try_setjmp_return = setjmp(_try_buffer))) { \
        do TRY while (0); \
    } else { \
        ex_exception_buffer = _try_old_buffer; \
        exception_type_t EXCEPTION_VAR_NAME = _try_setjmp_return; \
        do CATCH while (0); \
    } \
    ex_exception_buffer = _try_old_buffer; \
} while (0);

void throw(enum ex_exception_type) __attribute__((noreturn));

#ifdef _cplusplus
}
#endif

#endif
