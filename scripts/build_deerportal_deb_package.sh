#!/bin/bash
VERSION=0.6
DISTRO=`lsb_release -c|awk '{print $2}'`
mkdir ~/debs-tests
cd ~/debs-tests
rm -rf ~/debs-tests/*
ls
git clone --depth 1 https://github.com/deerportal/deerportal.git
cd deerportal
rm deerportal_*pro pagan*pro
cd ..
rsync -Pa --exclude .git deerportal/ deerportal-$VERSION/
#tar -cjvf deerportal_0.0.5.orig.tar.bz2 deerportal-$VERSION/
cd deerportal-$VERSION
sed -i "1 s/)/-${DISTRO})/" debian/changelog
cat debian/changelog
dpkg-buildpackage -b -rfakeroot -us -uc


