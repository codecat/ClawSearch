/** \file
 * \brief Old WC API
 *
 * See Copyright Notice in cd.h
 */

#ifndef WD_OLD_H
#define WD_OLD_H

#ifdef __cplusplus
extern "C" {
#endif

/* coordinate transformation */
void wdWindow(double xmin, double xmax, double  ymin, double ymax);
void wdGetWindow(double *xmin, double  *xmax,  double  *ymin, double *ymax);
void wdViewport(int xmin, int xmax, int ymin, int ymax);
void wdGetViewport(int *xmin, int  *xmax,  int  *ymin, int *ymax);
void wdWorld2Canvas(double xw, double yw, int *xv, int *yv);
void wdWorld2CanvasSize(double hw, double vw, int *hv, int *vv);
void wdCanvas2World(int xv, int yv, double *xw, double *yw);

void wdClipArea(double xmin, double xmax, double  ymin, double ymax);
int  wdGetClipArea(double *xmin, double *xmax, double *ymin, double *ymax);
double* wdGetClipPoly(int *n);
int  wdPointInRegion(double x, double y);
void wdOffsetRegion(double x, double y);
void wdRegionBox(double *xmin, double *xmax, double *ymin, double *ymax);

void wdHardcopy(cdContext* ctx, void *data, cdCanvas *cnv, void(*draw_func)(void));

/* primitives */
void wdPixel(double x, double y, long color);
void wdMark(double x, double y);
void wdLine(double x1, double y1, double x2, double y2);
void wdVertex(double x, double y);
void wdRect(double xmin, double xmax, double ymin, double ymax);
void wdBox(double xmin, double xmax, double ymin, double ymax);
void wdArc(double xc, double yc, double w, double h, double angle1, double angle2);
void wdSector(double xc, double yc, double w, double h, double angle1, double angle2);
void wdChord(double xc, double yc, double w, double h, double angle1, double angle2);
void wdText(double x, double y, const char* s);

void wdPutImageRect(cdImage* image, double x, double y, int xmin, int xmax, int ymin, int ymax);
void wdPutImageRectRGB(int iw, int ih, const unsigned char* r, const unsigned char* g, const unsigned char* b, double x, double y, double w, double h, int xmin, int xmax, int ymin, int ymax);
void wdPutImageRectRGBA(int iw, int ih, const unsigned char* r, const unsigned char* g, const unsigned char* b, const unsigned char* a, double x, double y, double w, double h, int xmin, int xmax, int ymin, int ymax);
void wdPutImageRectMap(int iw, int ih, const unsigned char* index, const long* colors, double x, double y, double w, double h, int xmin, int xmax, int ymin, int ymax);
void wdPutBitmap(cdBitmap* bitmap, double x, double y, double w, double h);

/* attributes */
double wdLineWidth(double width);
void wdFont(int type_face, int style, double size);
void wdGetFont(int *type_face, int *style, double *size);
double wdMarkSize(double size);
void wdFontDim(double *max_width, double *height, double *ascent, double *descent);
void wdTextSize(const char* s, double *width, double *height);
void wdTextBox(double x, double y, const char* s, double *xmin, double *xmax, double *ymin, double *ymax);
void wdTextBounds(double x, double y, const char* s, double *rect);
void wdStipple(int w, int h, const unsigned char* stipple, double w_mm, double h_mm);
void wdPattern(int w, int h, const long* pattern, double w_mm, double h_mm);

/* vector text */
void wdVectorTextDirection(double x1, double y1, double x2, double y2);
void wdVectorTextSize(double size_x, double size_y, const char* s);
void wdGetVectorTextSize(const char* s, double *x, double *y);
double wdVectorCharSize(double size);
void wdVectorText(double x, double y, const char* s);
void wdMultiLineVectorText(double x, double y, const char* s);
void wdGetVectorTextBounds(const char* s, double x, double y, double *rect);

/* OLD definitions, defined for backward compatibility */
#define wdVectorFont cdVectorFont
#define wdVectorTextTransform cdVectorTextTransform
#define wdActivate cdActivate
#define wdClip(mode) cdClip(mode)
#define wdBegin(mode) cdBegin(mode)
#define wdEnd() cdEnd();
#define wdMM2Pixel(mm_dx, mm_dy, dx, dy) cdMM2Pixel(mm_dx, mm_dy, dx, dy)
#define wdPixel2MM(dx, dy, mm_dx, mm_dy) cdPixel2MM(dx, dy, mm_dx, mm_dy)

#ifdef __cplusplus
}
#endif

#endif
