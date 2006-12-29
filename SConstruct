###############################################################
# Top level SConscript for fluxus
#
# Checks all dependancies needed, builds the fluxus canvas 
# application, then calls the sconscripts for libfluxus and
# the fluxus PLT modules 

import os, os.path

Target       = "fluxus-plt"
MajorVersion = "0"
MinorVersion = "12"

Prefix = "/usr/local"
Install      = Prefix + "/bin"
LibPaths     = Split("/usr/local/lib \
					  /usr/lib")
					  
IncludePaths = Split("/usr/local/include \
					  /usr/include")

# First member of each list is a library, second - a header or headers list
# to be passed to the CheckLibWithHeader(...) at configure time.
# We may add extra libraries later on per platform basis
LibList      = [["m", "math.h"],
		["pthread", "pthread.h"],
		["dl", "stdio.h"],
		["mzgc", "plt/scheme.h"],
		["mzscheme", "plt/scheme.h"],
		["jpeg", ["stdio.h", "stdlib.h", "jpeglib.h"]],
		["tiff", "tiff.h"],
		["z", "zlib.h"],
		["png", "libpng/png.h"]]

Source = Split("src/GLEditor.cpp \
		src/Utils.cpp \
		src/Recorder.cpp \
		src/FluxusMain.cpp \
		src/main.cpp")
		
FluxusVersion = "HEAD"

env = Environment(CCFLAGS = '-ggdb -pipe -Wall -O3 -ffast-math -Wno-unused -fPIC',
		  LIBPATH = LibPaths,
		  CPPPATH = IncludePaths,
		  VERSION_NUM = FluxusVersion)

Default(env.Program(source = Source, target = Target))
env.Append(CCFLAGS=' -DFLUXUS_MAJOR_VERSION='+MajorVersion)
env.Append(CCFLAGS=' -DFLUXUS_MINOR_VERSION='+MinorVersion)

if env['PLATFORM'] == 'darwin':
	env.Replace(LINK = "macos/libtool --mode=link g++")
	env.Prepend(LINKFLAGS = ["-static"])
else:
	LibList += [["X11", "X11/Xlib.h"],
           	    ["GL", "GL/gl.h"],
           	    ["GLU", "GL/glu.h"],
                ["glut", "GL/glut.h"],
                ["GLEW", "GL/glew.h"],
				["ode", "ode/ode.h"]]
	env.Append(LIBPATH = ["/usr/X11R6/lib"])
	
	# add the X11 libs on - needed if we are not building on xorg
	if ARGUMENTS.get("X11",0):
		LibList=[["Xi", "X11/Xlib.h"],
				 ["Xmu", "X11/Xlib.h"], 
				 ["Xext", "X11/Xlib.h"], 
				 ["Xt", "X11/Xlib.h"], 
				 ["SM", "X11/Xlib.h"], 
				 ["ICE", "X11/Xlib.h"]] + LibList;
	
if not GetOption('clean'):
	print '--------------------------------------------------------'		
	print 'Fluxus: Configuring Build Environment'
	print '--------------------------------------------------------'		
	conf = Configure(env)
	
	# all libraries are required, but they can be checked for independently
	# (hence autoadd=0), which allows us to speed up the tests ...
	for (lib,headers) in LibList:
		if not conf.CheckLibWithHeader(lib, headers, 'C', autoadd = 1):
			print "ERROR: '%s' must be installed!" % (lib)
			Exit(1)
			
	# enable users to enable multitexturing manually
	if ARGUMENTS.get("MULTITEXTURE",1)=="1":
		env.Append(CCFLAGS=' -DENABLE_MULTITEXTURING')
		
	env = conf.Finish()
	# ... but we shouldn't forget to add them to LIBS manually
	env.Replace(LIBS = [rec[0] for rec in LibList])
	

# packaging / installing
if env['PLATFORM'] == 'darwin':
	from macos.osxbundle import *
	TOOL_BUNDLE(env)
	# We add frameworks after configuration bit so that testing is faster.
	env.Replace(FRAMEWORKS = Split("GLUT OpenGL CoreAudio"))
	env.Alias("app", env.MakeBundle("Fluxus.app",
					"fluxus",
					"key",
					"macos/fluxus-Info.plist",
					typecode='APPL',
					icon_file='macos/fluxus.icns'))
	
	env['BUILDERS']['DiskImage'] = Builder(action = BuildDmg)
	DmgFiles = [File("COPYING"), Dir("Fluxus.app"), Dir("docs"), Dir("examples"), Dir("scm")]
	env.Alias("dmg", env.DiskImage('Fluxus-' + FluxusVersion + '.dmg',
				       DmgFiles))
else:
	env.Install(Install, Target)
	env.Alias('install', Prefix)

# call the core library builder and the scheme modules
SConscript(dirs=Split("libfluxus modules"), exports = ["Prefix"])
