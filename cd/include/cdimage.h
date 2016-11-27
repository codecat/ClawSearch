/** \file
 * \brief Server Image driver
 *
 * See Copyright Notice in cd.h
 */

#ifndef __CD_IMAGE_H
#define __CD_IMAGE_H

#ifdef __cplusplus
extern "C" {
#endif

cdContext* cdContextImage(void);

#define CD_IMAGE cdContextImage()

#ifdef __cplusplus
}
#endif

#endif /* ifndef __CD_IMAGE_ */

