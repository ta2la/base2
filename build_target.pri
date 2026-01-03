SOURCEDIR = debug

defined(BUILDROOT, var): ROOT_DIR = $$BUILDROOT/$$TARGET/$$SOURCEDIR
else:                    ROOT_DIR = $$PWD/../BUILD/$$TARGET/$$SOURCEDIR

DESTDIR      = $$ROOT_DIR
OBJECTS_DIR  = $$ROOT_DIR/compilation
MOC_DIR      = $$ROOT_DIR/compilation
UI_DIR       = $$ROOT_DIR/compilation
RCC_DIR      = $$ROOT_DIR/compilation
QMLCACHE_DIR = $$ROOT_DIR/compilation

FORCE =

QMAKE_EXTRA_COMPILERS += make_obj_dirs
make_obj_dirs.commands = mkdir -p $$OBJECTS_DIR
make_obj_dirs.depends = FORCE
make_obj_dirs.output = $$OBJECTS_DIR/.dir_created
