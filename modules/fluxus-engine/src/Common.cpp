#include <assert.h>
#include "Common.h"
#include "FluxusEngine.h"

using namespace std;
using namespace Common;

float Common::FloatFromScheme(Scheme_Object *ob)
{
	return (float)scheme_real_to_double(ob);
}

int Common::IntFromScheme(Scheme_Object *ob)
{
	return (int)scheme_real_to_double(ob);
}

char *Common::StringFromScheme(Scheme_Object *ob)
{
	return scheme_utf8_encode_to_buffer(SCHEME_CHAR_STR_VAL(ob),SCHEME_CHAR_STRLEN_VAL(ob),NULL,0);
}

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

dColour Common::ColourFromScheme(Scheme_Object *src)
{
	dColour ret;
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
	// wrong number of arguments, could mean optional arguments for this function, 
	// just give up in this case for now...
	
	if(argc==format.size())
	{
		for (int n=0; n<format.size(); n++)
		{
			switch(format[n])
			{
				case 'f':
					if (!SCHEME_NUMBERP(argv[n])) scheme_wrong_type(funcname.c_str(), "number", n, argc, argv);
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
					if (!SCHEME_NUMBERP(argv[n])) scheme_wrong_type(funcname.c_str(), "number", n, argc, argv);
				break;

				case 's':
					if (!SCHEME_CHAR_STRINGP(argv[n])) scheme_wrong_type(funcname.c_str(), "string", n, argc, argv);
				break;

				case '?':
				break;

				default:
					assert(false);
				break;
			};

		}
	}

}

fluxus::State *Common::State()
{
    if (FluxusEngine::Get()->Grabbed()) 
	{
		return FluxusEngine::Get()->Grabbed()->GetState();
	}
	return FluxusEngine::Get()->Renderer()->GetState();
}
