#include "Renderer.h"
#include "Physics.h"
#include "PolyPrimitive.h"
#include "TurtleBuilder.h"

class Engine
{
public:
	static Engine *Get() 
	{
		if (!m_Engine) m_Engine=new Engine;
		return m_Engine;
	}
	
	unsigned int MakeRenderer();
	bool PushRenderer(unsigned int);
	void PopRenderer();
	void ResetRenderers();
	fluxus::Renderer *Renderer();
	fluxus::Physics *Physics();
	void BeginScene(); 
	void EndScene(); 
	
	void PushGrab(int id);
	void PopGrab();
	void ClearGrabStack();
	fluxus::Primitive *Grabbed() { return m_Grabbed; }
	
	static fluxus::PolyPrimitive* StaticCube;
	static fluxus::PolyPrimitive* StaticPlane;
	static fluxus::PolyPrimitive* StaticSphere;
	static fluxus::PolyPrimitive* StaticCylinder;
	
	fluxus::TurtleBuilder *GetTurtle() { return &m_Turtle; }
	
	// helper for the bindings
	fluxus::State *State();

private:
	Engine();
	~Engine();
	
	static Engine *m_Engine;
	
	vector<pair<fluxus::Renderer *, fluxus::Physics *> > m_RendererVec;
	deque<unsigned int> m_RendererStack;
	deque<unsigned int> m_GrabStack;
	fluxus::Primitive *m_Grabbed;
	fluxus::TurtleBuilder m_Turtle;
};
