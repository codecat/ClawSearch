/** \file
 * \brief Name space for C++ high level API
 *
 * See Copyright Notice in cd.h
 */
 
#ifndef __CD_PLUS_H
#define __CD_PLUS_H


#include "cd.h"
#include "wd.h"

#include "cdiup.h"
#include "cddbuf.h"
#include "cdprint.h"
#include "cdnative.h"
#include "cdgdiplus.h"
#include "cdcgm.h"
#include "cddgn.h"
#include "cddxf.h"
#include "cdclipbd.h"
#include "cdemf.h"
#include "cdirgb.h"
#include "cdmf.h"
#include "cdps.h"
#include "cdpdf.h"
#include "cdsvg.h"
#include "cdwmf.h"
#include "cddebug.h"
#include "cdgl.h"
#include "cdpicture.h"
#include "cdim.h"




/** \brief Name space for C++ high level API
 *
 * \par
 * Defines wrapper classes for all C structures.
 *
 * See \ref cd_plus.h
 */
namespace cd
{
  inline char* Version() { return cdVersion(); }
  inline char* VersionDate() { return cdVersionDate(); }
  inline int VersionNumber() { return cdVersionNumber(); }


  inline long ColorEncode(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255) {
    return cdEncodeColorAlpha(red, green, blue, alpha); }
  inline unsigned char ColorAlpha(long color) { return cdAlpha(color); }
  inline unsigned char ColorRed(long color) { return cdRed(color); }
  inline unsigned char ColorGreen(long color) { return cdGreen(color); }
  inline unsigned char ColorBlue(long color) { return cdBlue(color); }


  class Context
  {
    cdContext* cd_context;

    friend class Canvas;

  public:
    Context(cdContext* ref_context) { cd_context = ref_context; }

    unsigned long Caps() { return cdContextCaps(cd_context); }
    bool IsPlus() { return cdContextIsPlus(cd_context) != 0; }
    int Type() { return cdContextType(cd_context); }

    static void UsePlus(bool use) { cdUseContextPlus(use); }
    static void InitPlus() { cdInitContextPlus(); }
    static void FinishPlus() { cdFinishContextPlus(); }

    static void GetScreenSize(int &width, int &height, double &width_mm, double &height_mm) { cdGetScreenSize(&width, &height, &width_mm, &height_mm); }
    static int GetScreenColorPlanes() { return cdGetScreenColorPlanes(); }
  };


  class Canvas
  {
  protected:
    cdCanvas* canvas;

    /* forbidden */
    Canvas() { canvas = 0; }
    Canvas(const Canvas&) {}

    static inline int PlaySize_CB(cdCanvas *cd_canvas, int w, int h, double w_mm, double h_mm) { 
      Canvas* canvas = (Canvas*)cdCanvasGetAttribute(cd_canvas, "USERDATA");
      return canvas->PlaySizeCallback(w, h, w_mm, h_mm); 
    }

  public:
    Canvas(cdCanvas* ref_canvas) { canvas = ref_canvas; }
    ~Canvas() { 
      if (canvas) 
        cdKillCanvas(canvas); 
    }

    cdCanvas* GetHandle() const { return canvas; }

    Context GetContext() { return cdCanvasGetContext(canvas); }
    int Activate() { return cdCanvasActivate(canvas); }
    void Deactivate() { cdCanvasDeactivate(canvas); }

    /* control */
    int Simulate(int mode) { return cdCanvasSimulate(canvas, mode); }
    void Flush() { cdCanvasFlush(canvas); }
    void Clear() { cdCanvasClear(canvas); }

    cdState* SaveState() { return cdCanvasSaveState(canvas); }
    void RestoreState(cdState* state) { cdCanvasRestoreState(canvas, state); }
    void ReleaseState(cdState* state) { cdReleaseState(state); }

    void SetAttribute(const char* name, char* data) { cdCanvasSetAttribute(canvas, name, data); }
    char* GetAttribute(const char* name) { return cdCanvasGetAttribute(canvas, name); }

    virtual int PlaySizeCallback(int /* w */, int /* h */, double /* w_mm */, double /* h_mm */) { return CD_CONTINUE; }
    /* interpretation */
    int Play(const Context& context, int xmin, int xmax, int ymin, int ymax, void *data) { 
      cdCanvasSetAttribute(canvas, "USERDATA", (char*)this);
      cdContextRegisterCallback(context.cd_context, CD_SIZECB, (cdCallback)PlaySize_CB);
      return cdCanvasPlay(canvas, context.cd_context, xmin, xmax, ymin, ymax, data); 
    }
    int wPlay(const Context& context, double xmin, double xmax, double ymin, double ymax, void *data) { 
      cdCanvasSetAttribute(canvas, "USERDATA", (char*)this);
      cdContextRegisterCallback(context.cd_context, CD_SIZECB, (cdCallback)PlaySize_CB);
      return wdCanvasPlay(canvas, context.cd_context, xmin, xmax, ymin, ymax, data); 
    }

