/*******************************************************************************
  FILE: dbg.h
  AUTOR: ixotum
  DATE: 2011-08-26 16:26:46
  DESCRIPTION: Defines for debug macrosses
*******************************************************************************/

#ifndef DBG_H
#define DBG_H

#include "stdio.h"

int logprint(const char *string, ...);

#define PRINT_LINE(x, ...) logprint(""x"", ##__VA_ARGS__);
#define PRINT_DBG(x, ...) logprint("[%s:%4u] %s(): "x"\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__);
#define PRINT_ERROR(x, ...) logprint("\033[1;31mERROR\033[0m [%s:%4u] %s(): "x"\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__);perror("perror");
#define PRINT_WARNING(x, ...) logprint("\033[1;33mWARNING\033[0m [%s:%4u] %s(): "x"\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__);
#define PRINT_START(x, ...) logprint("[%s:%4u] ++%s("x")\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__);
#define PRINT_RETURN(x, ...) logprint("[%s:%4u] --%s("x")\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__);

#define DBG_E   0x01
#define DBG_W   0x02
#define DBG_S   0x04
#define DBG_R   0x08
#define DBG_ALL 0x0ff

#define DBG1(x)
#define DBG2(x)
#define DBG3(x)
#define DBGE(x)
#define DBGW(x)
#define DBGS(x)
#define DBGR(x)

#if !defined(NO_DEBUGS)
  #if !defined(MODULE_DBG_LVL)
    #define MODULE_DBG_LVL 0
  #endif
  #if !defined(MODULE_DBG_OPTIONS)
    #define MODULE_DBG_OPTIONS 0
  #endif
  #if MODULE_DBG_LVL > 0
    #undef DBG1
    #define DBG1(x) x
  #endif
  #if MODULE_DBG_LVL > 1
    #undef DBG2
    #define DBG2(x) x
  #endif
  #if MODULE_DBG_LVL > 2
    #undef DBG3
    #define DBG3(x) x
  #endif
  #if MODULE_DBG_OPTIONS > 0
    #if (MODULE_DBG_OPTIONS) & DBG_E
      #undef DBGE
      #define DBGE(x) x
    #endif
    #if (MODULE_DBG_OPTIONS) & DBG_W
      #undef DBGW
      #define DBGW(x) x
    #endif
    #if (MODULE_DBG_OPTIONS) & DBG_S
      #undef DBGS
      #define DBGS(x) x
    #endif
    #if (MODULE_DBG_OPTIONS) & DBG_R
      #undef DBGR
      #define DBGR(x) x
    #endif
  #endif
#endif

#endif // DBG_H
