#include <string>
#include <escheme.h>
#include "../../../libfluxus/src/dada.h"

namespace Common
{
	void FloatsFromScheme(Scheme_Object *src, float *dst, unsigned int size);
	Scheme_Object *FloatsToScheme(float *src, unsigned int size);
	dVector VectorFromScheme(Scheme_Object *src);
	dQuat QuatFromScheme(Scheme_Object *src);
	dMatrix MatrixFromScheme(Scheme_Object *src);
	void ArgCheck(const std::string &funcname, const std::string &format, int argc, Scheme_Object **argv);
}
