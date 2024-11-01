/************************************************************************
* lsqlite3                                                              *
* Copyright (C) 2002-2016 Tiago Dionizio, Doug Currie                   *
* All rights reserved.                                                  *
* Author    : Tiago Dionizio <tiago.dionizio@ist.utl.pt>                *
* Author    : Doug Currie <doug.currie@alum.mit.edu>                    *
* Library   : lsqlite3 - an SQLite 3 database binding for Lua 5         *
*                                                                       *
* Permission is hereby granted, free of charge, to any person obtaining *
* a copy of this software and associated documentation files (the       *
* "Software"), to deal in the Software without restriction, including   *
* without limitation the rights to use, copy, modify, merge, publish,   *
* distribute, sublicense, and/or sell copies of the Software, and to    *
* permit persons to whom the Software is furnished to do so, subject to *
* the following conditions:                                             *
*                                                                       *
* The above copyright notice and this permission notice shall be        *
* included in all copies or substantial portions of the Software.       *
*                                                                       *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       *
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    *
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  *
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  *
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     *
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                *
************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define LUA_LIB
#include "lua.h"
#include "lauxlib.h"

#if LUA_VERSION_NUM > 501
/*
** Lua 5.2
*/
#ifndef lua_strlen
#define lua_strlen lua_rawlen
#endif
/* luaL_typerror always used with arg at ndx == NULL */
#define luaL_typerror(L,ndx,str) luaL_error(L,"bad argument %d (%s expected, got nil)",ndx,str)
/* luaL_register used once, so below expansion is OK for this case */
#define luaL_register(L,name,reg) lua_newtable(L);luaL_setfuncs(L,reg,0)
/* luaL_openlib always used with name == NULL */
#define luaL_openlib(L,name,reg,nup) luaL_setfuncs(L,reg,nup)

#if LUA_VERSION_NUM > 502
/*
** Lua 5.3
*/
#define luaL_checkint(L,n)  ((int)luaL_checkinteger(L, (n)))
#endif
#endif



/*
** =======================================================
** Register functions
** =======================================================
*/

#define SC(s)   { #s, SQLITE_ ## s },
#define LSC(s)  { #s, LSQLITE_ ## s },

static const struct {
    const char* name;
    int value;
} sqlite_constants[] = {
    /* error codes */
    SC(OK)          SC(ERROR)       SC(INTERNAL)    SC(PERM)
    SC(ABORT)       SC(BUSY)        SC(LOCKED)      SC(NOMEM)
    SC(READONLY)    SC(INTERRUPT)   SC(IOERR)       SC(CORRUPT)
    SC(NOTFOUND)    SC(FULL)        SC(CANTOPEN)    SC(PROTOCOL)
    SC(EMPTY)       SC(SCHEMA)      SC(TOOBIG)      SC(CONSTRAINT)
    SC(MISMATCH)    SC(MISUSE)      SC(NOLFS)
    SC(FORMAT)      SC(NOTADB)

    /* sqlite_step specific return values */
    SC(RANGE)       SC(ROW)         SC(DONE)

    /* column types */
    SC(INTEGER)     SC(FLOAT)       SC(TEXT)        SC(BLOB)
    SC(NULL)

    /* Authorizer Action Codes */
    SC(CREATE_INDEX       )
    SC(CREATE_TABLE       )
    SC(CREATE_TEMP_INDEX  )
    SC(CREATE_TEMP_TABLE  )
    SC(CREATE_TEMP_TRIGGER)
    SC(CREATE_TEMP_VIEW   )
    SC(CREATE_TRIGGER     )
    SC(CREATE_VIEW        )
    SC(DELETE             )
    SC(DROP_INDEX         )
    SC(DROP_TABLE         )
    SC(DROP_TEMP_INDEX    )
    SC(DROP_TEMP_TABLE    )
    SC(DROP_TEMP_TRIGGER  )
    SC(DROP_TEMP_VIEW     )
    SC(DROP_TRIGGER       )
    SC(DROP_VIEW          )
    SC(INSERT             )
    SC(PRAGMA             )
    SC(READ               )
    SC(SELECT             )
    SC(TRANSACTION        )
    SC(UPDATE             )
    SC(ATTACH             )
    SC(DETACH             )
    SC(ALTER_TABLE        )
    SC(REINDEX            )
    SC(ANALYZE            )
    SC(CREATE_VTABLE      )
    SC(DROP_VTABLE        )
    SC(FUNCTION           )
    SC(SAVEPOINT          )

    /* file open flags */
    SC(OPEN_READONLY)
    SC(OPEN_READWRITE)
    SC(OPEN_CREATE)
    SC(OPEN_URI)
    SC(OPEN_MEMORY)
    SC(OPEN_NOMUTEX)
    SC(OPEN_FULLMUTEX)
    SC(OPEN_SHAREDCACHE)
    SC(OPEN_PRIVATECACHE)
    
    /* terminator */
    { NULL, 0 }
};

