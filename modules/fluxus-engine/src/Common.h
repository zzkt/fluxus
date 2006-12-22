#include <string>
#include <plt/escheme.h>
#include "dada.h"
#include "Renderer.h"

namespace Common
{
	// utility functions to make life easier for binding, and also to allow us to 
	// replace the plt functions used for optimisation later, without having 
	// to rewrite everything...
	
	float FloatFromScheme(Scheme_Object *ob);
	int IntFromScheme(Scheme_Object *ob);
	void FloatsFromScheme(Scheme_Object *src, float *dst, unsigned int size);
	char *StringFromScheme(Scheme_Object *ob);
	Scheme_Object *FloatsToScheme(float *src, unsigned int size);
	dVector VectorFromScheme(Scheme_Object *src);
	dColour ColourFromScheme(Scheme_Object *src);
	dQuat QuatFromScheme(Scheme_Object *src);
	dMatrix MatrixFromScheme(Scheme_Object *src);
	void ArgCheck(const std::string &funcname, const std::string &format, int argc, Scheme_Object **argv);
	fluxus::State *State();
}
