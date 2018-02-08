#!/bin/bash
rm -rf builds/build_release_deerportal_win32/*
export PATH=/home/services/mxe/usr/bin/:~/mxe/usr/bin/:$PATH
~/mxe/usr/i686-w64-mingw32.static/qt/bin/qmake deerportal_windows.pro "DESTDIR=build_release_deerportal_win32"
echo $PATH
make
make install
ls -lah builds/build_release_deerportal_win32
cd builds
export FILENAME="deerportal_win32-`date +%Y%m%d%H%M%S`.zip"
zip -9 -r $FILENAME build_release_deerportal_win32/
ls -lah $FILENAME
cp $FILENAME /mnt/deerportal_downloads/windows/