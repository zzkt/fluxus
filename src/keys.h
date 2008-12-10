#ifndef _KEYS_H_
#define _KEYS_H_

#include <fixme.h>

#ifdef FLX_QT_END
#  include <Qt>
#elif defined(FLX_GLUT_END)
#  define FLX_NO_GL
#    include "GlBackendInclude.h"
#  undef FLX_NO_GL
#endif

#if defined(FLX_GLUT_END)
#  define FLX_GLUT_QT(glut_val, qt_val) glut_val
#  ifndef __APPLE__
#    define FLX_GLUTa_QT(glut_val_apple, glut_val_x11, qt_val) glut_val_apple
#  else
#    define FLX_GLUTa_QT(glut_val_apple, glut_val_x11, qt_val) glut_val_x11
#  endif
#else
#  define FLX_GLUT_QT(glut_val, qt_val) qt_val
#  define FLX_GLUTa_QT(glut_val_apple, glut_val_x11, qt_val) qt_val
#endif


#define GLEDITOR_DELETE         FLX_GLUTa_QT(127, 8, Qt::Key_Delete)
#define GLEDITOR_BACKSPACE      FLX_GLUTa_QT(8, 127, Qt::Key_Backspace)
#define GLEDITOR_TAB            FLX_GLUT_QT(9,  Qt::Key_Tab)
#define GLEDITOR_RETURN         FLX_GLUT_QT(13, Qt::Key_Return)
#define GLEDITOR_ESCAPE         FLX_GLUT_QT(27, Qt::Key_Escape)

#define GLEDITOR_F1             FLX_GLUT_QT(GLUT_KEY_F1,  Qt::Key_F1)
#define GLEDITOR_F2             FLX_GLUT_QT(GLUT_KEY_F2,  Qt::Key_F2)
#define GLEDITOR_F3             FLX_GLUT_QT(GLUT_KEY_F3,  Qt::Key_F3)
#define GLEDITOR_F4             FLX_GLUT_QT(GLUT_KEY_F4,  Qt::Key_F4)
#define GLEDITOR_F5             FLX_GLUT_QT(GLUT_KEY_F5,  Qt::Key_F5)
#define GLEDITOR_F6             FLX_GLUT_QT(GLUT_KEY_F6,  Qt::Key_F6)
#define GLEDITOR_F7             FLX_GLUT_QT(GLUT_KEY_F7,  Qt::Key_F7)
#define GLEDITOR_F8             FLX_GLUT_QT(GLUT_KEY_F8,  Qt::Key_F8)
#define GLEDITOR_F9             FLX_GLUT_QT(GLUT_KEY_F9,  Qt::Key_F9)
#define GLEDITOR_F10            FLX_GLUT_QT(GLUT_KEY_F10, Qt::Key_F10)
#define GLEDITOR_F11            FLX_GLUT_QT(GLUT_KEY_F11, Qt::Key_F11)
#define GLEDITOR_F12            FLX_GLUT_QT(GLUT_KEY_F12, Qt::Key_F12)

#define GLEDITOR_LEFT           FLX_GLUT_QT(GLUT_KEY_LEFT, Qt::Key_Left)
#define GLEDITOR_RIGHT          FLX_GLUT_QT(GLUT_KEY_RIGHT, Qt::Key_Right)
#define GLEDITOR_UP             FLX_GLUT_QT(GLUT_KEY_UP, Qt::Key_Up)
#define GLEDITOR_DOWN           FLX_GLUT_QT(GLUT_KEY_DOWN, Qt::Key_Down)
#define GLEDITOR_HOME           FLX_GLUT_QT(GLUT_KEY_HOME, Qt::Key_Home)
#define GLEDITOR_END            FLX_GLUT_QT(GLUT_KEY_END, Qt::Key_End)
#define GLEDITOR_PAGE_UP        FLX_GLUT_QT(GLUT_KEY_PAGE_UP, Qt::Key_PageUp)
#define GLEDITOR_PAGE_DOWN      FLX_GLUT_QT(GLUT_KEY_PAGE_DOWN, Qt::Key_PageDown)

// Modifiers
#define GLEDITOR_CTRL_MOD       FLX_GLUT_QT(GLUT_ACTIVE_CTRL, Qt::ControlModifier)
#define GLEDITOR_SHIFT_MOD       FLX_GLUT_QT(GLUT_ACTIVE_SHIFT, Qt::ShiftModifier)

