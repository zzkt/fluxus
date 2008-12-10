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

#include <iostream>

#include "FluxusMain.h"
#include "keys.h"

using namespace std;
using namespace fluxus;

// from main.cpp
void toggleFullScreen();

////////////////////////////////////////////////////////////////
// despite attempts at clearing all this up, this 
// is still an area of awkward code...

FluxusMain::FluxusMain(int x, int y) :
m_CurrentEditor(9),
m_Width(x),
m_Height(y),
m_HideScript(false),
m_ShowCursor(true), 
m_ShowFileDialog(false)
#ifdef FLX_QT_END
, m_mainWin(NULL)
#endif
{
	// use the interpreter to get the font name
	// and editor prefs
	Scheme_Object *txt;
	Scheme_Object *t;
	MZ_GC_DECL_REG(1);
	MZ_GC_VAR_IN_REG(0, txt);
	MZ_GC_VAR_IN_REG(1, t);
	MZ_GC_REG();	
	Interpreter::Interpret("fluxus-scratchpad-font", &txt);
	char *s=scheme_utf8_encode_to_buffer(SCHEME_CHAR_STR_VAL(txt),SCHEME_CHAR_STRLEN_VAL(txt),NULL,0);
	Interpreter::Interpret("fluxus-scratchpad-do-autofocus", &t);
	GLEditor::m_DoAutoFocus=scheme_real_to_double(t);
	Interpreter::Interpret("fluxus-scratchpad-debug-autofocus", &t);
	GLEditor::m_DebugAutoFocus=scheme_real_to_double(t);
	Interpreter::Interpret("fluxus-scratchpad-autofocus-width", &t);
	GLEditor::m_AutoFocusWidth=scheme_real_to_double(t);
	Interpreter::Interpret("fluxus-scratchpad-autofocus-height", &t);
	GLEditor::m_AutoFocusHeight=scheme_real_to_double(t);
	Interpreter::Interpret("fluxus-scratchpad-autofocus-error", &t);
	GLEditor::m_AutoFocusError=scheme_real_to_double(t);
	Interpreter::Interpret("fluxus-scratchpad-autofocus-drift", &t);
	GLEditor::m_AutoFocusDrift=scheme_real_to_double(t);
	Interpreter::Interpret("fluxus-scratchpad-autofocus-scale-drift", &t);
	GLEditor::m_AutoFocusScaleDrift=scheme_real_to_double(t);
	Interpreter::Interpret("fluxus-scratchpad-autofocus-min-scale", &t);
	GLEditor::m_AutoFocusMinScale=scheme_real_to_double(t);
	Interpreter::Interpret("fluxus-scratchpad-autofocus-max-scale", &t);
	GLEditor::m_AutoFocusMaxScale=scheme_real_to_double(t);
	Interpreter::Interpret("fluxus-scratchpad-visible-lines", &t);
	GLEditor::m_VisibleLines=(int)scheme_real_to_double(t);
	Interpreter::Interpret("fluxus-scratchpad-visible-columns", &t);
	GLEditor::m_VisibleColumns=(int)scheme_real_to_double(t);
	Interpreter::Interpret("fluxus-scratchpad-x-pos", &t);
	GLEditor::m_XPos=(int)scheme_real_to_double(t);
	Interpreter::Interpret("fluxus-scratchpad-y-pos", &t);
	GLEditor::m_YPos=(int)scheme_real_to_double(t);
  	MZ_GC_UNREG();
	
	GLEditor::InitFont(s);
	m_FileDialog = new GLFileDialog;

	for(int i=0; i<9; i++) 
	{
		m_Editor[i] = new GLEditor();
	}
	Repl *repl = new Repl();
	m_Editor[9] = repl;
	
	// register the repl with the interpreter so we can
	// use it to output error and output messages
	Interpreter::SetRepl(repl);
}

FluxusMain::~FluxusMain() 
{
	for(int i=0; i<10; i++)
	{
		delete m_Editor[i];
	}
}

