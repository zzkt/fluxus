#include <assert.h>
#include <plt/escheme.h>
#include "Common.h"
#include "FluxusEngine.h"
#include "FluxusGlobalState.h"
#include "Renderer.h"

using namespace FluxusGlobalState;
using namespace Common;


Scheme_Object *clear(int argc, Scheme_Object **argv)
{
	FluxusEngine::Get()->Renderer()->Clear();
	FluxusEngine::Get()->Renderer()->ClearLights();
	FluxusEngine::Get()->ClearGrabStack();
	FluxusEngine::Get()->Renderer()->UnGrab();
	return scheme_void;
}

Scheme_Object *blur(int argc, Scheme_Object **argv)
{
	ArgCheck("blur", "f", argc, argv);
	float blur=FloatFromScheme(argv[0]);	
	if (!blur) FluxusEngine::Get()->Renderer()->SetMotionBlur(false);
    else FluxusEngine::Get()->Renderer()->SetMotionBlur(true, blur);
    return scheme_void;
}

Scheme_Object *fog(int argc, Scheme_Object **argv)
{
	ArgCheck("fog", "vfff", argc, argv);
	FluxusEngine::Get()->Renderer()->SetFog(ColourFromScheme(argv[0]),
		FloatFromScheme(argv[1]),
		FloatFromScheme(argv[2]),
		FloatFromScheme(argv[3]));
    return scheme_void;
}

Scheme_Object *feedback(int argc, Scheme_Object **argv)
{
 	ArgCheck("feedback", "f", argc, argv);
	FluxusEngine::Get()->Renderer()->SetFeedBack(FloatFromScheme(argv[0]));
    return scheme_void;
}

Scheme_Object *feedback_transform(int argc, Scheme_Object **argv)
{
 	ArgCheck("feedback-transform", "m", argc, argv);
	FluxusEngine::Get()->Renderer()->SetFeedBackMat(MatrixFromScheme(argv[0]));
	return scheme_void;	
}

Scheme_Object *show_axis(int argc, Scheme_Object **argv)
{
 	ArgCheck("show-axis", "i", argc, argv);
    FluxusEngine::Get()->Renderer()->ShowAxis(IntFromScheme(argv[0]));
    //Fluxus->ShowLocators(IntFromScheme(argv[0]));
    return scheme_void;
}

Scheme_Object *show_fps(int argc, Scheme_Object **argv)
{
 	ArgCheck("show-fps", "i", argc, argv);
    FluxusEngine::Get()->Renderer()->SetFPSDisplay(IntFromScheme(argv[0]));
    return scheme_void;
}

Scheme_Object *lock_camera(int argc, Scheme_Object **argv)
{
 	ArgCheck("lock-camera", "i", argc, argv);
    FluxusEngine::Get()->Renderer()->LockCamera(IntFromScheme(argv[0]));
    return scheme_void;
}

Scheme_Object *camera_lag(int argc, Scheme_Object **argv)
{
 	ArgCheck("camera-lag", "f", argc, argv);
    FluxusEngine::Get()->Renderer()->SetCameraLag(FloatFromScheme(argv[0]));
    return scheme_void;
}

Scheme_Object *load_texture(int argc, Scheme_Object **argv)
{
 	ArgCheck("load-texture", "s", argc, argv);		
    return scheme_make_integer_value(FluxusEngine::Get()->Renderer()->LoadTexture(StringFromScheme(argv[0])));
}

Scheme_Object *force_load_texture(int argc, Scheme_Object **argv)
{
 	ArgCheck("force-load-texture", "s", argc, argv);
    return scheme_make_integer_value(FluxusEngine::Get()->Renderer()->LoadTexture(StringFromScheme(argv[0]),true));
}

Scheme_Object *frustum(int argc, Scheme_Object **argv)
{
 	ArgCheck("frustum", "ffff", argc, argv);
	FluxusEngine::Get()->Renderer()->SetFrustum(FloatFromScheme(argv[0]),
												FloatFromScheme(argv[1]),
									 		 	FloatFromScheme(argv[2]),
												FloatFromScheme(argv[3]));
    return scheme_void;
}

Scheme_Object *clip(int argc, Scheme_Object **argv)
{
	ArgCheck("clip", "ff", argc, argv);
	FluxusEngine::Get()->Renderer()->SetClip(FloatFromScheme(argv[0]),
											 FloatFromScheme(argv[1]));
    return scheme_void;
}

Scheme_Object *ortho(int argc, Scheme_Object **argv)
{
	FluxusEngine::Get()->Renderer()->SetOrtho(true);
    return scheme_void;
}

Scheme_Object *persp(int argc, Scheme_Object **argv)
{
	FluxusEngine::Get()->Renderer()->SetOrtho(false);
    return scheme_void;
}

Scheme_Object *backfacecull(int argc, Scheme_Object **argv)
{
	ArgCheck("backfacecull", "i", argc, argv);
	FluxusEngine::Get()->Renderer()->SetBackFaceCull(IntFromScheme(argv[0]));
	return scheme_void;
}

Scheme_Object *clear_colour(int argc, Scheme_Object **argv)
{
 	ArgCheck("clear-colour", "v", argc, argv);
    FluxusEngine::Get()->Renderer()->SetBGColour(ColourFromScheme(argv[0]));
    return scheme_void;
}

