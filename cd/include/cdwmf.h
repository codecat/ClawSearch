/** \file
 * \brief WMF driver
 *
 * See Copyright Notice in cd.h
 */

#ifndef __CD_WMF_H
#define __CD_WMF_H

#ifdef __cplusplus
extern "C" {
#endif

cdContext* cdContextWMF(void);

#define CD_WMF cdContextWMF()

#ifdef __cplusplus
}
#endif

#endif
