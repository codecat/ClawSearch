/** \file
 * \brief DXF driver
 *
 * See Copyright Notice in cd.h
 */

#ifndef __CD_DXF_H
#define __CD_DXF_H

#ifdef __cplusplus
extern "C" {
#endif

cdContext* cdContextDXF(void);

#define CD_DXF cdContextDXF()

#ifdef __cplusplus
}
#endif

#endif /* ifndef __CD_DXF_ */
