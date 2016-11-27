/** \file
* \brief OpenGL driver
*
* See Copyright Notice in cd.h
*/

#ifndef __CD_GL_H
#define __CD_GL_H

#ifdef __cplusplus
extern "C" {
#endif

cdContext* cdContextGL(void);

#define CD_GL cdContextGL()

#ifdef __cplusplus
}
#endif

#endif /* ifndef __CD_GL_ */

