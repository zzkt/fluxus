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

#include "FluxusMain.h"
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
#include "Utils.h"
//#include "Repl.h"
#include <iostream>

using namespace std;
using namespace fluxus;

////////////////////////////////////////////////////////////////

FluxusMain::FluxusMain(int x, int y) :
m_CameraMode(SCENE),
m_CurrentEditor(0),
m_Init(true),
m_LastMouseX(0),
m_LastMouseY(0),
m_LastButton(0),
m_CurButton(0),
m_RotX(0),
m_RotY(0),
m_PosX(0),
m_PosY(0),
m_DisY(-10),
m_MouseClickX(0),
m_MouseClickY(0),
m_ShowLocators(false),
m_Frame(-1),
m_Width(x),
m_Height(y),
m_HideScript(false),
m_ShowCursor(true),	
m_InteractiveCamera(true)
{
	for(int i=0; i<10; i++) 
	{
		m_Editor[i] = new GLEditor();
	}
	//m_Editor[9] = new Repl();
}

void FluxusMain::ResetCamera()
{
	m_RotX=m_RotY=m_PosX=m_PosY=0;
	m_DisY=-10;	
	m_MouseClickX=m_MouseClickY=0;
	m_RotStart=dQuat();
	m_RotNow=dQuat();
	m_InteractiveCamera=true;
}

void FluxusMain::Handle(unsigned char key, int button, int special, int state, int x, int y, int mod) 
{
	// insert recorder stuff here...
	
	HandleImpl(key, button, special, state, x, y, mod);
}

// this function is used in arcball implementation
static void onUnitSphere(const float mx, const float my,
			 float& x, float& y, float& z)
{
	x = mx;		// should divide radius
	y = my;
	float mag = x*x + y*y;
	if (mag > 1.0f) {
		float scale = 1.0f / ((float) sqrt(mag));
		x *= scale;
		y *= scale;
	z = 0;
		} else {
		z = (float) sqrt(1 - mag);
	}
}

