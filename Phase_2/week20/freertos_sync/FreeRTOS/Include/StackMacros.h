/*
 * File:    StackMacros.h
 * Author:  Alexander130892
 * Date:    24-5-2026
 *
 * Description:
 *   This file is a deprecated wrapper that redirects includes from the
 *   old filename `StackMacros.h` to the new filename `stack_macros.h`
 *   while issuing a compiler warning (on non-MSVC compilers) to notify
 *   developers to update their code.
 */
#ifndef _MSC_VER /* Visual Studio doesn't support #warning. */
    #warning The name of this file has changed to stack_macros.h.  Please update your code accordingly.  This source file (which has the original name) will be removed in a future release.
#endif

#include "stack_macros.h"
