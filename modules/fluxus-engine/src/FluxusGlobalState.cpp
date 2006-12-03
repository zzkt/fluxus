#include <assert.h>
#include <escheme.h>
#include "Common.h"
#include "FluxusEngine.h"
#include "FluxusGlobalState.h"
#include "../../../libfluxus/src/Renderer.h"

using namespace FluxusGlobalState;
using namespace Common;

Scheme_Object *set_camera(int argc, Scheme_Object **argv)
{
	ArgCheck("set-camera", "m", argc, argv);
	*FluxusEngine::Get()->GetCurrentRenderer()->GetCamera()=MatrixFromScheme(argv[0]);
	return scheme_void;
}

Scheme_Object *desiredfps(int argc, Scheme_Object **argv)
{
	ArgCheck("desiredfps", "f", argc, argv);
	FluxusEngine::Get()->GetCurrentRenderer()->SetDesiredFPS(scheme_real_to_double(argv[0]));
	return scheme_void;
}


void FluxusGlobalState::AddGlobals(Scheme_Env *env)
{	
	scheme_add_global("set-camera", scheme_make_prim_w_arity(set_camera, "set-camera", 1, 1), env);
	scheme_add_global("desiredfps", scheme_make_prim_w_arity(desiredfps, "desiredfps", 1, 1), env);
}
