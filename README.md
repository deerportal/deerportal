[![devcarpet](https://devcarpet.net/images/dc_black.png)](https://devcarpet.net)[![Build Status](https://travis-ci.org/deerportal/deerportal.svg?branch=master)](https://travis-ci.org/deerportal/deerportal) [![Build Status](https://semaphoreci.com/api/v1/bluszcz/deerportal/branches/master/badge.svg)](https://semaphoreci.com/bluszcz/deerportal) [![Build Status](https://snap-ci.com/deerportal/deerportal/branch/master/build_image)](https://snap-ci.com/deerportal/deerportal/branch/master)

presents

Deerportal
-----------

![DeerPortal Game play](https://bluszcz.net/projects/games/deerportal/selection_754.png/@@images/image.png)


Deerportal is a open source hybrid game which utilize  board and card games mechanisms. World of the game is driven by four classical elemets along with Almighty Deer God.

Homepage: https://devcarpet.net/deerportal/

Collect the diamonds, execute actions on cards and survive the chaos of nature! Compete against 3 other players.

Features
--------

* 0-4 players mode
* changing seasons
* open source (zlib / cc-by 4.0)

SFML, Linux, OSX, Windows.

![DeerPortal MultiPlayer Mode](https://bluszcz.net/projects/games/deerportal/deerportal-game-about-how-human-can-be-upgraded-to-the-deer_771.png/@@images/image.png)

Installation
------------
------------

Ubuntu Trusty
-------------

```
$ sudo apt-get install libsfml-devl
$ git clone https://github.com/deerportal/deerportal.git
$ cd deerportal
$ qmake -makefile pagan_board.pro
$ make
$ make install
$ ../build_release_pagan_board/pagan_board
```

Download
--------

In our [releases](https://github.com/deerportal/deerportal/releases) section we are providing compiled binaries for Ubuntu (Trusty, Xenial) and Windows.

Ubuntu apt repo
---------------

If you want to have an easy installation via system apt, add one of the folowing (depends on your distro) to your /etc/apt/sources.list, if you have Trusty 14.04
```
deb http://deb.devcarpet.net/ubuntu trusty main
```
or for Xenial 16.04
```
deb http://deb.devcarpet.net/ubuntu xenial main
```

and later:
```
sudo apt-get update
sudo apt-get install deerportal
```

Development windows builds
--------------------------

* https://devcarpet.net/deerportal/downloads/windows/

Developer documentation
-----------------------

Following doxygen api docs has been created using our Jenkins based Continous Integration:

* https://devcarpet.net/deerportal/downloads/docs/html/

**Developed with [Devcarpet](https://devcarpet.net) platform.**
