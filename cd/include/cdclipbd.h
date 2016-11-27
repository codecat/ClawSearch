/** \file
 * \brief Clipboard driver
 *
 * See Copyright Notice in cd.h
 */

#ifndef __CD_CLIPBOARD_H
#define __CD_CLIPBOARD_H

#ifdef __cplusplus
extern "C" {
#endif

cdContext* cdContextClipboard(void);

#define CD_CLIPBOARD cdContextClipboard()

#ifdef __cplusplus
}
#endif

#endif
