/** \file
 * \brief imImage driver
 *
 * See Copyright Notice in cd.h
 */

#ifndef __CD_IM_H
#define __CD_IM_H

#ifdef __cplusplus
extern "C" {
#endif

cdContext* cdContextImImage(void);

#define CD_IMIMAGE cdContextImImage()


#ifdef __IM_IMAGE_H
void cdCanvasPatternImImage(cdCanvas* canvas, const imImage* image);
void cdCanvasStippleImImage(cdCanvas* canvas, const imImage* image);
void cdCanvasPutImImage(cdCanvas* canvas, const imImage* image, int x, int y, int w, int h);
void cdCanvasGetImImage(cdCanvas* canvas, imImage* image, int x, int y);
void cdfCanvasPutImImage(cdCanvas* canvas, const imImage* image, double x, double y, double w, double h);
void wdCanvasPutImImage(cdCanvas* canvas, const imImage* image, double x, double y, double w, double h);
void wdCanvasGetImImage(cdCanvas* canvas, imImage* image, double x, double y);
#endif

#ifdef __cplusplus
}
#endif

#endif /* ifndef __CD_IM_ */
