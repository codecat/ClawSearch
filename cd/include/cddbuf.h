/** \file
 * \brief Double Buffer driver
 *
 * See Copyright Notice in cd.h
 */

#ifndef __CD_DBUF_H
#define __CD_DBUF_H

#ifdef __cplusplus
extern "C" {
#endif

cdContext* cdContextDBuffer(void);

#define CD_DBUFFER cdContextDBuffer()

#ifdef __cplusplus
}
#endif

#endif /* ifndef __CD_DBUF_ */

