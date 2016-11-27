/** \file
 * \brief User API
 * CD - Canvas Draw
 * Tecgraf: Computer Graphics Technology Group, PUC-Rio, Brazil
 * http://www.tecgraf.puc-rio.br/cd	
 * mailto:cd@tecgraf.puc-rio.br
 *
 * See Copyright Notice at the end of this file
 */
 
#ifndef __CD_H
#define __CD_H

#ifdef __cplusplus
extern "C" {
#endif


#define CD_NAME "CD - A 2D Graphics Library"
#define CD_DESCRIPTION "Vector Graphics Toolkit with Device Independent Output"
#define CD_COPYRIGHT "Copyright (C) 1994-2016 Tecgraf/PUC-Rio"
#define CD_VERSION "5.11"              /* bug fixes are reported only by cdVersion functions */
#define CD_VERSION_NUMBER 511000
#define CD_VERSION_DATE "2016/09/30"  /* does not include bug fix releases */

typedef struct _cdContext cdContext;
typedef struct _cdCanvas cdCanvas;
typedef struct _cdCanvas cdState;
typedef struct _cdImage cdImage;

/* client images using bitmap structure */
typedef struct _cdBitmap {
  int w;
  int h;
  int type;
  void *data;
} cdBitmap;


/* library */
char*         cdVersion(void);
char*         cdVersionDate(void);
int           cdVersionNumber(void);

/* canvas init */
cdCanvas*   cdCreateCanvas(cdContext *context, void *data);
cdCanvas*   cdCreateCanvasf(cdContext *context, const char* format, ...);
void        cdKillCanvas(cdCanvas* canvas);

cdContext*  cdCanvasGetContext(cdCanvas* canvas);
int         cdCanvasActivate(cdCanvas* canvas);
void        cdCanvasDeactivate(cdCanvas* canvas);
int         cdUseContextPlus(int use);
void        cdInitContextPlus(void);   /* need an external library */
void        cdFinishContextPlus(void);   /* need an external library */

/* context */
typedef int (*cdCallback)(cdCanvas* canvas, ...);
int cdContextRegisterCallback(cdContext *context, int cb, cdCallback func);
unsigned long cdContextCaps(cdContext *context);
int cdContextIsPlus(cdContext *context);
int cdContextType(cdContext *context);

/* control */
int      cdCanvasSimulate(cdCanvas* canvas, int mode);
void     cdCanvasFlush(cdCanvas* canvas);
void     cdCanvasClear(cdCanvas* canvas);
cdState* cdCanvasSaveState(cdCanvas* canvas);
void     cdCanvasRestoreState(cdCanvas* canvas, cdState* state);
void     cdReleaseState(cdState* state);
void     cdCanvasSetAttribute(cdCanvas* canvas, const char* name, char* data);
void     cdCanvasSetfAttribute(cdCanvas* canvas, const char* name, const char* format, ...);
char*    cdCanvasGetAttribute(cdCanvas* canvas, const char* name);

/* interpretation */
int cdCanvasPlay(cdCanvas* canvas, cdContext *context, int xmin, int xmax, int ymin, int ymax, void *data);

/* coordinate transformation */
void cdCanvasGetSize(cdCanvas* canvas, int *width, int *height, double *width_mm, double *height_mm);
int cdCanvasUpdateYAxis(cdCanvas* canvas, int* y);
double cdfCanvasUpdateYAxis(cdCanvas* canvas, double* y);
int cdCanvasInvertYAxis(cdCanvas* canvas, int y);
double cdfCanvasInvertYAxis(cdCanvas* canvas, double y);
void cdCanvasMM2Pixel(cdCanvas* canvas, double mm_dx, double mm_dy, int *dx, int *dy);
void cdCanvasPixel2MM(cdCanvas* canvas, int dx, int dy, double *mm_dx, double *mm_dy);
void cdfCanvasMM2Pixel(cdCanvas* canvas, double mm_dx, double mm_dy, double *dx, double *dy);
void cdfCanvasPixel2MM(cdCanvas* canvas, double dx, double dy, double *mm_dx, double *mm_dy);
void cdCanvasOrigin(cdCanvas* canvas, int x, int y);
void cdfCanvasOrigin(cdCanvas* canvas, double x, double y);
void cdCanvasGetOrigin(cdCanvas* canvas, int *x, int *y);
void cdfCanvasGetOrigin(cdCanvas* canvas, double *x, double *y);
void cdCanvasTransform(cdCanvas* canvas, const double* matrix);
double* cdCanvasGetTransform(cdCanvas* canvas);
void cdCanvasTransformMultiply(cdCanvas* canvas, const double* matrix);
void cdCanvasTransformRotate(cdCanvas* canvas, double angle);
void cdCanvasTransformScale(cdCanvas* canvas, double sx, double sy);
void cdCanvasTransformTranslate(cdCanvas* canvas, double dx, double dy);
void cdCanvasTransformPoint(cdCanvas* canvas, int x, int y, int *tx, int *ty);
void cdfCanvasTransformPoint(cdCanvas* canvas, double x, double y, double *tx, double *ty);

/* clipping */
int  cdCanvasClip(cdCanvas* canvas, int mode);
void cdCanvasClipArea(cdCanvas* canvas, int xmin, int xmax, int ymin, int ymax);
int  cdCanvasGetClipArea(cdCanvas* canvas, int *xmin, int *xmax, int *ymin, int *ymax);
void cdfCanvasClipArea(cdCanvas* canvas, double xmin, double xmax, double ymin, double ymax);
int  cdfCanvasGetClipArea(cdCanvas* canvas, double *xmin, double *xmax, double *ymin, double *ymax);

/* clipping region */
int  cdCanvasIsPointInRegion(cdCanvas* canvas, int x, int y);
void cdCanvasOffsetRegion(cdCanvas* canvas, int x, int y);
void cdCanvasGetRegionBox(cdCanvas* canvas, int *xmin, int *xmax, int *ymin, int *ymax);
int  cdCanvasRegionCombineMode(cdCanvas* canvas, int mode);

/* primitives */
void cdCanvasPixel(cdCanvas* canvas, int x, int y, long color);
void cdCanvasMark(cdCanvas* canvas, int x, int y);
void cdfCanvasPixel(cdCanvas* canvas, double x, double y, long color);
void cdfCanvasMark(cdCanvas* canvas, double x, double y);

void cdCanvasBegin(cdCanvas* canvas, int mode);
void cdCanvasPathSet(cdCanvas* canvas, int action);
void cdCanvasEnd(cdCanvas* canvas);

void cdCanvasLine(cdCanvas* canvas, int x1, int y1, int x2, int y2);
void cdCanvasVertex(cdCanvas* canvas, int x, int y);
void cdCanvasRect(cdCanvas* canvas, int xmin, int xmax, int ymin, int ymax);
void cdCanvasBox(cdCanvas* canvas, int xmin, int xmax, int ymin, int ymax);
void cdCanvasArc(cdCanvas* canvas, int xc, int yc, int w, int h, double angle1, double angle2);
void cdCanvasSector(cdCanvas* canvas, int xc, int yc, int w, int h, double angle1, double angle2);
void cdCanvasChord(cdCanvas* canvas, int xc, int yc, int w, int h, double angle1, double angle2);
void cdCanvasText(cdCanvas* canvas, int x, int y, const char* s);

void cdfCanvasLine(cdCanvas* canvas, double x1, double y1, double x2, double y2);
void cdfCanvasVertex(cdCanvas* canvas, double x, double y);
void cdfCanvasRect(cdCanvas* canvas, double xmin, double xmax, double ymin, double ymax);
void cdfCanvasBox(cdCanvas* canvas, double xmin, double xmax, double ymin, double ymax);
void cdfCanvasArc(cdCanvas* canvas, double xc, double yc, double w, double h, double angle1, double angle2);
void cdfCanvasSector(cdCanvas* canvas, double xc, double yc, double w, double h, double angle1, double angle2);
void cdfCanvasChord(cdCanvas* canvas, double xc, double yc, double w, double h, double angle1, double angle2);
void cdfCanvasText(cdCanvas* canvas, double x, double y, const char* s);

/* attributes */
void cdCanvasSetBackground(cdCanvas* canvas, long color);
void cdCanvasSetForeground(cdCanvas* canvas, long color);
long cdCanvasBackground(cdCanvas* canvas, long color);
long cdCanvasForeground(cdCanvas* canvas, long color);
int  cdCanvasBackOpacity(cdCanvas* canvas, int opacity);
int  cdCanvasWriteMode(cdCanvas* canvas, int mode);
int  cdCanvasLineStyle(cdCanvas* canvas, int style);
void cdCanvasLineStyleDashes(cdCanvas* canvas, const int* dashes, int count);
int  cdCanvasLineWidth(cdCanvas* canvas, int width);
int  cdCanvasLineJoin(cdCanvas* canvas, int join);
int  cdCanvasLineCap(cdCanvas* canvas, int cap);
int  cdCanvasInteriorStyle(cdCanvas* canvas, int style);
int  cdCanvasHatch(cdCanvas* canvas, int style);
void cdCanvasStipple(cdCanvas* canvas, int w, int h, const unsigned char* stipple);
unsigned char* cdCanvasGetStipple(cdCanvas* canvas, int *n, int *m);
void   cdCanvasPattern(cdCanvas* canvas, int w, int h, long const int *pattern);
long*  cdCanvasGetPattern(cdCanvas* canvas, int* n, int* m);
int    cdCanvasFillMode(cdCanvas* canvas, int mode);
int    cdCanvasFont(cdCanvas* canvas, const char* type_face, int style, int size);
void   cdCanvasGetFont(cdCanvas* canvas, char *type_face, int *style, int *size);
char*  cdCanvasNativeFont(cdCanvas* canvas, const char* font);
int    cdCanvasTextAlignment(cdCanvas* canvas, int alignment);
double cdCanvasTextOrientation(cdCanvas* canvas, double angle);
int    cdCanvasMarkType(cdCanvas* canvas, int type);
int    cdCanvasMarkSize(cdCanvas* canvas, int size);

/* vector text */
void cdCanvasVectorText(cdCanvas* canvas, int x, int y, const char* s);
void cdCanvasMultiLineVectorText(cdCanvas* canvas, int x, int y, const char* s);

/* vector text attributes */
char* cdCanvasVectorFont(cdCanvas* canvas, const char *filename);
void  cdCanvasVectorTextDirection(cdCanvas* canvas, int x1, int y1, int x2, int y2);
double* cdCanvasVectorTextTransform(cdCanvas* canvas, const double* matrix);
void  cdCanvasVectorTextSize(cdCanvas* canvas, int size_x, int size_y, const char* s);
int   cdCanvasVectorCharSize(cdCanvas* canvas, int size);
void  cdCanvasVectorFontSize(cdCanvas* canvas, double size_x, double size_y);
void  cdCanvasGetVectorFontSize(cdCanvas* canvas, double *size_x, double *size_y);

/* vector text properties */
void cdCanvasGetVectorTextSize(cdCanvas* canvas, const char* s, int *x, int *y);
void cdCanvasGetVectorTextBounds(cdCanvas* canvas, const char* s, int x, int y, int *rect);
void cdCanvasGetVectorTextBox(cdCanvas* canvas, int x, int y, const char *s, int *xmin, int *xmax, int *ymin, int *ymax);

void cdfCanvasVectorTextDirection(cdCanvas* canvas, double x1, double y1, double x2, double y2);
void cdfCanvasVectorTextSize(cdCanvas* canvas, double size_x, double size_y, const char* s);
void cdfCanvasGetVectorTextSize(cdCanvas* canvas, const char* s, double *x, double *y);
double cdfCanvasVectorCharSize(cdCanvas* canvas, double size);
void cdfCanvasVectorText(cdCanvas* canvas, double x, double y, const char* s);
void cdfCanvasMultiLineVectorText(cdCanvas* canvas, double x, double y, const char* s);
void cdfCanvasGetVectorTextBounds(cdCanvas* canvas, const char* s, double x, double y, double *rect);
void cdfCanvasGetVectorTextBox(cdCanvas* canvas, double x, double y, const char *s, double *xmin, double *xmax, double *ymin, double *ymax);

/* properties */
void cdCanvasGetFontDim(cdCanvas* canvas, int *max_width, int *height, int *ascent, int *descent);
void cdCanvasGetTextSize(cdCanvas* canvas, const char* s, int *width, int *height);
void cdCanvasGetTextBox(cdCanvas* canvas, int x, int y, const char* s, int *xmin, int *xmax, int *ymin, int *ymax);
void cdfCanvasGetTextBox(cdCanvas* canvas, double x, double y, const char *s, double *xmin, double *xmax, double *ymin, double *ymax);
void cdCanvasGetTextBounds(cdCanvas* canvas, int x, int y, const char* s, int *rect);
void cdfCanvasGetTextBounds(cdCanvas* canvas, double x, double y, const char *s, double *rect);
int  cdCanvasGetColorPlanes(cdCanvas* canvas);

/* color */
void cdCanvasPalette(cdCanvas* canvas, int n, const long *palette, int mode);

/* client images */
void cdCanvasGetImageRGB(cdCanvas* canvas, unsigned char* r, unsigned char* g, unsigned char* b, int x, int y, int iw, int ih);

void cdCanvasPutImageRectRGB(cdCanvas* canvas, int iw, int ih, const unsigned char* r, const unsigned char* g, const unsigned char* b, int x, int y, int w, int h, int xmin, int xmax, int ymin, int ymax);
void cdCanvasPutImageRectRGBA(cdCanvas* canvas, int iw, int ih, const unsigned char* r, const unsigned char* g, const unsigned char* b, const unsigned char* a, int x, int y, int w, int h, int xmin, int xmax, int ymin, int ymax);
void cdCanvasPutImageRectMap(cdCanvas* canvas, int iw, int ih, const unsigned char* index, const long* colors, int x, int y, int w, int h, int xmin, int xmax, int ymin, int ymax);

void cdfCanvasPutImageRectRGB(cdCanvas* canvas, int iw, int ih, const unsigned char* r, const unsigned char* g, const unsigned char* b, double x, double y, double w, double h, int xmin, int xmax, int ymin, int ymax);
void cdfCanvasPutImageRectRGBA(cdCanvas* canvas, int iw, int ih, const unsigned char* r, const unsigned char* g, const unsigned char* b, const unsigned char* a, double x, double y, double w, double h, int xmin, int xmax, int ymin, int ymax);
void cdfCanvasPutImageRectMap(cdCanvas* canvas, int iw, int ih, const unsigned char* index, const long* colors, double x, double y, double w, double h, int xmin, int xmax, int ymin, int ymax);

/* server images - deprecated (use double buffer drivers) */
cdImage* cdCanvasCreateImage(cdCanvas* canvas, int w, int h);
void cdKillImage(cdImage* image);
void cdCanvasGetImage(cdCanvas* canvas, cdImage* image, int x, int y);
void cdCanvasPutImageRect(cdCanvas* canvas, cdImage* image, int x, int y, int xmin, int xmax, int ymin, int ymax);
void cdCanvasScrollArea(cdCanvas* canvas, int xmin, int xmax, int ymin, int ymax, int dx, int dy);

/* bitmap - deprecated (use imImage) */
cdBitmap* cdCreateBitmap(int w, int h, int type);
cdBitmap* cdInitBitmap(int w, int h, int type, ...);
void cdKillBitmap(cdBitmap* bitmap);
unsigned char* cdBitmapGetData(cdBitmap* bitmap, int dataptr);
void cdBitmapSetRect(cdBitmap* bitmap, int xmin, int xmax, int ymin, int ymax);
void cdCanvasPutBitmap(cdCanvas* canvas, cdBitmap* bitmap, int x, int y, int w, int h);
void cdCanvasGetBitmap(cdCanvas* canvas, cdBitmap* bitmap, int x, int y);
void cdBitmapRGB2Map(cdBitmap* bitmap_rgb, cdBitmap* bitmap_map);

/* color */
long cdEncodeColor(unsigned char red, unsigned char green, unsigned char blue);
long cdEncodeColorAlpha(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
long cdEncodeAlpha(long color, unsigned char alpha);
void cdDecodeColor(long color, unsigned char* red, unsigned char* green, unsigned char* blue);
void cdDecodeColorAlpha(long color, unsigned char* red, unsigned char* green, unsigned char* blue, unsigned char* alpha);
unsigned char cdDecodeAlpha(long color);

#define cdAlpha(_)    (unsigned char)(~(((_) >> 24) & 0xFF))
#define cdReserved(_) (unsigned char)(((_) >> 24) & 0xFF)
#define cdRed(_)      (unsigned char)(((_) >> 16) & 0xFF)
#define cdGreen(_)    (unsigned char)(((_) >>  8) & 0xFF)
#define cdBlue(_)     (unsigned char)(((_) >>  0) & 0xFF)

/* client image color conversion */
void cdRGB2Map(int width, int height, const unsigned char* red, const unsigned char* green, const unsigned char* blue, unsigned char* index, int pal_size, long *color);


/* CD Values */

#define CD_QUERY -1             /* query value */

enum {                        /* bitmap type */
 CD_RGB,                      /* these definitions are compatible with the IM library */
 CD_MAP,
 CD_RGBA = 0x100 
};

enum {                         /* bitmap data */
 CD_IRED,
 CD_IGREEN,
 CD_IBLUE,
 CD_IALPHA,
 CD_INDEX,
 CD_COLORS
};

enum {                          /* status report */
 CD_ERROR = -1,
 CD_OK    =  0
};

enum {                          /* clip mode */
 CD_CLIPOFF,
 CD_CLIPAREA,
 CD_CLIPPOLYGON,
 CD_CLIPREGION
};

enum {                          /* region combine mode */
 CD_UNION,
 CD_INTERSECT,
 CD_DIFFERENCE,
 CD_NOTINTERSECT
};

enum {                          /* polygon mode (begin...end) */
 CD_FILL,
 CD_OPEN_LINES,
 CD_CLOSED_LINES,
 CD_CLIP,
 CD_BEZIER,
 CD_REGION,
 CD_PATH
};

#define CD_POLYCUSTOM 10

enum {                          /* path actions */
 CD_PATH_NEW,
 CD_PATH_MOVETO,
 CD_PATH_LINETO,
 CD_PATH_ARC,
 CD_PATH_CURVETO,
 CD_PATH_CLOSE,
 CD_PATH_FILL,
 CD_PATH_STROKE,
 CD_PATH_FILLSTROKE,
 CD_PATH_CLIP
};

enum {                          /* fill mode */
 CD_EVENODD,
 CD_WINDING
};

enum {                          /* line join  */
 CD_MITER,
 CD_BEVEL,
 CD_ROUND
};  

enum {                          /* line cap  */
 CD_CAPFLAT,  
 CD_CAPSQUARE,
 CD_CAPROUND
};  

enum {                          /* background opacity mode */
 CD_OPAQUE,
 CD_TRANSPARENT
};

enum {                          /* write mode */
 CD_REPLACE,
 CD_XOR,
 CD_NOT_XOR
};

enum {                          /* color allocation mode (palette) */
 CD_POLITE,
 CD_FORCE
};

enum {                          /* line style */
 CD_CONTINUOUS,
 CD_DASHED,
 CD_DOTTED,
 CD_DASH_DOT,
 CD_DASH_DOT_DOT,
 CD_CUSTOM
};

enum {                          /* marker type */
 CD_PLUS,
 CD_STAR,
 CD_CIRCLE,
 CD_X,
 CD_BOX,
 CD_DIAMOND,
 CD_HOLLOW_CIRCLE,
 CD_HOLLOW_BOX,
 CD_HOLLOW_DIAMOND
};

enum {                          /* hatch type */
 CD_HORIZONTAL,
 CD_VERTICAL,
 CD_FDIAGONAL,
 CD_BDIAGONAL,
 CD_CROSS,
 CD_DIAGCROSS
};

enum {                          /* interior style */
 CD_SOLID,
 CD_HATCH,
 CD_STIPPLE,
 CD_PATTERN,
 CD_HOLLOW
};

enum {                          /* text alignment */
 CD_NORTH,
 CD_SOUTH,
 CD_EAST,
 CD_WEST,
 CD_NORTH_EAST,
 CD_NORTH_WEST,
 CD_SOUTH_EAST,
 CD_SOUTH_WEST,
 CD_CENTER,
 CD_BASE_LEFT,
 CD_BASE_CENTER,
 CD_BASE_RIGHT
};

enum {                          /* style */
 CD_PLAIN  = 0,
 CD_BOLD   = 1,
 CD_ITALIC = 2,
 CD_UNDERLINE = 4,
 CD_STRIKEOUT = 8
};

#define CD_BOLD_ITALIC (CD_BOLD|CD_ITALIC)  /* compatibility name */

enum {                          /* some font sizes */
 CD_SMALL    =  8,
 CD_STANDARD = 12,
 CD_LARGE    = 18
};

/* Context Capabilities */
#define CD_CAP_NONE             0x00000000
#define CD_CAP_FLUSH            0x00000001
#define CD_CAP_CLEAR            0x00000002
#define CD_CAP_PLAY             0x00000004
#define CD_CAP_YAXIS            0x00000008
#define CD_CAP_CLIPAREA         0x00000010
#define CD_CAP_CLIPPOLY         0x00000020
#define CD_CAP_REGION           0x00000040
#define CD_CAP_RECT             0x00000080
#define CD_CAP_CHORD            0x00000100
#define CD_CAP_IMAGERGB         0x00000200
#define CD_CAP_IMAGERGBA        0x00000400
#define CD_CAP_IMAGEMAP         0x00000800
#define CD_CAP_GETIMAGERGB      0x00001000
#define CD_CAP_IMAGESRV         0x00002000
#define CD_CAP_BACKGROUND       0x00004000
#define CD_CAP_BACKOPACITY      0x00008000
#define CD_CAP_WRITEMODE        0x00010000
#define CD_CAP_LINESTYLE        0x00020000
#define CD_CAP_LINEWITH         0x00040000
#define CD_CAP_FPRIMTIVES       0x00080000
#define CD_CAP_HATCH            0x00100000
#define CD_CAP_STIPPLE          0x00200000
#define CD_CAP_PATTERN          0x00400000
#define CD_CAP_FONT             0x00800000
#define CD_CAP_FONTDIM          0x01000000
#define CD_CAP_TEXTSIZE         0x02000000
#define CD_CAP_TEXTORIENTATION  0x04000000
#define CD_CAP_PALETTE          0x08000000
#define CD_CAP_LINECAP          0x10000000
#define CD_CAP_LINEJOIN         0x20000000
#define CD_CAP_PATH             0x40000000
#define CD_CAP_BEZIER           0x80000000
#define CD_CAP_ALL              0xFFFFFFFF

/* Context Types */
enum { 
  CD_CTX_WINDOW, 
  CD_CTX_DEVICE, 
  CD_CTX_IMAGE,
  CD_CTX_FILE
};

/* cdPlay definitions */
#define CD_SIZECB 0        /* size callback */
typedef int(*cdSizeCB)(cdCanvas *canvas, int w, int h, double w_mm, double h_mm);
#define CD_ABORT 1
#define CD_CONTINUE 0

/* simulation flags */
#define CD_SIM_NONE         0x0000
#define CD_SIM_LINE         0x0001
#define CD_SIM_RECT         0x0002
#define CD_SIM_BOX          0x0004
#define CD_SIM_ARC          0x0008
#define CD_SIM_SECTOR       0x0010
#define CD_SIM_CHORD        0x0020
#define CD_SIM_POLYLINE     0x0040
#define CD_SIM_POLYGON      0x0080
#define CD_SIM_TEXT         0x0100
#define CD_SIM_ALL          0xFFFF

#define CD_SIM_LINES (CD_SIM_LINE | CD_SIM_RECT | CD_SIM_ARC | CD_SIM_POLYLINE)
#define CD_SIM_FILLS (CD_SIM_BOX | CD_SIM_SECTOR | CD_SIM_CHORD | CD_SIM_POLYGON)

/* some predefined colors for convenience */
#define  CD_RED           0xFF0000L   /* 255,  0,  0 */
#define  CD_DARK_RED      0x800000L   /* 128,  0,  0 */
#define  CD_GREEN         0x00FF00L   /*   0,255,  0 */
#define  CD_DARK_GREEN    0x008000L   /*   0,128,  0 */
#define  CD_BLUE          0x0000FFL   /*   0,  0,255 */
#define  CD_DARK_BLUE     0x000080L   /*   0,  0,128 */

#define  CD_YELLOW        0xFFFF00L   /* 255,255,  0 */
#define  CD_DARK_YELLOW   0x808000L   /* 128,128,  0 */
#define  CD_MAGENTA       0xFF00FFL   /* 255,  0,255 */
#define  CD_DARK_MAGENTA  0x800080L   /* 128,  0,128 */
#define  CD_CYAN          0x00FFFFL   /*   0,255,255 */
#define  CD_DARK_CYAN     0x008080L   /*   0,128,128 */

#define  CD_WHITE         0xFFFFFFL   /* 255,255,255 */
#define  CD_BLACK         0x000000L   /*   0,  0,  0 */

#define  CD_DARK_GRAY     0x808080L   /* 128,128,128 */
#define  CD_GRAY          0xC0C0C0L   /* 192,192,192 */

/* some usefull conversion factors */
#define CD_MM2PT     2.834645669   /* milimeters to points (pt = CD_MM2PT * mm) */
#define CD_RAD2DEG  57.295779513   /* radians to degrees (deg = CD_RAD2DEG * rad) */
#define CD_DEG2RAD  0.01745329252  /* degrees to radians (rad = CD_DEG2RAD * deg) */

/* paper sizes */
enum {
  CD_A0,
  CD_A1,
  CD_A2,
  CD_A3,
  CD_A4,
  CD_A5,
  CD_LETTER,
  CD_LEGAL
};


#ifdef __cplusplus
}
#endif



/******************************************************************************
Copyright (C) 1994-2016 Tecgraf/PUC-Rio.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************/

#endif /* ifndef CD_H */

