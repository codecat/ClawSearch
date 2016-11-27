/** \file
 * \brief PS driver
 *
 * See Copyright Notice in cd.h
 */

#ifndef __CD_PS_H
#define __CD_PS_H

#ifdef __cplusplus
extern "C" {
#endif

cdContext* cdContextPS(void);

#define CD_PS cdContextPS()

#ifdef __cplusplus
}
#endif

#endif /* ifndef __CD_PS_ */

