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
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <cstdio>
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>
#include <plt/scheme.h>
#include "FluxusMain.h"

using namespace std;

static const string INIT_SCRIPT=".fluxus-plt.scm";
static const string ENGINE_CALLBACK="(fluxus-frame-callback)";
static const string RESHAPE_CALLBACK="fluxus-reshape-callback";
static const string INPUT_CALLBACK="fluxus-input-callback";
static const string INPUT_RELEASE_CALLBACK="fluxus-input-release-callback";
static const string STARTUP_SCRIPT="(load (string-append (path->string (find-system-path 'home-dir)) \".fluxus/startup.scm\"))";

FluxusMain *app = NULL;
Scheme_Env *scheme; 

void RunScheme(const string &str, bool abort=false)
{	
  	Scheme_Object *curout=NULL;
 	mz_jmp_buf * volatile save, fresh;
	
	save = scheme_current_thread->error_buf;
    scheme_current_thread->error_buf = &fresh;
	
    if (scheme_setjmp(scheme_error_buf)) 
	{
		scheme_current_thread->error_buf = save;
		if (abort) exit(-1);
    } 
	else 
	{
		Scheme_Object *v = scheme_eval_string_all(str.c_str(), scheme, 1);
		scheme_current_thread->error_buf = save;
    }
}


void DisplayCallback()
{    
	//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	string fragment = app->GetScriptFragment();
    if (fragment!="")
    {
		RunScheme(fragment);
    }
	
	RunScheme(ENGINE_CALLBACK);
		
	app->Render();	
	glutSwapBuffers();
}

void ReshapeCallback(int width, int height)
{
	app->Reshape(width,height);
	char code[256];
	snprintf(code,256,"(%s %d %d)",RESHAPE_CALLBACK.c_str(),width,height);
	RunScheme(code);
}

void KeyboardCallback(unsigned char key,int x, int y)
{
	app->Handle(key, -1, -1, -1, x, y, glutGetModifiers());

	char code[256];
	snprintf(code,256,"(%s %d %d %d %d %d %d %d)",INPUT_CALLBACK.c_str(),key,-1,-1,-1,x,y,glutGetModifiers());
	RunScheme(code);
}

void KeyboardUpCallback(unsigned char key,int x, int y)
{
	char code[256];
	snprintf(code,256,"(%s %d %d %d %d %d %d %d)",INPUT_RELEASE_CALLBACK.c_str(),key,-1,-1,-1,x,y,0);
	RunScheme(code);
}

void SpecialKeyboardCallback(int key,int x, int y)
{
	app->Handle(0, -1, key, -1, x, y, glutGetModifiers());
	char code[256];
	snprintf(code,256,"(%s %d %d %d %d %d %d %d)",INPUT_CALLBACK.c_str(),0,-1,key,-1,x,y,glutGetModifiers());
	RunScheme(code);
}

void SpecialKeyboardUpCallback(int key,int x, int y)
{
	//app->Handle( 0, 0, key, 1, x, y);
	char code[256];
	snprintf(code,256,"(%s %d %d %d %d %d %d %d)",INPUT_RELEASE_CALLBACK.c_str(),0,-1,key,-1,x,y,glutGetModifiers());
	RunScheme(code);
}

void MouseCallback(int button, int state, int x, int y)
{
	app->Handle(0, button, -1, state, x, y, 0);
	char code[256];
	snprintf(code,256,"(%s %d %d %d %d %d %d %d)",INPUT_CALLBACK.c_str(),0,button,-1,state,x,y,0);
	RunScheme(code);
}

void MotionCallback(int x, int y)
{
	app->Handle(0, -1, -1, -1, x, y, 0);
	char code[256];
	snprintf(code,256,"(%s %d %d %d %d %d %d %d)",INPUT_CALLBACK.c_str(),0,-1,-1,-1,x,y,0);
	RunScheme(code);
}

void IdleCallback()
{
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	void *stack_start;
	stack_start = (void *)&stack_start;
	scheme_set_stack_base(stack_start, 1);  
  
	scheme = scheme_basic_env();
	RunScheme(STARTUP_SCRIPT,true);
	
	srand(time(NULL));
		
	glutInitWindowSize(720,576);
	app = new FluxusMain(720,576);
  	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);
	char windowtitle[256];
	snprintf(windowtitle,256,"fluxus %d.%d",FLUXUS_MAJOR_VERSION,FLUXUS_MINOR_VERSION);
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
	   
   	if (argc>1) app->LoadScript(argv[1]);
		
	if(glewInit() != GLEW_OK)
	{
		cerr << "ERROR Unable to check OpenGL extensions" << endl;
		return false;
	}

	//GLSLShader::Init();
	
	glutMainLoop();
	
	return 0;
}


