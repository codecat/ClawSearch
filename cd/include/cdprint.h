/** \file
 * \brief Printer driver
 *
 * See Copyright Notice in cd.h
 */

#ifndef __CD_PRINTER_H
#define __CD_PRINTER_H

#ifdef __cplusplus
extern "C" {
#endif

cdContext* cdContextPrinter(void);

#define CD_PRINTER cdContextPrinter()

#ifdef __cplusplus
}
#endif

#endif /* ifndef __CD_PRINTER_ */

