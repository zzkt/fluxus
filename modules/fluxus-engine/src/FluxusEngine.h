#include "../../../libfluxus/src/Renderer.h"
#include "../../../libfluxus/src/PolyPrimitive.h"

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
	fluxus::Renderer *GetCurrentRenderer();
	void BeginScene(); 
	void EndScene(); 
	
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
};
