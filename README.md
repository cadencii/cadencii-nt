# Cadencii

Cadencii is a VSQ file viewer.

## License

GNU General Public License version 2.0.

## Build

clone git repository.

    git clone https://github.com/kbinani/cadencii.git
    cd cadencii
    git submodule init
    git submodule update

build with `make`,

    qmake -o Makefile Cadencii.pro   # use qmake-q4, if you don't have qmake.
    make

or open Cadencii.pro with QtCreator, and build.

