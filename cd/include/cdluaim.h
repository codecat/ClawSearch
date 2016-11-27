/** \file
 * \brief CD+IM Lua Binding
 *
 * See Copyright Notice in cd.h
 */

#ifndef __CD_LUAIM_H
#define __CD_LUAIM_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LUA_TNONE  /* Lua 5 */
int cdluaim_open(lua_State *L);
#endif

#ifdef __cplusplus
}
#endif

#endif
