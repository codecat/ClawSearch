/** \file
 * \brief Lua Binding Control
 *
 * See Copyright Notice in cd.h
 */

#ifndef __CD_LUA_H
#define __CD_LUA_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LUA_NOOBJECT  /* Lua 3 */
void cdlua_open(void);
void cdlua_close(void);

/* utilities */
cdCanvas* cdlua_getcanvas(void);        /* pos=1, deprecated use cdlua_checkcanvas */
cdCanvas* cdlua_checkcanvas(int pos);
void cdlua_pushcanvas(cdCanvas* canvas);
#endif

#ifdef LUA_TNONE  /* Lua 5 */
int cdlua_open(lua_State *L);
int cdlua_close(lua_State *L);

/* utilities */
cdCanvas* cdlua_getcanvas(lua_State * L); /* pos=1, deprecated use cdlua_checkcanvas */
cdCanvas* cdlua_checkcanvas(lua_State * L, int pos);
void cdlua_pushcanvas(lua_State * L, cdCanvas* canvas);

#endif

#ifdef __cplusplus
}
#endif

#endif