    /* coordinates utilities */
    void GetSize(int &width, int &height, double &width_mm, double &height_mm) { cdCanvasGetSize(canvas, &width, &height, &width_mm, &height_mm); }
    int UpdateYAxis(int &y) { return cdCanvasUpdateYAxis(canvas, &y); }
    double UpdateYAxis(double &y) { return cdfCanvasUpdateYAxis(canvas, &y); }
    int InvertYAxis(int y) { return cdCanvasInvertYAxis(canvas, y); }
    double InvertYAxis(double y) { return cdfCanvasInvertYAxis(canvas, y); }
    void MM2Pixel(double mm_dx, double mm_dy, int &dx, int &dy) { cdCanvasMM2Pixel(canvas, mm_dx, mm_dy, &dx, &dy); }
    void MM2Pixel(double mm_dx, double mm_dy, double &dx, double &dy) { cdfCanvasMM2Pixel(canvas, mm_dx, mm_dy, &dx, &dy); }
    void Pixel2MM(int dx, int dy, double &mm_dx, double &mm_dy) { cdCanvasPixel2MM(canvas, dx, dy, &mm_dx, &mm_dy); }
    void Pixel2MM(double dx, double dy, double &mm_dx, double &mm_dy) { cdfCanvasPixel2MM(canvas, dx, dy, &mm_dx, &mm_dy); }
    void Origin(int x, int y) { cdCanvasOrigin(canvas, x, y); }
    void Origin(double x, double y) { cdfCanvasOrigin(canvas, x, y); }
    void GetOrigin(int &x, int &y) { cdCanvasGetOrigin(canvas, &x, &y); }
    void GetOrigin(double &x, double &y) { cdfCanvasGetOrigin(canvas, &x, &y); }

    /* coordinates transformation */
    void Transform(const double* matrix) { cdCanvasTransform(canvas, matrix); }
    double* GetTransform() { return cdCanvasGetTransform(canvas); }
    void TransformMultiply(const double* matrix) { cdCanvasTransformMultiply(canvas, matrix); }
    void TransformRotate(double angle) { cdCanvasTransformRotate(canvas, angle); }
    void TransformScale(double sx, double sy) { cdCanvasTransformScale(canvas, sx, sy); }
    void TransformTranslate(double dx, double dy) { cdCanvasTransformTranslate(canvas, dx, dy); }
    void TransformPoint(int x, int y, int &tx, int &ty) { cdCanvasTransformPoint(canvas, x, y, &tx, &ty); }
    void TransformPoint(double x, double y, double &tx, double &ty) { cdfCanvasTransformPoint(canvas, x, y, &tx, &ty); }

    /* world coordinate transformation */
    void wWindow(double xmin, double xmax, double  ymin, double ymax) { wdCanvasWindow(canvas, xmin, xmax, ymin, ymax); }
    void wGetWindow(double &xmin, double  &xmax, double  &ymin, double &ymax) { wdCanvasGetWindow(canvas, &xmin, &xmax, &ymin, &ymax); }
    void wViewport(int xmin, int xmax, int ymin, int ymax) { wdCanvasViewport(canvas, xmin, xmax, ymin, ymax); }
    void wGetViewport(int &xmin, int  &xmax, int  &ymin, int &ymax) { wdCanvasGetViewport(canvas, &xmin, &xmax, &ymin, &ymax); }
    void wWorld2Canvas(double xw, double yw, int &xv, int &yv) { wdCanvasWorld2Canvas(canvas, xw, yw, &xv, &yv); }
    void wWorld2CanvasSize(double hw, double vw, int &hv, int &vv) { wdCanvasWorld2CanvasSize(canvas, hw, vw, &hv, &vv); }
    void wCanvas2World(int xv, int yv, double &xw, double &yw) { wdCanvasCanvas2World(canvas, xv, yv, &xw, &yw); }
    void wSetTransform(double sx, double sy, double tx, double ty) { wdCanvasSetTransform(canvas, sx, sy, tx, ty); }
    void wGetTransform(double &sx, double &sy, double &tx, double &ty) { wdCanvasGetTransform(canvas, &sx, &sy, &tx, &ty); }
    void wTranslate(double dtx, double dty) { wdCanvasTranslate(canvas, dtx, dty); }
    void wScale(double dsx, double dsy) { wdCanvasScale(canvas, dsx, dsy); }