/* ======================================================= */

static const luaL_Reg dblib[] = {
    {"isopen",              db_isopen               },
    {"last_insert_rowid",   db_last_insert_rowid    },
    {"changes",             db_changes              },
    {"total_changes",       db_total_changes        },
    {"errcode",             db_errcode              },
    {"error_code",          db_errcode              },
    {"errmsg",              db_errmsg               },
    {"error_message",       db_errmsg               },
    {"interrupt",           db_interrupt            },
    {"db_filename",         db_db_filename          },

    {"create_function",     db_create_function      },
    {"create_aggregate",    db_create_aggregate     },
    {"create_collation",    db_create_collation     },
    {"load_extension",      db_load_extension       },

    {"trace",               db_trace                },
    {"progress_handler",    db_progress_handler     },
    {"busy_timeout",        db_busy_timeout         },
    {"busy_handler",        db_busy_handler         },
#if !defined(LSQLITE_OMIT_UPDATE_HOOK) || !LSQLITE_OMIT_UPDATE_HOOK
    {"update_hook",         db_update_hook          },
    {"commit_hook",         db_commit_hook          },
    {"rollback_hook",       db_rollback_hook        },
#endif

    {"prepare",             db_prepare              },
    {"rows",                db_rows                 },
    {"urows",               db_urows                },
    {"nrows",               db_nrows                },

    {"exec",                db_exec                 },
    {"execute",             db_exec                 },
    {"close",               db_close                },
    {"close_vm",            db_close_vm             },
    {"get_ptr",             db_get_ptr              },

    {"__tostring",          db_tostring             },
    {"__gc",                db_gc                   },

    {NULL, NULL}
};

static const luaL_Reg vmlib[] = {
    {"isopen",              dbvm_isopen             },

    {"step",                dbvm_step               },
    {"reset",               dbvm_reset              },
    {"finalize",            dbvm_finalize           },

    {"columns",             dbvm_columns            },

    {"bind",                dbvm_bind               },
    {"bind_values",         dbvm_bind_values        },
    {"bind_names",          dbvm_bind_names         },
    {"bind_blob",           dbvm_bind_blob          },
    {"bind_parameter_count",dbvm_bind_parameter_count},
    {"bind_parameter_name", dbvm_bind_parameter_name},

    {"get_value",           dbvm_get_value          },
    {"get_values",          dbvm_get_values         },
    {"get_name",            dbvm_get_name           },
    {"get_names",           dbvm_get_names          },
    {"get_type",            dbvm_get_type           },
    {"get_types",           dbvm_get_types          },
    {"get_uvalues",         dbvm_get_uvalues        },
    {"get_unames",          dbvm_get_unames         },
    {"get_utypes",          dbvm_get_utypes         },

    {"get_named_values",    dbvm_get_named_values   },
    {"get_named_types",     dbvm_get_named_types    },

    {"rows",                dbvm_rows               },
    {"urows",               dbvm_urows              },
    {"nrows",               dbvm_nrows              },

    {"last_insert_rowid",   dbvm_last_insert_rowid  },

    /* compatibility names (added by request) */
    {"idata",               dbvm_get_values         },
    {"inames",              dbvm_get_names          },
    {"itypes",              dbvm_get_types          },
    {"data",                dbvm_get_named_values   },
    {"type",                dbvm_get_named_types    },

    {"__tostring",          dbvm_tostring           },
    {"__gc",                dbvm_gc                 },

    { NULL, NULL }
};

