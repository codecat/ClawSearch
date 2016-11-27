/** \file
 * \brief PDF driver
 *
 * See Copyright Notice in cd.h
 */

#ifndef __CD_PDF_H
#define __CD_PDF_H

#ifdef __cplusplus
extern "C" {
#endif

cdContext* cdContextPDF(void);

#define CD_PDF cdContextPDF()
        
#ifdef __cplusplus
}
#endif

#endif /* ifndef _CD_PDF_ */

