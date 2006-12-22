mzc --compiler /usr/bin/g++ --cc src/FluxusEngine.cpp
mzc --compiler /usr/bin/g++ --cc src/FluxusGlobalState.cpp
mzc --compiler /usr/bin/g++ --cc src/FluxusLocalState.cpp
mzc --compiler /usr/bin/g++ --cc src/FluxusMaths.cpp
mzc --compiler /usr/bin/g++ --cc src/FluxusPrimitives.cpp
mzc --compiler /usr/bin/g++ --cc src/Common.cpp
mzc --linker /usr/bin/g++ --ld fluxus-engine.so \
	FluxusEngine.o \
	FluxusGlobalState.o \
	FluxusLocalState.o \
	FluxusPrimitives.o \
	FluxusMaths.o \
	Common.o \
	../../libfluxus/libfluxus.a \
	-O3 -lGL -lGLU -lglut -lGLEW
