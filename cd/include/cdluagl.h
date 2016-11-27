/** \file
 * \brief OpenGL Canvas Lua Binding
 *
 * See Copyright Notice in cd.h
 */

#ifndef __CD_LUAGL_H
#define __CD_LUAGL_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LUA_NOOBJECT  /* Lua 3 */
void cdluagl_open(void);
#endif

#ifdef LUA_TNONE  /* Lua 5 */
int cdluagl_open(lua_State *L);
#endif

#ifdef __cplusplus
}
#endif

#endif

