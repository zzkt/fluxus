Target       = "fluxus"
Install   	 = "/usr/local/bin"
LibPaths     = Split("/usr/local/lib /usr/X11R6/lib")
IncludePaths = Split("/usr/local/include")
	  
Libs = Split("fluxus  portaudio sndfile guile rfftw fftw3 ode fluxphysics png \
				glut tiff GL GLU z m Xi Xmu Xext X11 pthread lo")

Source = Split("src/AudioCollector.cpp \
				src/FluxusMain.cpp \
				src/FluxusBinding.cpp \
				src/PortAudioClient.cpp \
				src/TurtleBuilder.cpp \
				src/GLEditor.cpp \
				src/Utils.cpp \
				src/OSCServer.cpp \
				src/main.cpp")					

env = Environment(CCFLAGS = '-pipe -Wall -O3 -ggdb -ffast-math -Wno-unused -fPIC')
env.Program(source = Source, target = Target, LIBS=Libs, LIBPATH=LibPaths, CPPPATH=IncludePaths)

env.Install(Install, Target)
env.Alias('install', Install)