    /* clipping */
    int Clip(int mode) { return cdCanvasClip(canvas, mode); }
    void ClipArea(int xmin, int xmax, int ymin, int ymax) { cdCanvasClipArea(canvas, xmin, xmax, ymin, ymax); }
    void ClipArea(double xmin, double xmax, double ymin, double ymax) { cdfCanvasClipArea(canvas, xmin, xmax, ymin, ymax); }
    void wClipArea(double xmin, double xmax, double  ymin, double ymax) { wdCanvasClipArea(canvas, xmin, xmax, ymin, ymax); }
    int GetClipArea(int &xmin, int &xmax, int &ymin, int &ymax) { return cdCanvasGetClipArea(canvas, &xmin, &xmax, &ymin, &ymax); }
    int GetClipArea(double &xmin, double &xmax, double &ymin, double &ymax) { return cdfCanvasGetClipArea(canvas, &xmin, &xmax, &ymin, &ymax); }
    int wGetClipArea(double &xmin, double &xmax, double &ymin, double &ymax) { return wdCanvasGetClipArea(canvas, &xmin, &xmax, &ymin, &ymax); }

    /* clipping region */
    int IsPointInRegion(int x, int y) { return cdCanvasIsPointInRegion(canvas, x, y); }
    int wIsPointInRegion(double x, double y) { return wdCanvasIsPointInRegion(canvas, x, y); }
    void OffsetRegion(int x, int y) { cdCanvasOffsetRegion(canvas, x, y); }
    void wOffsetRegion(double x, double y) { wdCanvasOffsetRegion(canvas, x, y); }
    void GetRegionBox(int &xmin, int &xmax, int &ymin, int &ymax) { cdCanvasGetRegionBox(canvas, &xmin, &xmax, &ymin, &ymax); }
    void wGetRegionBox(double &xmin, double &xmax, double &ymin, double &ymax) { wdCanvasGetRegionBox(canvas, &xmin, &xmax, &ymin, &ymax); }
    int RegionCombineMode(int mode) { return cdCanvasRegionCombineMode(canvas, mode); }

    /* primitives */
    void Pixel(int x, int y, long color) { cdCanvasPixel(canvas, x, y, color); }
    void Pixel(double x, double y, long color) { cdfCanvasPixel(canvas, x, y, color); }
    void wPixel(double x, double y, long color) { wdCanvasPixel(canvas, x, y, color); }
    void Mark(int x, int y) { cdCanvasMark(canvas, x, y); }
    void Mark(double x, double y) { cdfCanvasMark(canvas, x, y); }
    void wMark(double x, double y) { wdCanvasMark(canvas, x, y); }

    void Begin(int mode) { cdCanvasBegin(canvas, mode); }
    void PathSet(int action) { cdCanvasPathSet(canvas, action); }
    void End() { cdCanvasEnd(canvas); }

    void Line(int x1, int y1, int x2, int y2) { cdCanvasLine(canvas, x1, y1, x2, y2); }
    void Line(double x1, double y1, double x2, double y2) { cdfCanvasLine(canvas, x1, y1, x2, y2); }
    void wLine(double x1, double y1, double x2, double y2) { wdCanvasLine(canvas, x1, y1, x2, y2); }
    void Vertex(int x, int y) { cdCanvasVertex(canvas, x, y); }
    void wVertex(double x, double y) { wdCanvasVertex(canvas, x, y); }
    void Vertex(double x, double y) { cdfCanvasVertex(canvas, x, y); }
    void Rect(int xmin, int xmax, int ymin, int ymax) { cdCanvasRect(canvas, xmin, xmax, ymin, ymax); }
    void Rect(double xmin, double xmax, double ymin, double ymax) { cdfCanvasRect(canvas, xmin, xmax, ymin, ymax); }
    void wRect(double xmin, double xmax, double ymin, double ymax) { wdCanvasRect(canvas, xmin, xmax, ymin, ymax); }
    void Box(int xmin, int xmax, int ymin, int ymax) { cdCanvasBox(canvas, xmin, xmax, ymin, ymax); }
    void Box(double xmin, double xmax, double ymin, double ymax) { cdfCanvasBox(canvas, xmin, xmax, ymin, ymax); }
    void wBox(double xmin, double xmax, double ymin, double ymax) { wdCanvasBox(canvas, xmin, xmax, ymin, ymax); }
    void Arc(int xc, int yc, int w, int h, double angle1, double angle2) { cdCanvasArc(canvas, xc, yc, w, h, angle1, angle2); }
    void Arc(double xc, double yc, double w, double h, double angle1, double angle2) { cdfCanvasArc(canvas, xc, yc, w, h, angle1, angle2); }
    void wArc(double xc, double yc, double w, double h, double angle1, double angle2) { wdCanvasArc(canvas, xc, yc, w, h, angle1, angle2); }
    void Sector(int xc, int yc, int w, int h, double angle1, double angle2) { cdCanvasSector(canvas, xc, yc, w, h, angle1, angle2); }
    void Sector(double xc, double yc, double w, double h, double angle1, double angle2) { cdfCanvasSector(canvas, xc, yc, w, h, angle1, angle2); }
    void wSector(double xc, double yc, double w, double h, double angle1, double angle2) { wdCanvasSector(canvas, xc, yc, w, h, angle1, angle2); }
    void Chord(double xc, double yc, double w, double h, double angle1, double angle2) { cdfCanvasChord(canvas, xc, yc, w, h, angle1, angle2); }
    void Chord(int xc, int yc, int w, int h, double angle1, double angle2) { cdCanvasChord(canvas, xc, yc, w, h, angle1, angle2); }
    void wChord(double xc, double yc, double w, double h, double angle1, double angle2) { wdCanvasChord(canvas, xc, yc, w, h, angle1, angle2); }
    void Text(int x, int y, const char* s) { cdCanvasText(canvas, x, y, s); }
    void Text(double x, double y, const char* s) { cdfCanvasText(canvas, x, y, s); }
    void wText(double x, double y, const char* s) { wdCanvasText(canvas, x, y, s); }

