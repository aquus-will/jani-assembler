# Read Me

This project refers to a master's degree deliverable of a compiler for a reconfigurable environment.

**This is NOT the main compiler. This is an assembler to generate binaries agnostically and test the Nios II simulator**

The work is all detailed in: https://www.dcce.ibilce.unesp.br/spd/pubs/Diss-10-WLima.pdf (_portuguese_)

The main article about it is in: https://www.dcce.ibilce.unesp.br/spd/pubs/ISCC2009.pdf (_english_)

## Install

Just run the `make` command on your machine and the build should work fine.

The Makefile was created for Windows platform, so, if you're working on another OS, you might want to edit it in the `clean` section.

All object files should be inside the directory `obj`.

## Usage

Once compiled, you must run the `builder` with one argument, informing which is the input file (extension `.nios`). There several samples inside the directory `input`.

Example (Windows):
````batch
.\builder.exe .\input\add_carry.nios
````

## Contributing

PRs accepted, but make sure you've understanded its goal by reading the mentioned dissertation and/or the IEEE article.

## License

Author: Willian dos Santos Lima - [@aquus-will](https://github.com/aquus-will)

Distributed and Parallel Systems Group - [GSPD](https://www.dcce.ibilce.unesp.br/spd/pindex.php)

UNESP - Sao Paulo State University - Sao Jose do Rio Preto - Brazil

_Since 2007_
