/** \file
 * \brief Private Lua 3 Binding Functions
 *
 * See Copyright Notice in cd.h
 */

#ifndef __CDLUA3_PRIVATE_H
#define __CDLUA3_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

#define COLOR_TAG     "CDLUA_COLOR_TAG"
#define CANVAS_TAG    "CDLUA_CANVAS_TAG"
#define STATE_TAG     "CDLUA_STATE_TAG"
#define BITMAP_TAG    "CDLUA_BITMAP_TAG"
#define IMAGE_TAG     "CDLUA_IMAGE_TAG"
#define IMAGERGB_TAG  "CDLUA_IMAGERGB_TAG"
#define IMAGERGBA_TAG "CDLUA_IMAGERGBA_TAG"
#define STIPPLE_TAG   "CDLUA_STIPPLE_TAG"
#define PATTERN_TAG   "CDLUA_PATTERN_TAG"
#define PALETTE_TAG   "CDLUA_PALETTE_TAG"
#define IMAGEMAP_TAG  "CDLUA_IMAGEMAP_TAG"
#define CHANNEL_TAG   "CDLUA_CHANNEL_TAG"

/* context management */

typedef struct _cdCallbackLUA {
  int lock;
  char *name;
  cdCallback func;
} cdCallbackLUA;

typedef struct _cdContextLUA {
  int id;
  char *name;
  cdContext* (*ctx)(void);
  void* (*checkdata)(int param);
  cdCallbackLUA* cb_list;
  int cb_n;
} cdContextLUA;

void cdlua_addcontext(cdContextLUA* luactx);
void cdlua_register(char* name, lua_CFunction func);
void cdlua_pushnumber(double num, char* name);

/* tag management */

typedef struct _canvas_t {
  cdCanvas *cd_canvas;
} canvas_t;

typedef struct _state_t {
  cdState *state;
} state_t;

typedef struct _stipple_t {
  unsigned char *value;
  int width;
  int height;
  long int size;
} stipple_t;

typedef struct _pattern_t {
  long int *color;
  int width;
  int height;
  long int size;
} pattern_t;

typedef struct _palette_t {
  long int *color;
  long int size;
} palette_t;

typedef struct _image_t {
  void *cd_image;
} image_t;

typedef struct _imagergb_t {
  unsigned char *red;
  unsigned char *green;
  unsigned char *blue;
  int width;
  int height;
  long int size;
} imagergb_t;

typedef struct _imagergba_t {
  unsigned char *red;
  unsigned char *green;
  unsigned char *blue;
  unsigned char *alpha;
  int width;
  int height;
  long int size;
} imagergba_t;
         
typedef struct _imagemap_t {
  unsigned char *index;
  int width;
  int height;
  long int size;
} imagemap_t;

typedef struct _channel_t {
  unsigned char *value;
  long int size;
} channel_t;

typedef struct _bitmap_t {
  cdBitmap *image;
} bitmap_t;

#ifdef __cplusplus
}
#endif

#endif