    /* attributes */
    void SetBackground(long color) { cdCanvasSetBackground(canvas, color); }
    void SeForeground(long color) { cdCanvasSetForeground(canvas, color); }
    long Background(long color) { return cdCanvasBackground(canvas, color); }
    long Foreground(long color) { return cdCanvasForeground(canvas, color); }
    int BackOpacity(int opacity) { return cdCanvasBackOpacity(canvas, opacity); }
    int WriteMode(int mode) { return cdCanvasWriteMode(canvas, mode); }
    int LineStyle(int style) { return cdCanvasLineStyle(canvas, style); }
    void LineStyleDashes(const int* dashes, int count) { cdCanvasLineStyleDashes(canvas, dashes, count); }
    int LineWidth(int width) { return cdCanvasLineWidth(canvas, width); }
    double wLineWidth(double width) { return wdCanvasLineWidth(canvas, width); }
    int LineJoin(int join) { return cdCanvasLineJoin(canvas, join); }
    int LineCap(int cap) { return cdCanvasLineCap(canvas, cap); }
    int InteriorStyle(int style) { return cdCanvasInteriorStyle(canvas, style); }
    int Hatch(int style) { return cdCanvasHatch(canvas, style); }
#ifdef __IM_PLUS_H
    void Stipple(const im::Image& stipple) { cdCanvasStippleImImage(canvas, stipple.GetHandle()); }
    unsigned char* GetStipple(int &n, int &m) { return cdCanvasGetStipple(canvas, &n, &m); }
    void Pattern(const im::Image& pattern) { cdCanvasPatternImImage(canvas, pattern.GetHandle()); }
    long* GetPattern(int& n, int& m) { return cdCanvasGetPattern(canvas, &n, &m); }
#endif    
    int FillMode(int mode) { return cdCanvasFillMode(canvas, mode); }
    int Font(const char* type_face, int style, int size) { return cdCanvasFont(canvas, type_face, style, size); }
    int wFont(const char* type_face, int style, double size) { return wdCanvasFont(canvas, type_face, style, size); }
    void GetFont(char *type_face, int &style, int &size) { cdCanvasGetFont(canvas, type_face, &style, &size); }
    void wGetFont(char *type_face, int &style, double &size) { wdCanvasGetFont(canvas, type_face, &style, &size); }
    char* NativeFont(const char* font) { return cdCanvasNativeFont(canvas, font); }
    int TextAlignment(int alignment) { return cdCanvasTextAlignment(canvas, alignment); }
    double TextOrientation(double angle) { return cdCanvasTextOrientation(canvas, angle); }
    int MarkType(int type) { return cdCanvasMarkType(canvas, type); }
    int MarkSize(int size) { return cdCanvasMarkSize(canvas, size); }
    double wMarkSize(double size) { return wdCanvasMarkSize(canvas, size); }

    /* vector text */
    void VectorText(int x, int y, const char* s) { cdCanvasVectorText(canvas, x, y, s); }
    void VectorText(double x, double y, const char* s) { cdfCanvasVectorText(canvas, x, y, s); }
    void wVectorText(double x, double y, const char* s) { wdCanvasVectorText(canvas, x, y, s); }
    void MultiLineVectorText(int x, int y, const char* s) { cdCanvasMultiLineVectorText(canvas, x, y, s); }
    void MultiLineVectorText(double x, double y, const char* s) { cdfCanvasMultiLineVectorText(canvas, x, y, s); }
    void wMultiLineVectorText(double x, double y, const char* s) { wdCanvasMultiLineVectorText(canvas, x, y, s); }

