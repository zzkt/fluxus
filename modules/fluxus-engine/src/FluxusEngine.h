#include "Renderer.h"
#include "PolyPrimitive.h"

class FluxusEngine
{
public:
	static FluxusEngine *Get() 
	{
		if (!m_FluxusEngine) m_FluxusEngine=new FluxusEngine;
		return m_FluxusEngine;
	}
	
	unsigned int MakeRenderer();
	bool PushRenderer(unsigned int);
	void PopRenderer();
	void ResetRenderers();
	fluxus::Renderer *Renderer();
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
	

private:
	FluxusEngine();
	~FluxusEngine();
	
	static FluxusEngine *m_FluxusEngine;
	
	vector<fluxus::Renderer *> m_RendererVec;
	deque<unsigned int> m_RendererStack;
	deque<unsigned int> m_GrabStack;
	fluxus::Primitive *m_Grabbed;
};
