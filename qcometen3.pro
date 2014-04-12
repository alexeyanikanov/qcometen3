
SOURCES += bezier.cpp cometen3.cpp cometenmath.cpp cometenscene.cpp \
	curvecomet.cpp decal.cpp explosion.cpp glow.cpp kcometen3.cpp \
	kglcometen3.cpp kscreensaver.cpp lightning.cpp main.cpp \
	particlesystem.cpp pcomet.cpp rotatecomet.cpp vec.cpp
HEADERS += bezier.h comet.h cometen3.h cometenmath.h cometenscene.h \
	curvecomet.h decal.h explosion.h global.h glow.h kcometen3.h \
	kglcometen3.h kscreensaver.h kscreensaver_vroot.h lightning.h \
	particlesystem.h pcomet.h rotatecomet.h vec.h
RESOURCES += pixmaps.qrc
CONFIG = qt release resources opengl
QT += opengl