    /* vector text attributes */
    char *VectorFont(const char *filename) { return cdCanvasVectorFont(canvas, filename); }
    void VectorTextDirection(int x1, int y1, int x2, int y2) { cdCanvasVectorTextDirection(canvas, x1, y1, x2, y2); }
    void VectorTextDirection(double x1, double y1, double x2, double y2) { cdfCanvasVectorTextDirection(canvas, x1, y1, x2, y2); }
    void wVectorTextDirection(double x1, double y1, double x2, double y2) { wdCanvasVectorTextDirection(canvas, x1, y1, x2, y2); }
    double* VectorTextTransform(const double* matrix) { return cdCanvasVectorTextTransform(canvas, matrix); }
    void VectorTextSize(int size_x, int size_y, const char* s) { cdCanvasVectorTextSize(canvas, size_x, size_y, s); }
    void VectorTextSize(double size_x, double size_y, const char* s) { cdfCanvasVectorTextSize(canvas, size_x, size_y, s); }
    void wVectorTextSize(double size_x, double size_y, const char* s) { wdCanvasVectorTextSize(canvas, size_x, size_y, s); }
    int VectorCharSize(int size) { return cdCanvasVectorCharSize(canvas, size); }
    double VectorCharSize(double size) { return cdfCanvasVectorCharSize(canvas, size); }
    double wVectorCharSize(double size) { return wdCanvasVectorCharSize(canvas, size); }
    void VectorFontSize(double size_x, double size_y) { cdCanvasVectorFontSize(canvas, size_x, size_y); }
    void GetVectorFontSize(double &size_x, double &size_y) { cdCanvasGetVectorFontSize(canvas, &size_x, &size_y); }


    /* vector text properties */
    void GetVectorTextSize(const char* s, int &x, int &y) { cdCanvasGetVectorTextSize(canvas, s, &x, &y); }
    void GetVectorTextSize(const char* s, double &x, double &y) { cdfCanvasGetVectorTextSize(canvas, s, &x, &y); }
    void wGetVectorTextSize(const char* s, double &x, double &y) { wdCanvasGetVectorTextSize(canvas, s, &x, &y); }
    void GetVectorTextBounds(const char* s, int x, int y, int *rect) { cdCanvasGetVectorTextBounds(canvas, s, x, y, rect); }
    void GetVectorTextBounds(const char* s, double x, double y, double *rect) { cdfCanvasGetVectorTextBounds(canvas, s, x, y, rect); }
    void wGetVectorTextBounds(const char* s, double x, double y, double *rect) { wdCanvasGetVectorTextBounds(canvas, s, x, y, rect); }
    void GetVectorTextBox(int x, int y, const char *s, int &xmin, int &xmax, int &ymin, int &ymax) { cdCanvasGetVectorTextBox(canvas, x, y, s, &xmin, &xmax, &ymin, &ymax); }
    void GetVectorTextBox(double x, double y, const char *s, double &xmin, double &xmax, double &ymin, double &ymax) { cdfCanvasGetVectorTextBox(canvas, x, y, s, &xmin, &xmax, &ymin, &ymax); }
    void wGetVectorTextBox(double x, double y, const char *s, double &xmin, double &xmax, double &ymin, double &ymax) { wdCanvasGetVectorTextBox(canvas, x, y, s, &xmin, &xmax, &ymin, &ymax); }

    /* properties */
    void GetFontDim(int &max_width, int &height, int &ascent, int &descent) { cdCanvasGetFontDim(canvas, &max_width, &height, &ascent, &descent); }
    void wGetFontDim(double &max_width, double &height, double &ascent, double &descent) { wdCanvasGetFontDim(canvas, &max_width, &height, &ascent, &descent); }
    void GetTextSize(const char* s, int &width, int &height) { cdCanvasGetTextSize(canvas, s, &width, &height); }
    void wGetTextSize(const char* s, double &width, double &height) { wdCanvasGetTextSize(canvas, s, &width, &height); }
    void GetTextBox(int x, int y, const char* s, int &xmin, int &xmax, int &ymin, int &ymax) { cdCanvasGetTextBox(canvas, x, y, s, &xmin, &xmax, &ymin, &ymax); }
    void GetTextBox(double x, double y, const char* s, double &xmin, double &xmax, double &ymin, double &ymax) { cdfCanvasGetTextBox(canvas, x, y, s, &xmin, &xmax, &ymin, &ymax); }
    void wGetTextBox(double x, double y, const char* s, double &xmin, double &xmax, double &ymin, double &ymax) { wdCanvasGetTextBox(canvas, x, y, s, &xmin, &xmax, &ymin, &ymax); }
    void GetTextBounds(int x, int y, const char* s, int *rect) { cdCanvasGetTextBounds(canvas, x, y, s, rect); }
    void GetTextBounds(double x, double y, const char* s, double *rect) { cdfCanvasGetTextBounds(canvas, x, y, s, rect); }
    void wGetTextBounds(double x, double y, const char* s, double *rect) { wdCanvasGetTextBounds(canvas, x, y, s, rect); }
    int GetColorPlanes() { return cdCanvasGetColorPlanes(canvas); }


