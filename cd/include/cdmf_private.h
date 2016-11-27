/** \file
 * \brief CD Metafile driver private declarations
 *
 * See Copyright Notice in cd.h
 */

#ifndef __CDMF_PRIVATE_H
#define __CDMF_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

/* public part of the internal cdCtxCanvas */
typedef struct cdCanvasMF 
{
  cdCanvas* canvas;
  char* filename;       
  void* data;           
} cdCanvasMF;

void cdcreatecanvasMF(cdCanvas *canvas, void *data);
void cdinittableMF(cdCanvas* canvas);
void cdkillcanvasMF(cdCanvasMF *mfcanvas);

#ifdef __cplusplus
}
#endif

#endif 