// Ctrl+something. This is required as glut sends usless codes
#define GLEDITOR_CUT            FLX_GLUT_QT(24, Qt::Key_X)
#define GLEDITOR_COPY           FLX_GLUT_QT(3,  Qt::Key_C)
#define GLEDITOR_PASTE          FLX_GLUT_QT(22, Qt::Key_V)
#define GLEDITOR_PLUS           FLX_GLUT_QT(61, Qt::Key_Plus)
#define GLEDITOR_MINUS          FLX_GLUT_QT(45, Qt::Key_Minus)

#define GLEDITOR_CTRL_A         FLX_GLUT_QT( 1, Qt::Key_A)
#define GLEDITOR_CTRL_B         FLX_GLUT_QT( 2, Qt::Key_B)
#define GLEDITOR_CTRL_C         FLX_GLUT_QT( 3, Qt::Key_C)
#define GLEDITOR_CTRL_D         FLX_GLUT_QT( 4, Qt::Key_D)
#define GLEDITOR_CTRL_E         FLX_GLUT_QT( 5, Qt::Key_E)
#define GLEDITOR_CTRL_F         FLX_GLUT_QT( 6, Qt::Key_F)
#define GLEDITOR_CTRL_G         FLX_GLUT_QT( 7, Qt::Key_G)
#define GLEDITOR_CTRL_H         FLX_GLUT_QT( 8, Qt::Key_H)
#define GLEDITOR_CTRL_I         FLX_GLUT_QT( 9, Qt::Key_I)
#define GLEDITOR_CTRL_J         FLX_GLUT_QT(10, Qt::Key_J)
#define GLEDITOR_CTRL_K         FLX_GLUT_QT(11, Qt::Key_K)
#define GLEDITOR_CTRL_L         FLX_GLUT_QT(12, Qt::Key_L)
#define GLEDITOR_CTRL_M         FLX_GLUT_QT(13, Qt::Key_M)
#define GLEDITOR_CTRL_N         FLX_GLUT_QT(14, Qt::Key_N)
#define GLEDITOR_CTRL_O         FLX_GLUT_QT(15, Qt::Key_O)
#define GLEDITOR_CTRL_P         FLX_GLUT_QT(16, Qt::Key_P)
#define GLEDITOR_CTRL_Q         FLX_GLUT_QT(17, Qt::Key_Q)
#define GLEDITOR_CTRL_R         FLX_GLUT_QT(18, Qt::Key_R)
#define GLEDITOR_CTRL_S         FLX_GLUT_QT(19, Qt::Key_S)
#define GLEDITOR_CTRL_T         FLX_GLUT_QT(20, Qt::Key_T)
#define GLEDITOR_CTRL_U         FLX_GLUT_QT(21, Qt::Key_U)
#define GLEDITOR_CTRL_V         FLX_GLUT_QT(22, Qt::Key_V)
#define GLEDITOR_CTRL_W         FLX_GLUT_QT(23, Qt::Key_W)
#define GLEDITOR_CTRL_X         FLX_GLUT_QT(24, Qt::Key_X)
#define GLEDITOR_CTRL_Y         FLX_GLUT_QT(25, Qt::Key_Y)
#define GLEDITOR_CTRL_Z         FLX_GLUT_QT(26, Qt::Key_Z)

#define GLEDITOR_CTRL_0         FLX_GLUTa_QT(48, 48, Qt::Key_0)
#define GLEDITOR_CTRL_1         FLX_GLUTa_QT(49, 49, Qt::Key_1)
#define GLEDITOR_CTRL_2         FLX_GLUTa_QT(50,  0, Qt::Key_2)
#define GLEDITOR_CTRL_3         FLX_GLUTa_QT(51, 27, Qt::Key_3)
#define GLEDITOR_CTRL_4         FLX_GLUTa_QT(52, 28, Qt::Key_4)
#define GLEDITOR_CTRL_5         FLX_GLUTa_QT(53, 29, Qt::Key_5)
#define GLEDITOR_CTRL_6         FLX_GLUTa_QT(54, 30, Qt::Key_6)
#define GLEDITOR_CTRL_7         FLX_GLUTa_QT(55, 31, Qt::Key_7)
#define GLEDITOR_CTRL_8         FLX_GLUTa_QT(56,127, Qt::Key_8)
#define GLEDITOR_CTRL_9         FLX_GLUTa_QT(57, 57, Qt::Key_9)

#endif
