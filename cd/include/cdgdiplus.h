/** \file
 * \brief GDI+ Control
 *
 * See Copyright Notice in cd.h
 */
 
#ifndef __CD_GDIPLUS_H
#define __CD_GDIPLUS_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
void cdInitGdiPlus(void);  /* old function, replaced by cdInitContextPlus */
#else
#define cdInitGdiPlus() (0)
#endif

/* Windows GDI+ Adicional Polygons */
#define CD_SPLINE       (CD_POLYCUSTOM+0)
#define CD_FILLSPLINE   (CD_POLYCUSTOM+1)
#define CD_FILLGRADIENT (CD_POLYCUSTOM+2)

#ifdef __cplusplus
}
#endif

#endif /* ifndef _CD_GDIPLUS_ */
