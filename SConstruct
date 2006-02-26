#                                                              -*- python -*-

import os, os.path

# common values, the rest added later on per platform basis
Target       = "fluxus"
Install      = "/usr/local/bin"
LibPaths     = ["/usr/local/lib"]
Libs = Split("jack sndfile guile fftw3 ode png tiff jpeg z m pthread lo")
#Libs = Split("jack sndfile guile fftw3 ode png glut tiff GL GLU z m Xi Xmu Xext Xt SM ICE X11 pthread lo jpeg")
IncludePaths = Split("/usr/local/include libfluxus/src libfluxphysics/src")
Source = Split("libfluxus/src/PData.cpp \
                libfluxus/src/PDataOperator.cpp \
		libfluxus/src/PDataContainer.cpp \
		libfluxus/src/PDataArithmetic.cpp \
		libfluxus/src/GraphicsUtils.cpp \
		libfluxus/src/PNGLoader.cpp \
		libfluxus/src/PolyPrimitive.cpp \
		libfluxus/src/TextPrimitive.cpp \
		libfluxus/src/CompiledPrimitive.cpp \
		libfluxus/src/LinePrimitive.cpp \
		libfluxus/src/ParticlePrimitive.cpp \
		libfluxus/src/NURBSPrimitive.cpp \
		libfluxus/src/Primitive.cpp \
		libfluxus/src/Light.cpp \
		libfluxus/src/Renderer.cpp \
		libfluxus/src/SceneGraph.cpp \
		libfluxus/src/State.cpp \
		libfluxus/src/TexturePainter.cpp \
		libfluxus/src/Tree.cpp \
		libfluxus/src/dada.cpp \
		libfluxus/src/SearchPaths.cpp \
		libfluxphysics/src/Physics.cpp \
		src/AudioCollector.cpp \
		src/FluxusMain.cpp \
		src/FluxusBinding.cpp \
		src/JackClient.cpp \
		src/TurtleBuilder.cpp \
		src/GLEditor.cpp \
		src/Utils.cpp \
		src/OSCServer.cpp \
		src/Recorder.cpp \
		src/main.cpp")
FluxusVersion = "0.9osx-rc2"

env = Environment(CCFLAGS = '-ggdb -pipe -Wall -O3 -ffast-math -Wno-unused -fPIC')

Default(env.Program(source = Source, target = Target, LIBS=Libs, LIBPATH=LibPaths, CPPPATH=IncludePaths))

def BuildDmg(target, source, env):
	tmp_dmg = 'tmp-' + str(target[0])
	os.system('hdiutil create -size 32m -fs HFS+ -volname "Fluxus" ' + tmp_dmg)
	outs = os.popen('hdiutil attach ' + tmp_dmg, 'r')
	disk = outs.readline().split()[0]
	for src in source:
		# FIXME
		print src
		os.system('cp -r ' + str(src) + ' /Volumes/Fluxus')
	os.system('hdiutil detach ' + disk)
	os.system('hdiutil convert -format UDZO -o ' + str(target[0]) + ' ' + tmp_dmg)
	os.remove(tmp_dmg)
	return None

def Deprefix(path, prefix):
	res = ""
	while (path != prefix):
		(path,a) = os.path.split(path)
		res = os.path.join(a,res)
	return res
	
def CopyDir(target, source, env):
	destination = target
	for src in [str(x) for x in source]:
		for dirpath, dirs, files in os.walk(src):
			p = os.path.join(str(destination), Deprefix(dirpath,src))
			for f in files:
				env.InstallAs(os.path.join(p,f), os.path.join(dirpath,f))
			for d in dirs:
				# makes sure the empty directories are recreated
				env.Command(Dir(os.path.join(p,d)), [], 'mkdir -p $TARGET')

if env['PLATFORM'] == 'darwin':
	from osxbundle import *
	TOOL_BUNDLE(env)
	env.Replace(FRAMEWORKS = Split("GLUT OpenGL CoreAudio"))
	env.Replace(LINKCOM = "glibtool --mode=link g++ $LINKFLAGS $_LIBDIRFLAGS $_LIBFLAGS $_FRAMEWORKS -o $TARGET $SOURCES")
	env.Prepend(LINKFLAGS = ["-static"])
	env.MakeBundle("Fluxus.app", "fluxus", "key", "fluxus-Info.plist", typecode='APPL', icon_file='macos/fluxus.icns')
	#env['BUILDERS']['CopyDir'] = Builder(action = CopyDir)
	CopyDir("Fluxus.app/Contents/Resources/guile_scripts", ["/usr/local/share/guile/1.6"], env)
	env['BUILDERS']['DiskImage'] = Builder(action = BuildDmg)
	DmgFiles = [File("COPYING"), Dir("Fluxus.app"), Dir("docs"), Dir("examples"), Dir("scm")]
	env.Alias("dmg", env.DiskImage('Fluxus-' + FluxusVersion + '.dmg', DmgFiles))
else:
	env.Append(LIBPATH = ["/usr/X11R6/lib"])
	env.Append(LIBS = Split("jack X11 glut GL GLU"))
	env.Install(Install, Target)
	env.Alias('install', Install)

if not GetOption('clean'):
	print '--------------------------------------------------------'		
	print 'Fluxus: Configuring Build Environment'
	print '--------------------------------------------------------'		
	conf = Configure( env )	
	
	if not conf.CheckLibWithHeader('m', 'math.h', 'C'):
		print 'ERROR: libm must be installed!'
		Exit(1)   
		
	if not conf.CheckLibWithHeader('pthread', 'pthread.h', 'C'):
		print 'ERROR: glut must be installed!'
		Exit(1)   

	if not conf.CheckLibWithHeader('sndfile', 'sndfile.h', 'C'):
		print 'ERROR: sndfile must be installed!'
		Exit(1)   
	
	if not conf.CheckLibWithHeader('fftw3', 'fftw3.h', 'C'):
		print 'ERROR: fftw3 must be installed!'
		Exit(1)   
		
	if not conf.CheckLibWithHeader('lo', 'lo/lo.h', 'C'):
		print 'ERROR: liblo must be installed!'
		Exit(1) 
		  
	if not conf.CheckLibWithHeader('ode', 'ode/ode.h', 'C'):
		print 'ERROR: ode must be installed!'
		Exit(1)   

	if not conf.CheckLibWithHeader('guile', 'guile/gh.h', 'C'):
		print 'ERROR: guile must be installed!'
		Exit(1)   

	if not conf.CheckLibWithHeader('jack', 'jack/jack.h', 'C'):
		print 'ERROR: jack must be installed!'
		Exit(1)   

	env = conf.Finish()	
