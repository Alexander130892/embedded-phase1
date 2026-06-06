/*
 * File:    StackMacros.h
 * Author:  Alexander130892
 * Date:    6-6-2026
 *
 * Description:
 *   This file is a deprecated compatibility header that warns users
 *   about a filename change from `StackMacros.h` to `stack_macros.h`
 *   and delegates to the new file. It will be removed in a future
 *   release.
 */
#ifndef _MSC_VER /* Visual Studio doesn't support #warning. */
    #warning The name of this file has changed to stack_macros.h.  Please update your code accordingly.  This source file (which has the original name) will be removed in a future release.
#endif

#include "stack_macros.h"
