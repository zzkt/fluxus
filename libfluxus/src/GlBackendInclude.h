// no reinclude protection - we could be included with different preprocessor symbols
// reinclusion of this header shouldn't do any harm

// Gl/Glu are only included when FLX_NO_GL isn't defined
// Glut is only included when FLX_GLUT_END is defined and FLX_NO_GLUT isn't

#ifndef __APPLE__

#ifndef FLX_NO_GL
# include "GL/gl.h"
# include "GL/glu.h"
#endif

#ifndef FLX_NO_GLUT
# ifdef FLX_GLUT_END
#   include "GL/glut.h"
# endif
#endif

#else // !__APPLE__

#ifndef FLX_NO_GL
# include "OpenGL/gl.h"
# include "OpenGL/glu.h"
#endif

#ifndef FLX_NO_GLUT
# ifdef FLX_GLUT_END
#   include "GLUT/glut.h"
# endif
#endif

#endif //__APPLE__

