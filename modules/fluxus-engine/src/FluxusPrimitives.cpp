#include <assert.h>
#include <escheme.h>
#include "FluxusEngine.h"
#include "FluxusPrimitives.h"
#include "../../../libfluxus/src/dada.h"
#include "../../../libfluxus/src/GraphicsUtils.h"

using namespace FluxusPrimitives;
using namespace fluxus;

Scheme_Object *draw_cube(int argc, Scheme_Object **argv)
{    	
    FluxusEngine::Get()->GetCurrentRenderer()->RenderPrimitive(FluxusEngine::StaticCube);
    return scheme_void;
}

Scheme_Object *build_cube(int argc, Scheme_Object **argv)
{
	PolyPrimitive *BoxPrim = new PolyPrimitive(PolyPrimitive::QUADS);
    MakeCube(BoxPrim);
    return scheme_make_integer_value(FluxusEngine::Get()->GetCurrentRenderer()->AddPrimitive(BoxPrim));    
}

void FluxusPrimitives::AddGlobals(Scheme_Env *env)
{	
	scheme_add_global("draw-cube", scheme_make_prim_w_arity(draw_cube, "draw-cube", 0, 0), env);
	scheme_add_global("build-cube", scheme_make_prim_w_arity(build_cube, "build-cube", 0, 0), env);
}
