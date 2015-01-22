# qvdraw

Draws quivers from the matrix representation.

### Usage

```
qvdraw [-nh] [filename]
```

##### Input

`filename` should be the name of a file which contains a number of matrix
representations of quivers (see [Matrix format](#matrix)). If the files
contains anything else then it could cause errors.

If no filename is specified then the script assumes the input will be stdin.
This allows other programs to pipe into `qvdraw`.

The `-n` flag can be added if the direction of arrows in the quiver is not
important. If this option is not specified then all arrows are directed, if
`-n` is specified then the edges in the quiver will not be directed.

If the `-h` flag is specified, then a short usage description is printed.

##### Output

For each line in `filename`, a picture of the quiver will be created in the
current working directory. These will always be named `1.png`, `2.png` etc.
There is currently no detection of whether files with these names already
exists, so any such files will be overwritten.

### Structure<a name="structure"></a>

qvdraw is a bash script which uses three programs:

 * qv2gml
 * gmlayout
 * gml2pic

The first two are included in this repo, while `gml2pic` can be found on the
[OGDF website][gml2pic site].

### Matrix format<a name="matrix"></a>

The matrix format expected is consistent with that used in the `libqv` library.
This allows any output from `qv*` programs to be used as input to `qvdraw`.

```
{ { a b c } { d e f } { g h i } }
```

* The matrix starts and ends with curly braces. `{ matrix }`
* Each row of the matrix is contained in curly braces. `{ row entries }`
* Each entry must be separated by a space.

If the input is not in this form, the chances are the program will throw an
exception.

### Build

Run `make` to compile the required `qv2gml` and `gmlayout`.

The `qvdraw` script requires the three programs specified [above](#structure)
and so either keep the programs in the same folder, or ensure they are included
in your path.

### Dependencies

`qv2gml` and `gmlayout` require the following libraries and their headers to be
installed and accessible by the compiler:

 * [libqv][libqv site]
 * [OGDF][ogdf site]

The Makefile can be modified to include the install locations of these
libraries, by changing the `INCLUDES` and `LFLAGS` variables.

[gml2pic site]: http://www.ogdf.net/doku.php/project:gml2pic
[OGDF site]: http://www.ogdf.net/ogdf.php
[libqv site]: http://github.com/jwlawson/qv