static const luaL_Reg ctxlib[] = {
    {"user_data",               lcontext_user_data              },

    {"get_aggregate_data",      lcontext_get_aggregate_context  },
    {"set_aggregate_data",      lcontext_set_aggregate_context  },
    {"aggregate_count",         lcontext_aggregate_count        },

    {"result",                  lcontext_result                 },
    {"result_null",             lcontext_result_null            },
    {"result_number",           lcontext_result_double          },
    {"result_double",           lcontext_result_double          },
    {"result_int",              lcontext_result_int             },
    {"result_text",             lcontext_result_text            },
    {"result_blob",             lcontext_result_blob            },
    {"result_error",            lcontext_result_error           },

    {"__tostring",              lcontext_tostring               },
    {NULL, NULL}
};

static const luaL_Reg dbbulib[] = {

    {"step",        dbbu_step       },
    {"remaining",   dbbu_remaining  },
    {"pagecount",   dbbu_pagecount  },
    {"finish",      dbbu_finish     },

/*  {"__tostring",  dbbu_tostring   },   */
    {"__gc",        dbbu_gc         },
    {NULL, NULL}
};

static const luaL_Reg sqlitelib[] = {
    {"lversion",        lsqlite_lversion        },
    {"version",         lsqlite_version         },
    {"complete",        lsqlite_complete        },
#ifndef _WIN32
    {"temp_directory",  lsqlite_temp_directory  },
#endif
#if defined(SQLITE_ENABLE_CEROD)
    {"activate_cerod",  lsqlite_activate_cerod  },
#endif
    {"open",            lsqlite_open            },
    {"open_memory",     lsqlite_open_memory     },
    {"open_ptr",        lsqlite_open_ptr        },

    {"backup_init",     lsqlite_backup_init     },

    {"__newindex",      lsqlite_newindex        },
    {NULL, NULL}
};

static void create_meta(lua_State *L, const char *name, const luaL_Reg *lib) {
    luaL_newmetatable(L, name);
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);               /* push metatable */
    lua_rawset(L, -3);                  /* metatable.__index = metatable */

    /* register metatable functions */
    luaL_openlib(L, NULL, lib, 0);

    /* remove metatable from stack */
    lua_pop(L, 1);
}

LUALIB_API int luaopen_didkit(lua_State *L) {
    create_meta(L, sqlite_meta, dblib);
    create_meta(L, sqlite_vm_meta, vmlib);
    create_meta(L, sqlite_bu_meta, dbbulib);
    create_meta(L, sqlite_ctx_meta, ctxlib);

    luaL_getmetatable(L, sqlite_ctx_meta);
    sqlite_ctx_meta_ref = luaL_ref(L, LUA_REGISTRYINDEX);

    /* register (local) sqlite metatable */
    luaL_register(L, "sqlite3", sqlitelib);

    {
        int i = 0;
        /* add constants to global table */
        while (sqlite_constants[i].name) {
            lua_pushstring(L, sqlite_constants[i].name);
            lua_pushinteger(L, sqlite_constants[i].value);
            lua_rawset(L, -3);
            ++i;
        }
    }

    /* set sqlite's metatable to itself - set as readonly (__newindex) */
    lua_pushvalue(L, -1);
    lua_setmetatable(L, -2);

    return 1;
}
