/*
 * File:    newlib-freertos.h
 * Author:  Alexander130892
 * Date:    24-5-2026
 *
 * Description:
 *   This header file provides FreeRTOS integration with newlib's
 *   thread-local storage (TLS) by defining macros that manage per-task
 *   reentrant data structures, enabling thread-safe C runtime library
 *   operations in FreeRTOS environments.
 */
#ifndef INC_NEWLIB_FREERTOS_H
#define INC_NEWLIB_FREERTOS_H

/* Note Newlib support has been included by popular demand, but is not
 * used by the FreeRTOS maintainers themselves.  FreeRTOS is not
 * responsible for resulting newlib operation.  User must be familiar with
 * newlib and must provide system-wide implementations of the necessary
 * stubs. Be warned that (at the time of writing) the current newlib design
 * implements a system-wide malloc() that must be provided with locks.
 *
 * See the third party link http://www.nadler.com/embedded/newlibAndFreeRTOS.html
 * for additional information. */

#include <reent.h>

#define configUSE_C_RUNTIME_TLS_SUPPORT    1

#ifndef configTLS_BLOCK_TYPE
    #define configTLS_BLOCK_TYPE           struct _reent
#endif

#ifndef configINIT_TLS_BLOCK
    #define configINIT_TLS_BLOCK( xTLSBlock, pxTopOfStack )    _REENT_INIT_PTR( &( xTLSBlock ) )
#endif

#ifndef configSET_TLS_BLOCK
    #define configSET_TLS_BLOCK( xTLSBlock )    ( _impure_ptr = &( xTLSBlock ) )
#endif

#ifndef configDEINIT_TLS_BLOCK
    #define configDEINIT_TLS_BLOCK( xTLSBlock )    _reclaim_reent( &( xTLSBlock ) )
#endif

#endif /* INC_NEWLIB_FREERTOS_H */
