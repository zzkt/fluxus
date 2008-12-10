// Copyright (C) 2005 Dave Griffiths
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/time.h>
#include <iostream>
#include <string>

#ifdef FLX_QT_END
#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QtOpenGL>
#include <QKeyEvent>

#include "QFluxusWidget.h"
#endif // FLX_QT_END


#include "FluxusMain.h"
#include "Interpreter.h"
#include "Recorder.h"
#include "fixme.h"
#include "keys.h"

using namespace std;

static const string ENGINE_CALLBACK="(fluxus-frame-callback)";
static const string RESHAPE_CALLBACK="fluxus-reshape-callback";
static const string INPUT_CALLBACK="fluxus-input-callback";
static const string INPUT_RELEASE_CALLBACK="fluxus-input-release-callback";

FluxusMain *app = NULL;
EventRecorder *recorder = NULL;
int modifiers = 0;

void ReshapeCallback(int width, int height)
{
  app->Reshape(width,height);
  char code[256];
  snprintf(code,256,"(%s %d %d)",RESHAPE_CALLBACK.c_str(),width,height);
  Interpreter::Interpret(code);
}

void MouseCallback(int button, int state, int x, int y)
{
  app->Handle(-1, button, -1, state, x, y, 0);
  char code[256];
  snprintf(code,256,"(%s %d %d %d %d %d %d %d)",INPUT_CALLBACK.c_str(),-1,button,-1,state,x,y,0);
  Interpreter::Interpret(code);
  recorder->Record(RecorderMessage("mouse",x,y,button,state));
}

void MotionCallback(int x, int y)
{
  app->Handle(-1, -1, -1, -1, x, y, 0);
  char code[256];
  snprintf(code,256,"(%s %d %d %d %d %d %d %d)",INPUT_CALLBACK.c_str(),-1,-1,-1,-1,x,y,0);
  Interpreter::Interpret(code);
  recorder->Record(RecorderMessage("motion",x,y));
}

#if defined(FLX_GLUT_END)

void KeyboardCallback(unsigned char key,int x, int y)
{
  int mod=modifiers;
  if (recorder->GetMode()!=EventRecorder::PLAYBACK) mod=glutGetModifiers();
  app->Handle(key, -1, -1, -1, x, y, mod);
  char code[256];
  if (key > 0) { // key is 0 on ctrl+2
    snprintf(code,256,"(%s #\\%c %d %d %d %d %d %d)",INPUT_CALLBACK.c_str(),key,-1,-1,-1,x,y,mod);
    Interpreter::Interpret(code);
  }
  recorder->Record(RecorderMessage("keydown",key,mod));
}

void KeyboardUpCallback(unsigned char key,int x, int y)
{
  char code[256];
  if (key > 0) { // key is 0 on ctrl+2
    snprintf(code,256,"(%s #\\%c %d %d %d %d %d %d)",INPUT_RELEASE_CALLBACK.c_str(),key,-1,-1,-1,x,y,0);
    Interpreter::Interpret(code);
  }
  recorder->Record(RecorderMessage("keyup",key,0));
}

void SpecialKeyboardCallback(int key,int x, int y)
{
  int mod=modifiers;
  if (recorder->GetMode()!=EventRecorder::PLAYBACK) mod=glutGetModifiers();
  app->Handle(-1, -1, key, -1, x, y, mod);
  char code[256];
  snprintf(code,256,"(%s %d %d %d %d %d %d %d)",INPUT_CALLBACK.c_str(),-1,-1,key,-1,x,y,mod);
  Interpreter::Interpret(code);
  recorder->Record(RecorderMessage("specialkeydown",key,mod));
}

void SpecialKeyboardUpCallback(int key,int x, int y)
{
  //app->Handle( -1, -1, key, 1, x, y);
  char code[256];
  snprintf(code,256,"(%s %d %d %d %d %d %d %d)",INPUT_RELEASE_CALLBACK.c_str(),-1,-1,key,-1,x,y,0);
  Interpreter::Interpret(code);
  recorder->Record(RecorderMessage("specialkeyup",key,0));
}

void IdleCallback()
{
  glutPostRedisplay();
}
#elif defined(FLX_QT_END)

void QFluxusWidget::keyPressEvent(QKeyEvent * ev)
{
    FIXME("Implement record/playback in Qt");
    int mod=modifiers;
    if (recorder->GetMode()!=EventRecorder::PLAYBACK) 
        mod=ev->modifiers();

    int key = ev->key();
    if (key>=Qt::Key_A && key<=Qt::Key_Z && !(mod&GLEDITOR_CTRL_MOD)) {
        key = (int)ev->text()[0].toAscii();
    }
    QPoint cur = QCursor::pos();

    app->Handle(key, -1, key, -1, 
                cur.x(), cur.y(),
                mod);
    char code[256];
    snprintf(code,256,"(%s %d %d %d %d %d %d %d)",
             INPUT_CALLBACK.c_str(),
             key, -1, key, -1,
             cur.x(), cur.y(),
             mod);
    Interpreter::Interpret(code);
    recorder->Record(RecorderMessage("keydown",key,mod));
}

