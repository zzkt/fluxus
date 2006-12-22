#include <assert.h>
#include <plt/escheme.h>
#include "Common.h"
#include "FluxusEngine.h"
#include "FluxusUtils.h"
#include "Renderer.h"

using namespace FluxusUtils;
using namespace Common;

Scheme_Object *time(int argc, Scheme_Object **argv)
{
	return scheme_make_double(FluxusEngine::Get()->Renderer()->GetTime());
}

Scheme_Object *delta(int argc, Scheme_Object **argv)
{
	return scheme_make_double(FluxusEngine::Get()->Renderer()->GetDelta());
}

Scheme_Object *flxrnd(int argc, Scheme_Object **argv)
{
	return scheme_make_double(RandFloat());
}

Scheme_Object *flxseed(int argc, Scheme_Object **argv)
{
  	ArgCheck("flxseed", "i", argc, argv);
	srand(IntFromScheme(argv[0]));
	return scheme_void;
}

void FluxusUtils::AddGlobals(Scheme_Env *env)
{	
	// renderstate operations
	scheme_add_global("flxtime",scheme_make_prim_w_arity(time,"flxtime",0,0), env);
	scheme_add_global("delta",scheme_make_prim_w_arity(delta,"delta",0,0), env);
	scheme_add_global("flxrnd",scheme_make_prim_w_arity(flxrnd,"flxrnd",0,0), env);
	scheme_add_global("flxseed",scheme_make_prim_w_arity(flxseed,"flxseed",1,1), env);	
}
