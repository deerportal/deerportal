Devcarpet                         {#mainpage}
=========
presents

Deerportal
----------

Deerportal is a open source (free as a speech, free as a beer) hybrid game which utilize  board and card games mechanisms. World of the game is driven by four classical elemets along with Almighty Deer God.

Homepage: https://devcarpet.net/deerportal/

Collect the diamonds, execute actions on cards and survive the chaos of nature! Compete against 3 other players.

Features
--------

* 0-4 players mode
* changing seasons
* open source (zlib / cc-by 4.0)

SFML 3, Linux, OSX, Windows.

## Installation

### Compilation

#### OSX

```
$ git clone https://github.com/deerportal/deerportal.git
$ cd deerportal
$ cmake .
$ make
$ sudo make install
$ open DeerPortal.app
```

##### Debian 

If you would like to see Deer Portal in Debian repositories, there is an opened bug: https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=826612 - please help :)

To make deb package locally:

```
$ git clone https://github.com/deerportal/deerportal.git
$ cd deerportal
$ fakeroot debian/rules binary
```




**Developed with [Devcarpet](https://devcarpet.net) platform.**
