HOST_NAME = $$QMAKE_HOST.name
win32:contains(HOST_NAME,SIS000307) {
    message( "Chiryshev Iurii $$HOST_NAME detected" )
    include(custom\sis000307.pri)
} # Chiryshev Iurii PCs

SPEC = $$basename(QMAKESPEC)

UI_DIR      = .ui/$${SPEC}
MOC_DIR     = .moc/$${SPEC}
OBJECTS_DIR = .obj/$${SPEC}
RCC_DIR     = .resources/$${SPEC}

CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/bin/$${SPEC}/Debug
}

CONFIG(release, debug|release) {
    DESTDIR = $$PWD/bin/$${SPEC}/Release
}
