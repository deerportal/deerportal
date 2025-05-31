Devcarpet
=========
presents

Deerportal
----------

Deerportal is an open source (free as speech, free as beer) hybrid game that utilizes board and card game mechanisms. The world of the game is driven by four classical elements along with the Almighty Deer God.

Homepage: https://devcarpet.net/deerportal/

Collect diamonds, execute actions on cards, and survive the chaos of nature! Compete against 3 other players.

Features
--------

* 0-4 players mode
* changing seasons
* open source (zlib / cc-by 4.0)

SFML 3, Linux, macOS, Windows.

## Installation

### Compilation

#### macOS

```bash
$ git clone https://github.com/deerportal/deerportal.git
$ cd deerportal

# For a Release build (recommended for performance):
$ cmake -DCMAKE_BUILD_TYPE=Release .
# Or, for a Debug build (for development and debugging):
# $ cmake -DCMAKE_BUILD_TYPE=Debug .

# To always show the FPS counter (e.g., in a Release build for testing):
# $ cmake -DCMAKE_BUILD_TYPE=Release -DSHOW_FPS_COUNTER=ON .
# The FPS counter is automatically shown in Debug builds.

$ make
$ sudo make install
$ open DeerPortal.app
```

##### Debian 

If you would like to see Deer Portal in Debian repositories, there is an open bug: https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=826612 - please help :)

To make a deb package locally:

```
$ git clone https://github.com/deerportal/deerportal.git
$ cd deerportal
$ fakeroot debian/rules binary
```

**Developed with [Devcarpet](https://devcarpet.net) platform.**
