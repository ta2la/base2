wasm {
    CONFIG(debug, debug|release): SOURCEDIR = debug_wasm
    else:                         SOURCEDIR = release_wasm
} else {
    CONFIG(debug, debug|release): SOURCEDIR = debug
    else:                         SOURCEDIR = release
}

BUILDDIR = $$PWD/../BUILD/$$TARGET/$$SOURCEDIR

DESTDIR      = $$BUILDDIR
OBJECTS_DIR  = $$BUILDDIR/compilation
MOC_DIR      = $$BUILDDIR/compilation
UI_DIR       = $$BUILDDIR/compilation
RCC_DIR      = $$BUILDDIR/compilation
QMLCACHE_DIR = $$BUILDDIR/compilation
