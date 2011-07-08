@ECHO OFF
del makefile > NUL 2> NUL
copy makefile.win makefile > NUL
ECHO   done. run make to build
