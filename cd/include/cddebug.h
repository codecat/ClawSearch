/** \file
 * \brief CD Debug driver
 *
 * See Copyright Notice in cd.h
 */

#ifndef __CD_DEBUG_H
#define __CD_DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif

cdContext* cdContextDebug(void);

#define CD_DEBUG cdContextDebug()

#ifdef __cplusplus
}
#endif

#endif /* ifndef __CD_DEBUG_H */