void FluxusMain::HandleImpl(unsigned char key, int button, int special, int state, int x, int y, int mod) 
{
	//cerr<<"key:"<<key<<" button:"<<button<<" special:"<<special<<" state:"<<state<<" x:"<<x<<" y:"<<y<<endl;
	m_CurMouseX=x;
	m_CurMouseY=y;
	
	if (mod&GLUT_ACTIVE_CTRL)
	{
		// pretty sure this is going to have to change...
		switch(key)
		{
			case 6: glutFullScreen(); break; // f	
			case 23: // w
			{
				glutReshapeWindow(640,480);
				glutPositionWindow(100,100);
			} 
			break;
			case 19: SaveScript(); break; // s			
			case 8: HideScript(); break; // h
			case 13: HideCursor(); break; // m
#ifndef __APPLE__
			case 49: SetCurrentEditor(0); break; // 1
			case 0: SetCurrentEditor(1); break; // 2
			case 27: SetCurrentEditor(2); break; // 3
			case 28: SetCurrentEditor(3); break; // 4
			case 29: SetCurrentEditor(4); break; // 5
			case 30: SetCurrentEditor(5); break; // 6
			case 31: SetCurrentEditor(6); break; // 7
			case 127: SetCurrentEditor(7); break; // 8
			case 57: SetCurrentEditor(8); break; // 9
			case 48: SetCurrentEditor(9); break; // 0
#else
			case 49: SetCurrentEditor(0); break; // 1
			case 50: SetCurrentEditor(1); break; // 2
			case 51: SetCurrentEditor(2); break; // 3
			case 52: SetCurrentEditor(3); break; // 4
			case 53: SetCurrentEditor(4); break; // 5
			case 54: SetCurrentEditor(5); break; // 6
			case 55: SetCurrentEditor(6); break; // 7
			case 56: SetCurrentEditor(7); break; // 8
			case 57: SetCurrentEditor(8); break; // 9
			case 48: SetCurrentEditor(9); break; // 0
#endif
		}
	}
	
	if (key!=0 || special!=-1) 
	{
		if (special==GLUT_KEY_F1) m_CameraMode=SCENE;
		else if (special==GLUT_KEY_F2) m_CameraMode=EDITOR;
		else if (special==GLUT_KEY_F3) ResetCamera();
		else if (special==GLUT_KEY_F4) m_Editor[m_CurrentEditor]->Reset();
		else if (special==GLUT_KEY_F9) 
		{
			m_Editor[m_CurrentEditor]->m_TextColourRed=rand()%1000/1000.0f;
			m_Editor[m_CurrentEditor]->m_TextColourBlue=rand()%1000/1000.0f;
			m_Editor[m_CurrentEditor]->m_TextColourGreen=rand()%1000/1000.0f;
		}	
		else if (special==GLUT_KEY_F10) m_Editor[m_CurrentEditor]->m_TextWidth--;
		else if (special==GLUT_KEY_F11) m_Editor[m_CurrentEditor]->m_TextWidth++;
		else if (special==GLUT_KEY_F5 && m_CurrentEditor<9) m_Script=m_Editor[m_CurrentEditor]->GetText();
	
		// the editor only takes keyboard events
		if (!m_HideScript) m_Editor[m_CurrentEditor]->Handle(button,key,special,state,x,y,mod);
	}
	else
	{
		if (state==0) m_CurButton=button+1; // button on
		else if (state==1) m_CurButton=0; // button off			
	
		if (m_CameraMode==SCENE)
		{
			// mouse event then?
			if (state==0) 
			{
				m_LastButton=button;
				m_LastMouseX=x;
				m_LastMouseY=y;
				// arcball
				m_RotStart = m_RotNow * m_RotStart;
				m_RotNow = dQuat(); // identity
				// unlike m_LastMouseX/Y the following aren't updated during drag
				m_MouseClickX = (((float)x / (((float)m_Width  - 1.0) / 2.0)) - 1.0);
				m_MouseClickY = -(((float)y / (((float)m_Height  - 1.0) / 2.0)) - 1.0);
			}
			else
			{
				switch (m_LastButton)
				{
					case 0:
					{
						/* arcball rotation */
						float dx,dy,dz;
						float mx,my,mz;
						onUnitSphere(m_MouseClickX, m_MouseClickY, dx, dy, dz);
						onUnitSphere((((float)x / (((float)m_Width  - 1.0) / 2.0)) - 1.0), 
							     -(((float)y / (((float)m_Height  - 1.0) / 2.0)) - 1.0), 
							     mx, my, mz);
						
						m_RotNow.x = dy*mz - dz*my;
						m_RotNow.y = dz*mx - dx*mz;
						m_RotNow.z = dx*my - dy*mx;
						m_RotNow.w = dx*mx + dy*my + dz*mz;
						
						m_RotNow.renorm();
					}
					break;
					case 1:
					{
						m_PosX+=(x-m_LastMouseX)/50.0f;
						m_PosY+=-(y-m_LastMouseY)/50.0f;
					}
					break;
					case 2:
					{
						m_DisY+=-(y-m_LastMouseY)/50.0f;
					}
					break;
				}
				m_LastMouseX=x;
				m_LastMouseY=y;
			}		
		}		
		else if (m_CameraMode==EDITOR)
		{
			// mouse event then?
			if (state==0) 
			{
				m_LastButton=button;
				m_LastMouseX=x;
				m_LastMouseY=y;
			}
			else
			{
				switch (m_LastButton)
				{
					case 0:
					{
						m_Editor[m_CurrentEditor]->m_RotY+=(x-m_LastMouseX)/2.0f;
						m_Editor[m_CurrentEditor]->m_RotX+=-(y-m_LastMouseY)/2.0f;
					}
					break;
					case 1:
					{
						m_Editor[m_CurrentEditor]->m_PosX+=(x-m_LastMouseX)*2;
						m_Editor[m_CurrentEditor]->m_PosY+=-(y-m_LastMouseY)*2;
					}
					break;
					case 2:
					{
						m_Editor[m_CurrentEditor]->m_DisY+=-(y-m_LastMouseY)*4;
					}
					break;
				}
				m_LastMouseX=x;
				m_LastMouseY=y;
			}		
		}
	}
	
	if (m_InteractiveCamera)
	{
		//m_Renderer.GetCamera()->init();
		//m_Renderer.GetCamera()->translate(m_PosX,m_PosY,m_DisY);
		//(*m_Renderer.GetCamera()) *= (m_RotNow * m_RotStart).conjugate().toMatrix();
		//m_Renderer.SetOrthoZoom(m_DisY);
	}
}