void QFluxusWidget::keyReleaseEvent(QKeyEvent * ev)
{
    int mod=modifiers;
    if (recorder->GetMode()!=EventRecorder::PLAYBACK) 
        mod=ev->modifiers();

    int key = ev->key();
    if (key>=Qt::Key_A && key<=Qt::Key_Z && !(mod&GLEDITOR_CTRL_MOD)) {
        key = (int)ev->text()[0].toAscii();
    }

    QPoint cur = QCursor::pos();

    char code[256];
    snprintf(code,256,"(%s %d %d %d %d %d %d %d)",
             INPUT_RELEASE_CALLBACK.c_str(),
             key, -1, key, -1,
             cur.x(), cur.y(),
             0);
    Interpreter::Interpret(code);
    recorder->Record(RecorderMessage("keyup",ev->key(),0));
}

void QFluxusWidget::mouseMoveEvent(QMouseEvent * ev)
{
    MotionCallback(ev->x(), ev->y());
}

inline int glutifyButton(Qt::MouseButton button)
{
    int but = -1;

    switch(button) {
    case Qt::LeftButton: but = 0; break;
    case Qt::MidButton: but = 1; break;
    case Qt::RightButton: but = 2; break;
    default: break;
    }
    return but;
}

void QFluxusWidget::mousePressEvent(QMouseEvent * ev)
{

    MouseCallback(glutifyButton(ev->button()), 0, ev->x(), ev->y());
}

void QFluxusWidget::mouseReleaseEvent(QMouseEvent * ev)
{
    MouseCallback(glutifyButton(ev->button()), 1, ev->x(), ev->y());
}

void toggleFullScreen()
{
    QWidget * w = app->mainWin();
    w->setWindowState(w->windowState() ^ Qt::WindowFullScreen);
}
#endif

void resize(int w, int h)
{
#ifdef FLX_QT_END
    app->mainWin()->resize(w,h);
#elif defined(FLX_GLUT_END)
	glutReshapeWindow(w,h);
#endif
}

void DoRecorder()
{
#if defined(FLX_GLUT_END)
  list<RecorderMessage> events;
  if (recorder->Get(events))
    {
      for (list<RecorderMessage>::iterator i=events.begin(); i!=events.end(); i++)
	{
	  if (i->Name=="keydown")
	    {
	      modifiers=i->Mod;
	      KeyboardCallback(i->Data,0,0);
	    }
	  else if (i->Name=="keyup") KeyboardUpCallback(i->Data,0,0);
	  else if (i->Name=="specialkeydown")
	    {
	      modifiers=i->Mod;
	      SpecialKeyboardCallback(i->Data,0,0);
	    }
	  else if (i->Name=="specialkeyup") SpecialKeyboardUpCallback(i->Data,0,0);
	  else if (i->Name=="mouse") MouseCallback(i->Button,i->State,i->Data,i->Mod);
	  else if (i->Name=="motion") MotionCallback(i->Data,i->Mod);
	}
    }
  recorder->UpdateClock();
#else
  FIXME("Implement recorder in Qt");
#endif
}

void DisplayCallback()
{
  string fragment = app->GetScriptFragment();
  if (fragment!="")
    {
        Interpreter::Interpret(fragment);
    }

  if (!Interpreter::Interpret(ENGINE_CALLBACK))
    {
      // the callback has failed, so clear the screen so we can fix the error...
      glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    }

  app->Render();
  
#if defined(FLX_GLUT_END)
  glutSwapBuffers();
  DoRecorder();
#elif !defined(FLX_QT_END)
  // We don't have to swap buffers in Qt, because Qt will do that for us at apropriate moment
  // Which means that this is a wrong place to DoRecorder()
  // FIXME find the right place then...
#error Unknown backend
#endif

}

void ExitHandler()
{
  delete app;
}

