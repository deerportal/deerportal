#!/bin/sh

[ -f "linuxdeploy-x86_64.AppImage" ] ||  wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
chmod 755 linuxdeploy-x86_64.AppImage
rm -rf CMakeCache.txt CMakeFiles

mkdir build
cd build
rm -rf CMakeCache.txt CMakeFiles
[ -d "SFML-2.5.1" ] || wget https://www.sfml-dev.org/files/SFML-2.5.1-linux-gcc-64-bit.tar.gz && tar -xvf SFML-2.5.1-linux-gcc-64-bit.tar.gz
export SFML_ROOT=SFML-2.5.1
cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release
make -j 4
../linuxdeploy-x86_64.AppImage --appdir AppDir
make install DESTDIR=AppDir
cp ../AppRun AppDir/
mkdir -p AppDir/usr/share/metainfo/
cp ../net.devcarpet.deerportal.appdata.xml AppDir/usr/share/metainfo/
#sed -i '/Exec/d' AppDir/deerportal.desktop
../linuxdeploy-x86_64.AppImage --appdir AppDir  --output appimage -i ../assets/img/deerportal.png -d ../net.devcarpet.deerportal.desktop

exit 0
[ -f "linuxdeploy-x86_64.AppImage" ] ||  wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
chmod 755 linuxdeploy-x86_64.AppImage
rm -rf CMakeCache.txt CMakeFiles

mkdir build
cd build
rm -rf CMakeCache.txt CMakeFiles
cmake .. -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release
make -j 4
../linuxdeploy-x86_64.AppImage --appdir AppDir
make install DESTDIR=AppDir
cp ../AppRun AppDir/
mkdir -p AppDir/usr/share/metainfo/
cp ../net.devcarpet.deerportal.appdata.xml AppDir/usr/share/metainfo/
sed -i '/Exec/d' AppDir/deerportal.desktop
../linuxdeploy-x86_64.AppImage --appdir AppDir  --output appimage -i ../assets/img/deerportal.png -d ../net.devcarpet.deerportal.desktop