Scheme_Object *clear_frame(int argc, Scheme_Object **argv)
{
 	ArgCheck("clear-frame", "i", argc, argv);
	FluxusEngine::Get()->Renderer()->SetClearFrame(IntFromScheme(argv[0]));
    return scheme_void;
}

Scheme_Object *get_transform(int argc, Scheme_Object **argv)
{
	return FloatsToScheme(State()->Transform.arr(),16);
}

Scheme_Object *get_camera_transform(int argc, Scheme_Object **argv)
{
	return FloatsToScheme(FluxusEngine::Get()->Renderer()->GetCamera()->inverse().arr(),16);
}

Scheme_Object *set_camera_transform(int argc, Scheme_Object **argv)
{
	ArgCheck("set-camera-transform", "m", argc, argv);
	dMatrix m;
	FloatsFromScheme(argv[0],m.arr(),16);
	(*FluxusEngine::Get()->Renderer()->GetCamera())=m.inverse();
	return scheme_void;
}

Scheme_Object *get_projection_transform(int argc, Scheme_Object **argv)
{
	return FloatsToScheme(FluxusEngine::Get()->Renderer()->GetProjection().arr(),16);
}

Scheme_Object *get_screen_size(int argc, Scheme_Object **argv)
{
	float res[2];
	int x=0,y=0;
	FluxusEngine::Get()->Renderer()->GetResolution(x,y);
	res[0]=x; res[1]=y;
	return FloatsToScheme(res,2);
}

Scheme_Object *set_screen_size(int argc, Scheme_Object **argv)
{
	if (!SCHEME_VECTORP(argv[0])) scheme_wrong_type("set-screen-size", "vector", 0, argc, argv);
	if (SCHEME_VEC_SIZE(argv[0])!=2) scheme_wrong_type("set-screen-size", "vector size 2", 0, argc, argv);
	float v[2];
	FloatsFromScheme(argv[0],v,2);
	// hmmm, seems a bit wrong, but hey...
	glutReshapeWindow((int)v[0],(int)v[1]);
	return scheme_void;
}


Scheme_Object *desiredfps(int argc, Scheme_Object **argv)
{
	ArgCheck("desiredfps", "f", argc, argv);
	FluxusEngine::Get()->Renderer()->SetDesiredFPS(scheme_real_to_double(argv[0]));
	return scheme_void;
}

void FluxusGlobalState::AddGlobals(Scheme_Env *env)
{	
	scheme_add_global("clear", scheme_make_prim_w_arity(clear, "clear", 0, 0), env);
	scheme_add_global("blur", scheme_make_prim_w_arity(blur, "blur", 1, 1), env);
	scheme_add_global("fog", scheme_make_prim_w_arity(fog, "fog", 4, 4), env);
	scheme_add_global("feedback", scheme_make_prim_w_arity(feedback, "feedback", 1, 1), env);
	scheme_add_global("feedback-transform", scheme_make_prim_w_arity(feedback_transform, "feedback-transform", 1, 1), env);
	scheme_add_global("show-axis", scheme_make_prim_w_arity(show_axis, "show-axis", 1, 1), env);
	scheme_add_global("show-fps", scheme_make_prim_w_arity(show_fps, "show-fps", 1, 1), env);
	scheme_add_global("lock-camera", scheme_make_prim_w_arity(lock_camera, "lock-camera", 1, 1), env);
	scheme_add_global("camera-lag", scheme_make_prim_w_arity(camera_lag, "camera-lag", 1, 1), env);
	scheme_add_global("load-texture", scheme_make_prim_w_arity(load_texture, "load-texture", 1, 1), env);
	scheme_add_global("force-load-texture", scheme_make_prim_w_arity(force_load_texture, "force-load-texture", 1, 1), env);
	scheme_add_global("frustum", scheme_make_prim_w_arity(frustum, "frustum", 0, 0), env);
	scheme_add_global("clip", scheme_make_prim_w_arity(clip, "clip", 2, 2), env);
	scheme_add_global("ortho", scheme_make_prim_w_arity(ortho, "ortho", 0, 0), env);
	scheme_add_global("persp", scheme_make_prim_w_arity(persp, "persp", 0, 0), env);
	scheme_add_global("backfacecull", scheme_make_prim_w_arity(backfacecull, "backfacecull", 1, 1), env);
	scheme_add_global("clear-colour", scheme_make_prim_w_arity(clear_colour, "clear-colour", 1, 1), env);
	scheme_add_global("clear-frame", scheme_make_prim_w_arity(clear_frame, "clear-frame", 1, 1), env);
	scheme_add_global("get-transform", scheme_make_prim_w_arity(get_transform, "get-transform", 0, 0), env);
	scheme_add_global("get-camera-transform", scheme_make_prim_w_arity(get_camera_transform, "get-camera-transform", 0, 0), env);
	scheme_add_global("set-camera-transform", scheme_make_prim_w_arity(set_camera_transform, "set-camera-transform", 1, 1), env);
	scheme_add_global("get-projection-transform", scheme_make_prim_w_arity(get_projection_transform, "get-projection-transform", 0, 0), env);
	scheme_add_global("get-screen-size", scheme_make_prim_w_arity(get_screen_size, "get-screen-size", 0, 0), env);
	scheme_add_global("set-screen-size", scheme_make_prim_w_arity(set_screen_size, "set-screen-size", 1, 1), env);
	scheme_add_global("desiredfps", scheme_make_prim_w_arity(desiredfps, "desiredfps", 1, 1), env);
}