int run(Scheme_Env* se, int argc, char *argv[])
{
	// we create our own Scheme_Env in here, as we need 
	// to be able to reset it with F6. Seems to be ok to ignore se...
  Interpreter::Register();
  Interpreter::Initialise();

  srand(time(NULL));
  
  char windowtitle[256];
  snprintf(windowtitle,256,"fluxus scratchpad %d.%d",FLUXUS_MAJOR_VERSION,FLUXUS_MINOR_VERSION);

#ifdef FLX_GLUT_END

  unsigned int flags = GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL;
#ifdef ACCUM_BUFFER
  flags|=GLUT_ACCUM;
#endif
#ifdef STEREODEFAULT
  flags|=GLUT_STEREO;
#endif

  // init OpenGL
  glutInit(&argc,argv);
  glutInitWindowSize(720,576);
  glutInitDisplayMode(flags);
  glutCreateWindow(windowtitle);
  glutDisplayFunc(DisplayCallback);
  glutReshapeFunc(ReshapeCallback);
  glutKeyboardFunc(KeyboardCallback);
  glutSpecialFunc(SpecialKeyboardCallback);
  glutMouseFunc(MouseCallback);
  glutMotionFunc(MotionCallback);
  glutIdleFunc(IdleCallback);
  glutKeyboardUpFunc(KeyboardUpCallback);
  glutSpecialUpFunc(SpecialKeyboardUpCallback);

#elif defined(FLX_QT_END)

  QApplication qapp(argc, argv);
  QWidget window;
  window.setWindowTitle(windowtitle);

  QHBoxLayout * layout = new QHBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  
  QGLFormat fmt(QGL::DoubleBuffer | QGL::DepthBuffer | QGL::Rgba | QGL::StencilBuffer);
#ifdef ACCUM_BUFFER
  fmt.setAccum(true);
#endif
#ifdef STEREODEFAULT
  fmt.setStereo(true);
#endif
  QGLFormat::setDefaultFormat(fmt);

  QFluxusWidget *gfxView = new QFluxusWidget;

  gfxView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  layout->addWidget(gfxView);

  window.setLayout(layout);

#else
#error Unknown backend, define one of FLX_{GLUT,QT}_END
#endif 

  app = new FluxusMain(720,576);
#ifdef FLX_QT_END
  app->setMainWin(&window);
#endif
  atexit(ExitHandler);

  recorder = new EventRecorder;

  int arg=1;
  int currentEditor=0;
  bool exe=false;

  while(arg<argc)
    {
      if (!strcmp(argv[arg],"-v"))
	{
	  cerr<<"fluxus version: "<<FLUXUS_MAJOR_VERSION<<"."<<FLUXUS_MINOR_VERSION<<endl;
	  exit(1);
	}
      else if (!strcmp(argv[arg],"-h"))
	{
	  cerr<<"fluxus [options] [filename1] [filename2] ..."<<endl;
	  cerr<<"options:"<<endl;
	  cerr<<"-h : help"<<endl;
	  cerr<<"-v : version info"<<endl;
	  cerr<<"-r filename : record keypresses"<<endl;
	  cerr<<"-p filename : playback keypresses"<<endl;
	  cerr<<"-d time : set delta time between frames for keypress playback"<<endl;
	  cerr<<"-lang language : sets the PLT language to use (may not work)"<<endl;
	  cerr<<"-fs : startup in fullscreen mode"<<endl;
	  cerr<<"-hm : hide the mouse pointer on startup"<<endl;
	  cerr<<"-x : execute and hide script at startup"<<endl;
	  exit(1);
	}
      else if (!strcmp(argv[arg],"-r"))
	{
	  if (arg+1 < argc)
	    {
	      recorder->SetFilename(argv[arg+1]);
	      recorder->SetMode(EventRecorder::RECORD);
	      arg++;
	    }
	}
      else if (!strcmp(argv[arg],"-p"))
	{
	  if (arg+1 < argc)
	    {
	      recorder->SetFilename(argv[arg+1]);
	      recorder->Load();
	      recorder->SetMode(EventRecorder::PLAYBACK);
	      arg++;
	    }
	}
      else if (!strcmp(argv[arg],"-d"))
	{
	  if (arg+1 < argc)
	    {
	      recorder->SetDelta(atof(argv[arg+1]));
	      arg++;
	    }
	}
      else if (!strcmp(argv[arg],"-lang"))
	{
	  if (arg+1 < argc)
	    {
	      Interpreter::SetLanguage(argv[arg+1]);
	      arg++;
	    }
	}
      else if (!strcmp(argv[arg],"-fs"))
	{
#ifdef FLX_GLUT_END
	  glutFullScreen();
#else
        window.setWindowState(Qt::WindowFullScreen);
#endif
	}
      else if (!strcmp(argv[arg],"-hm"))
	{
	  app->ToggleCursor();
	}
      else if (!strcmp(argv[arg],"-x"))
	{
	  exe=true;
	}
      else
	{
	  if (currentEditor<fluxus::NUM_EDITORS)
	    {
	      app->SetCurrentEditor(currentEditor); // flip it out of the repl
	      app->LoadScript(argv[arg]);
	      if (exe && currentEditor==0)
		{
		  app->Execute();
		  app->HideScript();
		}
	      currentEditor++;
	    }
	}
      arg++;
    }

#if defined(FLX_GLUT_END)
  glutMainLoop();
  return 0;
#elif defined(FLX_QT_END)

  // update timer
  QTimer *timer = new QTimer();
  QObject::connect(timer, SIGNAL(timeout()), gfxView, SLOT(updateGL()));
  timer->start(50); 

  window.show();
  return qapp.exec();
#else
#error Unknown backend
#endif

}

int main(int argc, char *argv[])
{
  return scheme_main_setup(1, run, argc, argv);
}



