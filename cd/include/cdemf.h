/** \file
 * \brief EMF driver
 *
 * See Copyright Notice in cd.h
 */

#ifndef __CD_EMF_H
#define __CD_EMF_H

#ifdef __cplusplus
extern "C" {
#endif

cdContext* cdContextEMF(void);

#define CD_EMF cdContextEMF()

#ifdef __cplusplus
}
#endif

#endif
