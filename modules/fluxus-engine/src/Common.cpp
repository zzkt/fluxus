#include <assert.h>
#include "Common.h"

using namespace std;
using namespace Common;

void Common::FloatsFromScheme(Scheme_Object *src, float *dst, unsigned int size)
{
	assert(size==SCHEME_VEC_SIZE(src));
	Scheme_Object **array = SCHEME_VEC_ELS(src);
	for (int n=0; n<size; n++)
	{
	
		dst[n]=scheme_real_to_double(array[n]);
	}
}

Scheme_Object *Common::FloatsToScheme(float *src, unsigned int size)
{
	Scheme_Object *ret = scheme_make_vector(size, NULL);
	Scheme_Object **array = SCHEME_VEC_ELS(ret);
	for (int n=0; n<size; n++)
	{
		array[n]=scheme_make_double(src[n]);
	}
	return ret;
}

dVector Common::VectorFromScheme(Scheme_Object *src)
{
	dVector ret;
	FloatsFromScheme(src,ret.arr(),3);
	return ret;
}

dQuat Common::QuatFromScheme(Scheme_Object *src)
{
	dQuat ret;
	FloatsFromScheme(src,ret.arr(),4);
	return ret;
}

dMatrix Common::MatrixFromScheme(Scheme_Object *src)
{
	dMatrix ret;
	FloatsFromScheme(src,ret.arr(),16);
	return ret;
}

void Common::ArgCheck(const string &funcname, const string &format, int argc, Scheme_Object **argv)
{
	// wrong number of arguments, should be checked by mz scheme, so means a mismatch
	// between the code and the scheme_add_global command for this function
	assert(argc==format.size());
	
	for (int n=0; n<format.size(); n++)
	{
		switch(format[n])
		{
			case 'f':
				if (!SCHEME_REALP(argv[n])) scheme_wrong_type(funcname.c_str(), "real number", n, argc, argv);
			break;
			
			case 'v':
				if (!SCHEME_VECTORP(argv[n])) scheme_wrong_type(funcname.c_str(), "vector", n, argc, argv);
				if (SCHEME_VEC_SIZE(argv[n])!=3) scheme_wrong_type(funcname.c_str(), "vector size 3", n, argc, argv);
			break;
			
			case 'q':
				if (!SCHEME_VECTORP(argv[n])) scheme_wrong_type(funcname.c_str(), "vector", n, argc, argv);
				if (SCHEME_VEC_SIZE(argv[n])!=4) scheme_wrong_type(funcname.c_str(), "quat (vector size 4)", n, argc, argv);
			break;
			
			case 'm':
				if (!SCHEME_VECTORP(argv[n])) scheme_wrong_type(funcname.c_str(), "vector", n, argc, argv);
				if (SCHEME_VEC_SIZE(argv[n])!=16) scheme_wrong_type(funcname.c_str(), "matrix (vector size 16)", n, argc, argv);
			break;
			
			case 'i':
			break;
			
			case 's':
			break;
			
			default:
				assert(false);
			break;
		};
	}

}
