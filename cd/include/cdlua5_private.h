/** \file
* \brief Private Lua 5 Binding Functions
* 
* See Copyright Notice in cd.h
*/

#ifndef __CDLUA5_PRIVATE_H
#define __CDLUA5_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif


/* context management */

typedef struct _cdluaCallback {
  int lock;
  const char* name;
  cdCallback func;
} cdluaCallback;

typedef struct _cdluaContext {
  int id;
  const char* name;
  cdContext* (*ctx)(void);
  void* (*checkdata)(lua_State* L,int param);
  cdluaCallback* cb_list;
  int cb_n;
} cdluaContext;

typedef struct _cdluaLuaState {
  cdCanvas* void_canvas;            /* the VOID canvas to avoid a NULL active canvas */
  cdluaContext* drivers[50];  /* store the registered drivers, map integer values to cdContext */
  int numdrivers;
} cdluaLuaState;

/* metatables */

typedef struct _cdluaStipple {
  unsigned char* stipple;
  int width;
  int height;
  long int size;
} cdluaStipple;

typedef struct _cdluaPattern {
  long* pattern;
  int width;
  int height;
  long int size;
} cdluaPattern;

/* this is the same declaration used in the IM toolkit for imPalette in Lua */
typedef struct _cdluaPalette {
  long* color;
  int count;
} cdluaPalette;

typedef struct _cdluaImageRGB {
  unsigned char* red;
  unsigned char* green;
  unsigned char* blue;
  int width;
  int height;
  long int size;
  int free;
} cdluaImageRGB;

typedef struct _cdluaImageRGBA {
  unsigned char* red;
  unsigned char* green;
  unsigned char* blue;
  unsigned char* alpha;
  int width;
  int height;
  long int size;
  int free;
} cdluaImageRGBA;
         
typedef struct _cdluaImageMap {
  unsigned char* index;
  int width;
  int height;
  long int size;
} cdluaImageMap;

typedef struct _cdluaImageChannel {
  unsigned char* channel;
  long int size;
} cdluaImageChannel;


cdluaLuaState* cdlua_getstate(lua_State* L);
cdluaContext* cdlua_getcontext(lua_State* L, int param);

lua_State* cdlua_getplaystate(void);
void cdlua_setplaystate(lua_State* L);

void cdlua_register_lib(lua_State *L, const luaL_Reg* funcs);
void cdlua_register_funcs(lua_State *L, const luaL_Reg* funcs);

void cdlua_kill_active(lua_State* L, cdCanvas* canvas);
void cdlua_open_active(lua_State* L, cdluaLuaState* cdL);

void cdlua_open_canvas(lua_State* L);

void cdlua_addcontext(lua_State* L, cdluaLuaState* cdL, cdluaContext* luactx);
void cdlua_initdrivers(lua_State* L, cdluaLuaState* cdL);

cdluaPalette* cdlua_checkpalette(lua_State* L, int param);
cdluaStipple* cdlua_checkstipple(lua_State* L, int param);
cdluaPattern* cdlua_checkpattern(lua_State* L, int param);
cdluaImageRGB* cdlua_checkimagergb(lua_State* L, int param);
cdluaImageRGBA* cdlua_checkimagergba(lua_State* L, int param);
cdluaImageMap* cdlua_checkimagemap(lua_State* L, int param);
cdluaImageChannel* cdlua_checkchannel(lua_State* L, int param);

long cdlua_checkcolor(lua_State* L, int param);
cdImage* cdlua_checkimage(lua_State* L, int param);
cdState* cdlua_checkstate(lua_State* L, int param);
cdBitmap* cdlua_checkbitmap(lua_State* L, int param);

void cdlua_pushcolor(lua_State* L, long color);
void cdlua_pushpalette(lua_State* L, long* palette, int size);
void cdlua_pushstipple(lua_State* L, unsigned char* stipple, int width, int height);
void cdlua_pushpattern(lua_State* L, long int* pattern, int width, int height);
void cdlua_pushimagergb(lua_State* L, unsigned char* red, unsigned char* green, unsigned char* blue, int width, int height);
void cdlua_pushimagergba(lua_State* L, unsigned char* red, unsigned char* green, unsigned char* blue, unsigned char* alpha, int width, int height);
void cdlua_pushimagemap(lua_State* L, unsigned char* index, int width, int height);
void cdlua_pushchannel(lua_State* L, unsigned char* channel, int size);

void cdlua_pushimage(lua_State* L, cdImage* image);
void cdlua_pushstate(lua_State* L, cdState* state);
void cdlua_pushbitmap(lua_State* L, cdBitmap* bitmap);

void cdlua_pushimagergb_ex(lua_State* L, unsigned char* red, unsigned char* green, unsigned char* blue, int width, int height);
void cdlua_pushimagergba_ex(lua_State* L, unsigned char* red, unsigned char* green, unsigned char* blue, unsigned char* alpha, int width, int height);

#ifdef __cplusplus
}
#endif

#endif
