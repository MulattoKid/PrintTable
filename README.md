# PrintTable
Simple C++ header-only library for printing formatted tables

## Usage
To use the library (generally in a *.cpp* file), simply include the header file 'PrintTable.h' where needed and define the implementation as so:
~~~~
#define PRINT_TABLE_IMPLEMENTATION
#include "PrintTable.h"
~~~~
(This is similar to *stb* and *tinyobjloader*).

If you only need access to the structure and not its member function definitions (generally in a *.h* file), only include the header file as so:
~~~~
#include "PrintTable.h"
~~~~