bool FluxusMain::KeyPressed(char b)
{
	//return Fl::event_key(b);
	return false;
}

void FluxusMain::StartDumpFrames(const string &Filename, const string &Type)
{
	m_Frame=0;
	m_FrameName = Filename;
	m_FrameType = Type;
}

void FluxusMain::EndDumpFrames()
{
	m_Frame=-1;
}

void FluxusMain::Reshape(int width, int height)
{
	for(int n=0; n<NUM_EDITORS; n++)
	{
		m_Editor[n]->Reshape(width,height);
	}
	
	//m_Renderer.SetResolution(width,height);
	m_Width=width;
	m_Height=height;
	m_Init=true;
}

void FluxusMain::Render()
{		
	gettimeofday(&m_Time,NULL);
	if (!m_HideScript) m_Editor[m_CurrentEditor]->Render();
	
	if (m_Frame!=-1)
	{
		char Fnum[7];
		snprintf(Fnum,7,"%06d",m_Frame);
		
		if (m_FrameType=="jpg")
		{
			string Filename=m_FrameName+"-"+string(Fnum)+".jpg";
			WriteJPG((char*)Filename.c_str(),"fluxus pixels",0,0,m_Width,m_Height,80,1);
			cerr<<Filename<<endl;
		}
		else if (m_FrameType=="tif")
		{
			string Filename=m_FrameName+"-"+string(Fnum)+".tif";
			WriteTiff((char*)Filename.c_str(),"fluxus pixels",0,0,m_Width,m_Height,1);
			cerr<<Filename<<endl;
		}
		else if (m_FrameType=="ppm")
		{
			string Filename=m_FrameName+"-"+string(Fnum)+".ppm";
			WritePPM((char*)Filename.c_str(),"fluxus pixels",0,0,m_Width,m_Height,1);
			cerr<<Filename<<endl;
		}
		
		m_Frame++;
	}
}

void FluxusMain::LoadScript(const string &Filename) 
{ 
	FILE *file=fopen(Filename.c_str(),"r");
	if (file)
	{
		fseek(file,0,SEEK_END);
		long size=ftell(file);
		fseek(file,0,SEEK_SET);		
		
		if (size==0) 
		{
			fclose(file);
			cerr<<"empty file: "<<Filename<<endl;
			return;
		}

		if (size<0) 
		{
			fclose(file);
			cerr<<"error loading file: "<<Filename<<" size: "<<size<<"??"<<endl;
			return;
		}
		
		char *buffer = new char[size+1];
		if (buffer)
		{
			if (size!=(long)fread(buffer,1,size,file))	
			{
				delete[] buffer;
				fclose(file);
				cerr<<"read error: "<<Filename<<endl;
				return;
			}			
			buffer[size]='\0';
			m_Editor[m_CurrentEditor]->SetText(buffer);	
		}
		else
		{
			cerr<<"couldn't allocate buffer for load"<<endl;
		}
		
		delete[] buffer;
		fclose(file);
	}
	else
	{
		cerr<<"couldn't load: "<<Filename<<endl;
	}
	
	m_SaveName[m_CurrentEditor]=Filename; // just a precaution
}

void FluxusMain::SaveScript() 
{ 
	if (m_SaveName[m_CurrentEditor]=="") 
	{	
		m_SaveName[m_CurrentEditor]="temp.scm";
	}
	
	FILE *file=fopen(m_SaveName[m_CurrentEditor].c_str(),"w");
	if (file)
	{	
		fwrite(m_Editor[m_CurrentEditor]->GetAllText().c_str(),1,m_Editor[m_CurrentEditor]->GetAllText().size(),file);	
		fclose(file);
	}
	
	Dump("Saved ["+m_SaveName[m_CurrentEditor]+"]");
	
}

