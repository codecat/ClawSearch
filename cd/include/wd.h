/** \file
 * \brief World Coordinate Functions
 *
 * See Copyright Notice in cd.h
 */

#ifndef __WD_H
#define __WD_H

#ifdef __cplusplus
extern "C" {
#endif

int  wdCanvasPlay(cdCanvas* canvas, cdContext *context, double xmin, double xmax, double ymin, double ymax, void *data);

/* coordinate transformation */
void wdCanvasWindow(cdCanvas* canvas, double xmin, double xmax, double  ymin, double ymax);
void wdCanvasGetWindow(cdCanvas* canvas, double *xmin, double  *xmax,  double  *ymin, double *ymax);
void wdCanvasViewport(cdCanvas* canvas, int xmin, int xmax, int ymin, int ymax);
void wdCanvasGetViewport(cdCanvas* canvas, int *xmin, int  *xmax,  int  *ymin, int *ymax);
void wdCanvasWorld2Canvas(cdCanvas* canvas, double xw, double yw, int *xv, int *yv);
void wdCanvasWorld2CanvasSize(cdCanvas* canvas, double hw, double vw, int *hv, int *vv);
void wdCanvasCanvas2World(cdCanvas* canvas, int xv, int yv, double *xw, double *yw);
void wdCanvasSetTransform(cdCanvas* canvas, double sx, double sy, double tx, double ty);
void wdCanvasGetTransform(cdCanvas* canvas, double *sx, double *sy, double *tx, double *ty);
void wdCanvasTranslate(cdCanvas* canvas, double dtx, double dty);
void wdCanvasScale(cdCanvas* canvas, double dsx, double dsy);

void wdCanvasClipArea(cdCanvas* canvas, double xmin, double xmax, double  ymin, double ymax);
int  wdCanvasGetClipArea(cdCanvas* canvas, double *xmin, double *xmax, double *ymin, double *ymax);
int  wdCanvasIsPointInRegion(cdCanvas* canvas, double x, double y);
void wdCanvasOffsetRegion(cdCanvas* canvas, double x, double y);
void wdCanvasGetRegionBox(cdCanvas* canvas, double *xmin, double *xmax, double *ymin, double *ymax);

void wdCanvasHardcopy(cdCanvas* canvas, cdContext* ctx, void *data, void(*draw_func)(cdCanvas *canvas_copy));

/* primitives */
void wdCanvasPixel(cdCanvas* canvas, double x, double y, long color);
void wdCanvasMark(cdCanvas* canvas, double x, double y);
void wdCanvasLine(cdCanvas* canvas, double x1, double y1, double x2, double y2);
void wdCanvasVertex(cdCanvas* canvas, double x, double y);
void wdCanvasRect(cdCanvas* canvas, double xmin, double xmax, double ymin, double ymax);
void wdCanvasBox(cdCanvas* canvas, double xmin, double xmax, double ymin, double ymax);
void wdCanvasArc(cdCanvas* canvas, double xc, double yc, double w, double h, double angle1, double angle2);
void wdCanvasSector(cdCanvas* canvas, double xc, double yc, double w, double h, double angle1, double angle2);
void wdCanvasChord(cdCanvas* canvas, double xc, double yc, double w, double h, double angle1, double angle2);
void wdCanvasText(cdCanvas* canvas, double x, double y, const char* s);

void wdCanvasGetImageRGB(cdCanvas* canvas, unsigned char* r, unsigned char* g, unsigned char* b, double x, double y, int iw, int ih);
void wdCanvasPutImageRectRGB(cdCanvas* canvas, int iw, int ih, const unsigned char* r, const unsigned char* g, const unsigned char* b, double x, double y, double w, double h, int xmin, int xmax, int ymin, int ymax);
void wdCanvasPutImageRectRGBA(cdCanvas* canvas, int iw, int ih, const unsigned char* r, const unsigned char* g, const unsigned char* b, const unsigned char* a, double x, double y, double w, double h, int xmin, int xmax, int ymin, int ymax);
void wdCanvasPutImageRectMap(cdCanvas* canvas, int iw, int ih, const unsigned char* index, const long* colors, double x, double y, double w, double h, int xmin, int xmax, int ymin, int ymax);

void wdCanvasPutImageRect(cdCanvas* canvas, cdImage* image, double x, double y, int xmin, int xmax, int ymin, int ymax);

void wdCanvasPutBitmap(cdCanvas* canvas, cdBitmap* bitmap, double x, double y, double w, double h);

/* attributes */
double wdCanvasLineWidth(cdCanvas* canvas, double width);
int wdCanvasFont(cdCanvas* canvas, const char* type_face, int style, double size);
void wdCanvasGetFont(cdCanvas* canvas, char *type_face, int *style, double *size);
double wdCanvasMarkSize(cdCanvas* canvas, double size);
void wdCanvasGetFontDim(cdCanvas* canvas, double *max_width, double *height, double *ascent, double *descent);
void wdCanvasGetTextSize(cdCanvas* canvas, const char* s, double *width, double *height);
void wdCanvasGetTextBox(cdCanvas* canvas, double x, double y, const char* s, double *xmin, double *xmax, double *ymin, double *ymax);
void wdCanvasGetTextBounds(cdCanvas* canvas, double x, double y, const char* s, double *rect);
void wdCanvasStipple(cdCanvas* canvas, int w, int h, const unsigned char*fgbg, double w_mm, double h_mm);
void wdCanvasPattern(cdCanvas* canvas, int w, int h, const long *color, double w_mm, double h_mm);

/* vector text */
void wdCanvasVectorTextDirection(cdCanvas* canvas, double x1, double y1, double x2, double y2);
void wdCanvasVectorTextSize(cdCanvas* canvas, double size_x, double size_y, const char* s);
void wdCanvasGetVectorTextSize(cdCanvas* canvas, const char* s, double *x, double *y);
double wdCanvasVectorCharSize(cdCanvas* canvas, double size);
void wdCanvasVectorText(cdCanvas* canvas, double x, double y, const char* s);
void wdCanvasMultiLineVectorText(cdCanvas* canvas, double x, double y, const char* s);
void wdCanvasGetVectorTextBounds(cdCanvas* canvas, const char* s, double x, double y, double *rect);
void wdCanvasGetVectorTextBox(cdCanvas* canvas, double x, double y, const char *s, double *xmin, double *xmax, double *ymin, double *ymax);

#ifdef __cplusplus
}
#endif

#ifndef CD_NO_OLD_INTERFACE
#include "wd_old.h"
#endif

#endif