    /* client images */
#ifdef __IM_PLUS_H
    void PutImage(const im::Image& image, int x, int y, int w, int h) { cdCanvasPutImImage(canvas, image.GetHandle(), x, y, w, h); }
    void PutImage(const im::Image& image, double x, double y, double w, double h) { cdfCanvasPutImImage(canvas, image.GetHandle(), x, y, w, h); }
    void wPutImage(const im::Image& image, double x, double y, double w, double h) { wdCanvasPutImImage(canvas, image.GetHandle(), x, y, w, h); }

    void GetImage(im::Image& image, int x, int y) { cdCanvasGetImImage(canvas, image.GetHandle(), x, y); }
    void wGetImage(im::Image& image, double x, double y) { wdCanvasGetImImage(canvas, image.GetHandle(), x, y); }
#endif    
  };


  class CanvasImageRGB : public Canvas
  {
  public:
    CanvasImageRGB(int width, int height, bool has_alpha = false, double res = 0)
      : Canvas() { const char *alpha = "";
      if (has_alpha)
        alpha = "-a";

      if (res)
        canvas = cdCreateCanvasf(CD_IMAGERGB, "%dx%d %g %s", width, height, res, alpha);
      else
        canvas = cdCreateCanvasf(CD_IMAGERGB, "%dx%d %s", width, height, alpha); }
  };
#ifdef __IM_PLUS_H
  class CanvasImImage : public Canvas
  {
  public:
    CanvasImImage(im::Image& image, double res = 0)
      : Canvas() { canvas = cdCreateCanvas(CD_IMIMAGE, image.GetHandle());
      if (canvas && res)
        cdCanvasSetfAttribute(canvas, "RESOLUTION", "%g", res); 
    }
  };
#endif
  class CanvasMetafileEMF : public Canvas
  {
  public:
    CanvasMetafileEMF(const char* filename, int width, int height, double res = 0)
      : Canvas() { if (res)
        canvas = cdCreateCanvasf(CD_EMF, "\"%s\" %dx%d %g", filename, width, height, res);
      else
        canvas = cdCreateCanvasf(CD_EMF, "\"%s\" %dx%d", filename, width, height); 
    }
  };
  class CanvasMetafileWMF : public Canvas
  {
  public:
    CanvasMetafileWMF(const char* filename, int width, int height, double res = 0)
      : Canvas() { if (res)
        canvas = cdCreateCanvasf(CD_WMF, "\"%s\" %dx%d %g", filename, width, height, res);
      else
        canvas = cdCreateCanvasf(CD_WMF, "\"%s\" %dx%d", filename, width, height); 
    }
  };
  class CanvasMetafileMF : public Canvas
  {
  public:
    CanvasMetafileMF(const char* filename, double res = 0)
      : Canvas() { if (res)
        canvas = cdCreateCanvasf(CD_METAFILE, "\"%s\" %g", filename, res);
      else
        canvas = cdCreateCanvasf(CD_METAFILE, "\"%s\"", filename); 
    }
    CanvasMetafileMF(const char* filename, double width_mm, double height_mm, double res = 0)
      : Canvas() { if (res)
        canvas = cdCreateCanvasf(CD_METAFILE, "\"%s\" %gx%g %g", filename, width_mm, height_mm, res);
      else
        canvas = cdCreateCanvasf(CD_METAFILE, "\"%s\" %gx%g", filename, width_mm, height_mm); 
    }
  };
  class CanvasMetafileSVG : public Canvas
  {
  public:
    CanvasMetafileSVG(const char* filename, double res = 0)
      : Canvas() { if (res)
        canvas = cdCreateCanvasf(CD_SVG, "\"%s\" %g", filename, res);
      else
        canvas = cdCreateCanvasf(CD_SVG, "\"%s\"", filename); 
    }
    CanvasMetafileSVG(const char* filename, double width_mm, double height_mm, double res = 0)
      : Canvas() { if (res)
        canvas = cdCreateCanvasf(CD_SVG, "\"%s\" %gx%g %g", filename, width_mm, height_mm, res);
      else
        canvas = cdCreateCanvasf(CD_SVG, "\"%s\" %gx%g", filename, width_mm, height_mm); 
    }
  };
  class CanvasMetafileDebug : public Canvas
  {
  public:
    CanvasMetafileDebug(const char* filename, double res = 0)
      : Canvas() { if (res)
        canvas = cdCreateCanvasf(CD_DEBUG, "\"%s\" %g", filename, res);
      else
        canvas = cdCreateCanvasf(CD_DEBUG, "\"%s\"", filename); 
    }
    CanvasMetafileDebug(const char* filename, double width_mm, double height_mm, double res = 0)
      : Canvas() { if (res)
        canvas = cdCreateCanvasf(CD_DEBUG, "\"%s\" %gx%g %g", filename, width_mm, height_mm, res);
      else
        canvas = cdCreateCanvasf(CD_DEBUG, "\"%s\" %gx%g", filename, width_mm, height_mm); 
    }
  };
  class CanvasPrinter : public Canvas
  {
  public:
    CanvasPrinter(const char* name, bool show_dialog = false)
      : Canvas() { if (show_dialog)
        canvas = cdCreateCanvasf(CD_PRINTER, "%s -d", name);
      else
        canvas = cdCreateCanvasf(CD_PRINTER, "%s", name); 
    }
  };
  class CanvasOpenGL : public Canvas
  {
  public:
    CanvasOpenGL(int width, int height, double res = 0)
      : Canvas() { if (res)
        canvas = cdCreateCanvasf(CD_GL, "%dx%d %g", width, height, res);
      else
        canvas = cdCreateCanvasf(CD_GL, "%dx%d", width, height); 
    }
  };
  class CanvasPicture : public Canvas
  {
  public:
    CanvasPicture(double res = 0)
      : Canvas() { if (res)
        canvas = cdCreateCanvasf(CD_PICTURE, "%g", res);
      else
        canvas = cdCreateCanvas(CD_PICTURE, ""); 
    }
  };
  class CanvasMetafileDGN : public Canvas
  {
  public:
    CanvasMetafileDGN(const char* filename, double res = 0, bool polygon_filling = true, const char* seed_file = 0)
      : Canvas() { char* polygon_filling_str = "";
      if (!polygon_filling)
        polygon_filling_str = "-f";

      char* seed_file_str = "";
      if (seed_file)
        seed_file_str = "-s";
      else
        seed_file = "";

      if (res)
        canvas = cdCreateCanvasf(CD_DGN, "\"%s\" %g %s %s%s", filename, res, polygon_filling_str, seed_file_str, seed_file);
      else
        canvas = cdCreateCanvasf(CD_DGN, "\"%s\" %s %s%s", filename, polygon_filling_str, seed_file_str, seed_file); 
    }
    CanvasMetafileDGN(const char* filename, double width_mm, double height_mm, double res = 0, bool polygon_filling = true, const char* seed_file = 0)
      : Canvas() { char* polygon_filling_str = "";
      if (!polygon_filling)
        polygon_filling_str = "-f";

      char* seed_file_str = "";
      if (seed_file)
        seed_file_str = "-s";
      else
        seed_file = "";

      if (res)
        canvas = cdCreateCanvasf(CD_DGN, "\"%s\" %gx%g %g %s %s%s", filename, width_mm, height_mm, res, polygon_filling_str, seed_file_str, seed_file);
      else
        canvas = cdCreateCanvasf(CD_DGN, "\"%s\" %gx%g %s %s%s", filename, width_mm, height_mm, polygon_filling_str, seed_file_str, seed_file); 
    }
  };
  class CanvasMetafileDXF : public Canvas
  {
  public:
    CanvasMetafileDXF(const char* filename, double res = 0, bool acad2000 = true, double xmin = 0, double ymin = 0, double xmax = 0, double ymax = 0)
      : Canvas() { char* acad2000_str = "";
      if (acad2000)
        acad2000_str = "-ac2000";

      if (xmin || ymin || xmax || ymax)
      {   if (res)
          canvas = cdCreateCanvasf(CD_DXF, "\"%s\" %g %s -limits %g %g %g %g", filename, res, acad2000_str, xmin, ymin, xmax, ymax);
        else
          canvas = cdCreateCanvasf(CD_DXF, "\"%s\" %s -limits %g %g %g %g", filename, acad2000_str, xmin, ymin, xmax, ymax);
      }
      else
      {   if (res)
          canvas = cdCreateCanvasf(CD_DXF, "\"%s\" %g %s", filename, res, acad2000_str);
        else
          canvas = cdCreateCanvasf(CD_DXF, "\"%s\" %s", filename, acad2000_str);
      }
    }
    CanvasMetafileDXF(const char* filename, double width_mm, double height_mm, double res = 0, bool acad2000 = true, double xmin = 0, double ymin = 0, double xmax = 0, double ymax = 0)
      : Canvas() { char* acad2000_str = "";
      if (acad2000)
        acad2000_str = "-ac2000";

      if (xmin || ymin || xmax || ymax)
      {   if (res)
          canvas = cdCreateCanvasf(CD_DXF, "\"%s\" %gx%g %g %s -limits %g %g %g %g", filename, width_mm, height_mm, res, acad2000_str, xmin, ymin, xmax, ymax);
        else
          canvas = cdCreateCanvasf(CD_DXF, "\"%s\" %gx%g %s -limits %g %g %g %g", filename, width_mm, height_mm, acad2000_str, xmin, ymin, xmax, ymax);
      }
      else
      {   if (res)
          canvas = cdCreateCanvasf(CD_DXF, "\"%s\" %gx%g %g %s %s%s", filename, width_mm, height_mm, res, acad2000_str);
        else
          canvas = cdCreateCanvasf(CD_DXF, "\"%s\" %gx%g %s %s%s", filename, width_mm, height_mm, acad2000_str);
      }
    }
  };
  class CanvasMetafileCGM : public Canvas
  {
  public:
    CanvasMetafileCGM(const char* filename, double res = 0, bool text_encoding = false, const char* precision = 0, const char* description = 0)
      : Canvas() { char* text_encoding_str = "";
      if (text_encoding)
        text_encoding_str = "-d";

      if (!precision)   /* can be "16" (short), "32" (int), "F" (float), "D" (double) */
        precision = "16";

      char* description_str = "";
      if (description)
        description_str = "-s";
      else
        description = "";

      if (res)
        canvas = cdCreateCanvasf(CD_CGM, "\"%s\" %g %s -p%s %s%s", filename, res, text_encoding_str, precision, description_str, description);
      else
        canvas = cdCreateCanvasf(CD_CGM, "\"%s\" %s -p%s %s%s", filename, text_encoding_str, precision, description_str, description); 
    }
    CanvasMetafileCGM(const char* filename, double width_mm, double height_mm, double res = 0, bool text_encoding = false, const char* precision = 0, const char* description = 0)
      : Canvas() { char* text_encoding_str = "";
      if (text_encoding)
        text_encoding_str = "-t";

      if (!precision)   /* can be "16" (short), "32" (int), "F" (float), "D" (double) */
        precision = "16";

      char* description_str = "";
      if (description)
        description_str = "-d";
      else
        description = "";

      if (res)
        canvas = cdCreateCanvasf(CD_CGM, "\"%s\" %gx%g %g %s -p%s %s%s", filename, width_mm, height_mm, res, text_encoding_str, precision, description_str, description);
      else
        canvas = cdCreateCanvasf(CD_CGM, "\"%s\" %gx%g %s -p%s %s%s", filename, width_mm, height_mm, text_encoding_str, precision, description_str, description); 
    }
  };
  class CanvasMetafilePDF : public Canvas
  {
  public:
    CanvasMetafilePDF(const char* filename, int paper, int res_dpi = 0, bool landscape = false)
      : Canvas() { char* landscape_str = "";
      if (landscape)
        landscape_str = "-o";

      char* res_dpi_str = "";
      if (res_dpi)
        res_dpi_str = "-s";

      canvas = cdCreateCanvasf(CD_PDF, "\"%s\" -p%d %s%s %s", filename, paper, res_dpi_str, res_dpi, landscape_str); 
    }
    CanvasMetafilePDF(const char* filename, double width_mm, double height_mm, int res_dpi = 0, bool landscape = false)
      : Canvas() { char* landscape_str = "";
      if (landscape)
        landscape_str = "-o";

      char* res_dpi_str = "";
      if (res_dpi)
        res_dpi_str = "-s";

      canvas = cdCreateCanvasf(CD_PDF, "\"%s\" -w%g -h%g %s%s %s", filename, width_mm, height_mm, res_dpi_str, res_dpi, landscape_str); 
    }
  };
  class CanvasMetafilePS : public Canvas
  {
  public:
    CanvasMetafilePS(const char* filename, int paper, int res_dpi = 0, bool landscape = false)
      : Canvas() { char* landscape_str = "";
      if (landscape)
        landscape_str = "-o";

      char* res_dpi_str = "";
      if (res_dpi)
        res_dpi_str = "-s";

      canvas = cdCreateCanvasf(CD_PS, "\"%s\" -p%d %s%s %s", filename, paper, res_dpi_str, res_dpi, landscape_str); 
    }
    CanvasMetafilePS(const char* filename, double width_mm, double height_mm, int res_dpi = 0, bool landscape = false)
      : Canvas() { char* landscape_str = "";
      if (landscape)
        landscape_str = "-o";

      char* res_dpi_str = "";
      if (res_dpi)
        res_dpi_str = "-s";

      canvas = cdCreateCanvasf(CD_PS, "\"%s\" -w%g -h%g %s%s %s", filename, width_mm, height_mm, res_dpi_str, res_dpi, landscape_str); 
    }
    CanvasMetafilePS(const char* filename, int res_dpi = 0, bool landscape = false)
      : Canvas() { char* landscape_str = "";
      if (landscape)
        landscape_str = "-o";

      char* res_dpi_str = "";
      if (res_dpi)
        res_dpi_str = "-s";

      canvas = cdCreateCanvasf(CD_PS, "\"%s\" -e %s%s %s", filename, res_dpi_str, res_dpi, landscape_str); 
    }
  };
}

#endif
