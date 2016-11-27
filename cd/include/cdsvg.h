/** \file
 * \brief SVG driver
 *
 * See Copyright Notice in cd.h
 */

#ifndef __CD_SVG_H
#define __CD_SVG_H

#ifdef __cplusplus
extern "C" {
#endif

cdContext* cdContextSVG(void);

#define CD_SVG cdContextSVG()

#ifdef __cplusplus
}
#endif

#endif /* ifndef __CD_SVG_ */
