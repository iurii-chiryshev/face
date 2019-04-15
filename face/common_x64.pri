HOST_NAME = $$QMAKE_HOST.name
#-------------------------------------------------
#   Chiryshev Iurii <iurii.chiryshev@mail.ru>
#   DESKTOP-QRN46PP - home pc
#   DESKTOP-48BO0EE - work pc
#-------------------------------------------------
win32:contains(HOST_NAME,DESKTOP-QRN46PP|DESKTOP-48BO0EE) {
    message( "Chiryshev Iurii $$HOST_NAME detected" )
    include(common\uchir_x64.pri)
} # Chiryshev Iurii PCs
