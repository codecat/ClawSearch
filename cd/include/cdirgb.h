/** \file
 * \brief IMAGERGB driver
 *
 * See Copyright Notice in cd.h
 */

#ifndef __CD_IRGB_H
#define __CD_IRGB_H

#ifdef __cplusplus
extern "C" {
#endif

cdContext* cdContextImageRGB(void);
cdContext* cdContextDBufferRGB(void);

#define CD_IMAGERGB cdContextImageRGB()
#define CD_DBUFFERRGB cdContextDBufferRGB()

/* DEPRECATED functions, use REDIMAGE, GREENIMAGE, 
   BLUEIMAGE, and ALPHAIMAGE attributes. */
unsigned char* cdRedImage(cdCanvas* cnv);
unsigned char* cdGreenImage(cdCanvas* cnv);
unsigned char* cdBlueImage(cdCanvas* cnv);
unsigned char* cdAlphaImage(cdCanvas* cnv);


#ifdef __cplusplus
}
#endif

#endif /* ifndef __CD_IRGB_ */
