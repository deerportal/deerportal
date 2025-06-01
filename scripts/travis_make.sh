#!/bin/bash

if [ $TRAVIS_OS_NAME = 'linux' ]; then
  sh make_appimage.sh
fi

if [ $TRAVIS_OS_NAME = 'osx' ]; then
  cmake .
  make
fi

