/** \file
 * \brief PDF Canvas Lua Binding
 *
 * See Copyright Notice in cd.h
 */

#ifndef __CD_LUAPDF_H
#define __CD_LUAPDF_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LUA_NOOBJECT  /* Lua 3 */
void cdluapdf_open(void);
#endif

#ifdef LUA_TNONE  /* Lua 5 */
int cdluapdf_open(lua_State *L);
#endif

#ifdef __cplusplus
}
#endif

#endif

