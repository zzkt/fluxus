#include <plt/escheme.h>
#include <iostream>
#include "OSCServer.h"

using namespace std;
using namespace fluxus;

Server *OSCServer = NULL;
Client *OSCClient = NULL;

Scheme_Object *osc_source(int argc, Scheme_Object **argv)
{
	if (!SCHEME_CHAR_STRINGP(argv[0])) scheme_wrong_type("osc-source", "string", 0, argc, argv);
	char *port=scheme_utf8_encode_to_buffer(SCHEME_CHAR_STR_VAL(argv[0]),SCHEME_CHAR_STRLEN_VAL(argv[0]),NULL,0);

	if (!OSCServer)
	{
		OSCServer = new Server(port);
		OSCServer->Run();
	}
	else
	{
		OSCServer->SetPort(port);
	}

    return scheme_void;
}

Scheme_Object *osc_msg(int argc, Scheme_Object **argv)
{
	if (!SCHEME_CHAR_STRINGP(argv[0])) scheme_wrong_type("osc-msg", "string", 0, argc, argv);
	char *name=scheme_utf8_encode_to_buffer(SCHEME_CHAR_STR_VAL(argv[0]),SCHEME_CHAR_STRLEN_VAL(argv[0]),NULL,0);
	if (OSCServer->SetMsg(name))
	{
		return scheme_make_true();
	}
	return scheme_make_false();
}

Scheme_Object *osc(int argc, Scheme_Object **argv)
{
	if (!SCHEME_NUMBERP(argv[0])) scheme_wrong_type("osc", "number", 0, argc, argv);
 
	unsigned int index=(unsigned int)scheme_real_to_double(argv[0]);
	if (OSCServer!=NULL)
	{
		vector<OSCData*> args;
		OSCServer->GetArgs(args);
		char type = args[index]->Type();
		
		Scheme_Object *ret;
		if (type=='f') ret=scheme_make_double(static_cast<OSCFloat*>(args[index])->Value);
		else if (type=='i') ret=scheme_make_integer_value_from_unsigned(static_cast<OSCInt*>(args[index])->Value);
		else if (type=='s') 
		{
			string value=static_cast<OSCString*>(args[index])->Value;
			ret=scheme_make_utf8_string(value.c_str());	
		}
		else ret=scheme_void;
		
		return ret;
	}
	return scheme_void;
}

Scheme_Object *osc_destination(int argc, Scheme_Object **argv)
{
	if (!SCHEME_CHAR_STRINGP(argv[0])) scheme_wrong_type("osc-destination", "string", 0, argc, argv);
	char *port=scheme_utf8_encode_to_buffer(SCHEME_CHAR_STR_VAL(argv[0]),SCHEME_CHAR_STRLEN_VAL(argv[0]),NULL,0);
	if (!OSCClient)
	{
		OSCClient = new Client();
	}
	OSCClient->SetDestination(port);
    return scheme_void;
}

Scheme_Object *osc_peek(int argc, Scheme_Object **argv)
{
	return scheme_make_utf8_string(OSCServer->GetLastMsg().c_str());	
}

Scheme_Object *osc_send(int argc, Scheme_Object **argv)
{
	if (!SCHEME_CHAR_STRINGP(argv[0])) scheme_wrong_type("osc-send", "string", 0, argc, argv);
	if (!SCHEME_CHAR_STRINGP(argv[1])) scheme_wrong_type("osc-send", "string", 1, argc, argv);

	// todo: fix this...
	//SCM_ASSERT(SCM_LISTP(s_arglist), s_arglist, SCM_ARG2, "osc-send");
	char *msg=scheme_utf8_encode_to_buffer(SCHEME_CHAR_STR_VAL(argv[0]),SCHEME_CHAR_STRLEN_VAL(argv[0]),NULL,0);
	char *types=scheme_utf8_encode_to_buffer(SCHEME_CHAR_STR_VAL(argv[1]),SCHEME_CHAR_STRLEN_VAL(argv[1]),NULL,0);
	
/*
	// vectors seem easier to handle than lists with this api
	SCM argvec = scm_vector(s_arglist);
	
	vector<OSCData*> oscargs;
	for (unsigned int n=0; n<scm_c_generalized_vector_length(argvec); n++)
	{
		SCM arg=scm_vector_ref(argvec, scm_from_int(n));

		if (scm_is_number(arg))// ||  scm_is_true(scm_exact_p(arg)) || scm_is_true(scm_inexact_p(arg)))
		{
			if (n<strlen(types))
			{
				if (types[n]=='f') oscargs.push_back(new OSCFloat(scheme_real_to_double(arg)));
				else if (types[n]=='i') oscargs.push_back(new OSCInt(scm_to_uint(arg)));
			}
		}
		else if (scm_is_string(arg))
		{
			char *argstring=scm_to_locale_string(arg);
			oscargs.push_back(new OSCString(argstring));
			free(argstring);
		}
		else
		{
			cerr<<"osc-send has found an argument type it can't send, numbers and strings only"<<endl;
			free(msg);
			free(types);
    		return SCM_UNSPECIFIED;
		}
	}

	Fluxus->SendOSC(msg,oscargs);
	free(msg);
	free(types);
	
*/

    return scheme_void;
}

/////////////////////

Scheme_Object *scheme_reload(Scheme_Env *env)
{
	// add all the modules from this extension
	Scheme_Env *menv=scheme_primitive_module(scheme_intern_symbol("fluxus-osc"), env);

	scheme_add_global("osc-source", scheme_make_prim_w_arity(osc_source, "osc-source", 1, 1), menv);
	scheme_add_global("osc-msg", scheme_make_prim_w_arity(osc_msg, "osc-msg", 1, 1), menv);
	scheme_add_global("osc", scheme_make_prim_w_arity(osc, "osc", 1, 1), menv);
	scheme_add_global("osc-destination", scheme_make_prim_w_arity(osc_destination, "osc-destination", 1, 1), menv);
	scheme_add_global("osc-peek", scheme_make_prim_w_arity(osc_peek, "osc-peek", 0, 0), menv);
	scheme_add_global("osc-send", scheme_make_prim_w_arity(osc_send, "osc-send", 3, 3), menv);

	scheme_finish_primitive_module(menv);	
	
	return scheme_void;
}

Scheme_Object *scheme_initialize(Scheme_Env *env)
{
	return scheme_reload(env);
}

Scheme_Object *scheme_module_name()
{
	return scheme_intern_symbol("fluxus-osc");
}