void FluxusMain::Handle(int key, int button, int special, int state, int x, int y, int mod) 
{	
	if (mod&GLEDITOR_CTRL_MOD)
	{
		// pretty sure this is going to have to change...
		switch(key)
		{
			case GLEDITOR_CTRL_F: 
#ifdef FLX_GLUT_END
                glutFullScreen(); 
                break;
#endif
                // on Qt fallthrough, to fullscreen/windowed state toggle
			case GLEDITOR_CTRL_W: 
#ifdef FLX_GLUT_END
				glutReshapeWindow(720,576);
				glutPositionWindow(100,100);
#elif defined(FLX_QT_END)
                toggleFullScreen();
#endif
                break;
            case GLEDITOR_CTRL_P: 
                Pretty(); 
                break; 
            case GLEDITOR_CTRL_S: 
                if (m_CurrentEditor!=9) 
                    SaveScript(); 
                break;
            case GLEDITOR_CTRL_H: 
                HideScript(); 
                break;
            case GLEDITOR_CTRL_M: 
                ToggleCursor(); 
                break;
            case GLEDITOR_CTRL_L: 
                if (m_CurrentEditor!=9) // don't go into the dialogs from the repl
                {
                    m_FileDialog->SetSaveAsMode(false);
                    m_ShowFileDialog=!m_ShowFileDialog;
                } 
                break; 
            case GLEDITOR_CTRL_D: 
                if (m_CurrentEditor!=9) // don't go into the dialogs from the repl
                {
                    m_FileDialog->SetSaveAsMode(true);
                    m_ShowFileDialog=!m_ShowFileDialog; 
                }
                break; 
            case GLEDITOR_CTRL_E:
                {
                    Execute();
                }			
			break;

			case GLEDITOR_CTRL_1: SetCurrentEditor(0); break; 
			case GLEDITOR_CTRL_2: SetCurrentEditor(1); break; 
			case GLEDITOR_CTRL_3: SetCurrentEditor(2); break; 
			case GLEDITOR_CTRL_4: SetCurrentEditor(3); break; 
			case GLEDITOR_CTRL_5: SetCurrentEditor(4); break; 
			case GLEDITOR_CTRL_6: SetCurrentEditor(5); break; 
			case GLEDITOR_CTRL_7: SetCurrentEditor(6); break; 
			case GLEDITOR_CTRL_8: SetCurrentEditor(7); break; 
			case GLEDITOR_CTRL_9: SetCurrentEditor(8); break; 
			case GLEDITOR_CTRL_0: SetCurrentEditor(9); break; 
		}
	}
	
	if (key!=-1 || special!=-1) 
	{
                switch (special) {
                case GLEDITOR_F9:
			m_Editor[m_CurrentEditor]->m_TextColourRed=rand()%1000/1000.0f;
			m_Editor[m_CurrentEditor]->m_TextColourBlue=rand()%1000/1000.0f;
			m_Editor[m_CurrentEditor]->m_TextColourGreen=rand()%1000/1000.0f;
                        break;
                case GLEDITOR_F10:
			m_Editor[m_CurrentEditor]->m_TextColourAlpha-=0.05;
			if (m_Editor[m_CurrentEditor]->m_TextColourAlpha<0)
			{
				m_Editor[m_CurrentEditor]->m_TextColourAlpha=0;
			}
		        break;
                case GLEDITOR_F11:
			m_Editor[m_CurrentEditor]->m_TextColourAlpha+=0.05;
			if (m_Editor[m_CurrentEditor]->m_TextColourAlpha>1)
			{
				m_Editor[m_CurrentEditor]->m_TextColourAlpha=1;
			}
		        break;
                case GLEDITOR_F4:
                        if (m_CurrentEditor<9) 
                        {
                                m_Script=m_Editor[m_CurrentEditor]->GetSExpr();				
                        }
                        break;
                case GLEDITOR_F5:
                        if (m_CurrentEditor<9) 
                        {
                                Execute();
                        }
                        break;
                case GLEDITOR_F6:
		        if (m_CurrentEditor<9) 
                        {
                                Interpreter::Initialise();
                                m_Script=m_Editor[m_CurrentEditor]->GetText();
                                SaveBackupScript();
                        }
                        break;
                }
	
		// the editors only take keyboard events
		if (m_ShowFileDialog) 
		{
			if (key==GLEDITOR_ESCAPE)
			{
				m_ShowFileDialog=false;
			}
			
			m_FileDialog->Handle(button,key,special,state,x,y,mod);
			if (m_FileDialog->GetOutput()!="")
			{
				if (m_FileDialog->GetSaveAsMode()) 
				{
					m_SaveName[m_CurrentEditor]=m_FileDialog->GetOutput();
					SaveScript();
				}
				else 
				{
					LoadScript(m_FileDialog->GetOutput());
				}
				m_FileDialog->Clear();
				m_ShowFileDialog=false;
			}
		}
		else if (!m_HideScript) 
		{
			m_Editor[m_CurrentEditor]->Handle(button,key,special,state,x,y,mod);
		}
	}
}

