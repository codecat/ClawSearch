/** \file
 * \brief NativeWindow driver
 *
 * See Copyright Notice in cd.h
 */

#ifndef __CD_NATIVE_H
#define __CD_NATIVE_H

#ifdef __cplusplus
extern "C" {
#endif

cdContext* cdContextNativeWindow(void);

#define CD_NATIVEWINDOW cdContextNativeWindow()

void cdGetScreenSize(int *width, int *height, double *width_mm, double *height_mm);
int cdGetScreenColorPlanes(void);

#ifdef __cplusplus
}
#endif

#endif /* ifndef __CD_NATIVE_ */

