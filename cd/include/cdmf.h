/** \file
 * \brief CD Metafile driver
 *
 * See Copyright Notice in cd.h
 */

#ifndef __CD_MF_H
#define __CD_MF_H

#ifdef __cplusplus
extern "C" {
#endif

cdContext* cdContextMetafile(void);

#define CD_METAFILE cdContextMetafile()

#ifdef __cplusplus
}
#endif

#endif /* ifndef __CD_MF_ */