void FluxusMain::Reshape(int width, int height)
{
	for(int n=0; n<NUM_EDITORS; n++)
	{
		m_Editor[n]->Reshape(width,height);
	}
	
	m_FileDialog->Reshape(width,height);
	m_Width=width;
	m_Height=height;
}

void FluxusMain::Render()
{		
        if (m_ShowFileDialog) m_FileDialog->Render();
        else 
                if (!m_HideScript) m_Editor[m_CurrentEditor]->Render();
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
 
void FluxusMain::Execute()
{
	m_Script=m_Editor[m_CurrentEditor]->GetText();
	SaveBackupScript();
}

void FluxusMain::SaveBackupScript() 
{ 
	if (m_SaveName[m_CurrentEditor]=="") 
	{	
		m_SaveName[m_CurrentEditor]="temp.scm";
	}
	
	FILE *file=fopen((m_SaveName[m_CurrentEditor]+string("~")).c_str(),"w");
	if (file)
	{	
		fwrite(m_Editor[m_CurrentEditor]->GetAllText().c_str(),1,m_Editor[m_CurrentEditor]->GetAllText().size(),file);	
		fclose(file);
	}
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
	
	cerr<<"Saved ["<<m_SaveName[m_CurrentEditor]<<"]"<<endl;
	
}

string Escape(const string &str)
{
	string out;
	for(unsigned int i=0; i<str.size(); i++)
	{
		if (str[i]=='\"') out+="\\\"";
		else if (str[i]=='\\') out+="\\\\";
		else out+=str[i];
	}
	return out;
}

void FluxusMain::Pretty()
{
	Scheme_Object *txt;
	MZ_GC_DECL_REG(1);
	MZ_GC_VAR_IN_REG(0, txt);
	MZ_GC_REG();	
	string command(string("(fluxus-auto-indent \"")+Escape(m_Editor[m_CurrentEditor]->GetAllText())+string("\")"));
	Interpreter::Interpret(command, &txt);
	if (txt!=NULL)
	{
		char *s=scheme_utf8_encode_to_buffer(SCHEME_CHAR_STR_VAL(txt),SCHEME_CHAR_STRLEN_VAL(txt),NULL,0);
		m_Editor[m_CurrentEditor]->SetText(s);	
	}
  	MZ_GC_UNREG();
}

void FluxusMain::ToggleCursor() 
{
	m_ShowCursor=!m_ShowCursor; 

#if defined(FLX_GLUT_END)
    glutSetCursor(m_ShowCursor ? GLUT_CURSOR_INHERIT : GLUT_CURSOR_NONE);
#elif defined(FLX_QT_END)
    mainWin()->setCursor(m_ShowCursor ? Qt::ArrowCursor : Qt::BlankCursor);
#else
#error Unknown backend
#endif
}
