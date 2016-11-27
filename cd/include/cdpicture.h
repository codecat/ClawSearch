/** \file
 * \brief CD Picture driver
 *
 * See Copyright Notice in cd.h
 */

#ifndef __CD_PICTURE_H
#define __CD_PICTURE_H

#ifdef __cplusplus
extern "C" {
#endif

cdContext* cdContextPicture(void);

#define CD_PICTURE cdContextPicture()

#ifdef __cplusplus
}
#endif

#endif
