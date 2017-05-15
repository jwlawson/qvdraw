# qvdraw utils

This repo provides a number of tools and utilities for drawing quivers, exchange
graphs and other cluster objects. The main tools included are:

* `qvdraw` - A bash script to draw a picture representation of a quiver.

* `qv2tex` - A program to compute exchange graphs and output a LaTeX file which
             uses Tikz to draw the graph

* `qv*2gml` - A program to convert various objects into a gml representation.

## qvdraw

Draws quivers from the matrix representation.

### Usage

```
qvdraw [-nh] [-o "opts"] [filename]
```

##### Input

`filename` should be the name of a file which contains a number of matrix
representations of quivers (see [Matrix format](#matrix)). If the files
contains anything else then it could cause errors.

If no filename is specified then the script assumes the input will be stdin.
This allows other programs to pipe into `qvdraw`.

* `-n` supresses the direction of arrows. If this option is not specified then
	all arrows are directed, if `-n` is specified then the edges in the quiver
	will not be directed.

* `-h` show usage.

* `-o "options"` passes any options to the `gml2pic` program. Examples include
	`-width pixels` and `-pic filetype`. See the [gml2pic usage][gml2pic docs] docs to
	see all options.

##### Output

For each line in `filename`, a picture of the quiver will be created in the
current working directory. These will always be named `1.png`, `2.png` etc.
There is currently no detection of whether files with these names already
exists, so any such files will be overwritten.

##### Structure<a name="structure"></a>

qvdraw is a bash script which uses three programs:

 * qv2gml
 * gmlayout
 * gml2pic

The first two are included in this repo, while `gml2pic` can be found on the
[OGDF website][gml2pic site].


## qv2tex
`qv2tex` outputs LaTeX code to generate pictures of various cluster objects.

##### Usage
```
qv2tex -lr [-n number] [-q|m|g|e|c quiver]
Takes a qv matrix and outputs the TeX to draw the quiver.
   -q Draw a single quiver
   -m Draw the move graph of a quiver
   -g Draw the quiver graph of a quiver
   -e Draw the exchange graph of a quiver with cluster (x1 ... )
   -c Draw the quasi-Cartan companion exchange graph
   -l Draw the labelled exchange/quiver graph
   -n Limit the number of seeds computed to given number
   -r Don't compute mutations which do not lead to green sequences
```

The main options are `-q`, `-m`, `-g`, `-e`, `-c` which specify what type of
graph the program should output. Each of these options should be followied by a
matrix representing a quiver, as described in [Matrix format](#matrix):

 * `-q` Outputs a single quiver, provided as a matrix.
 * `-m` Draws the minimal mutation-infinite move graph of the MMI quiver.
 * `-g` Draws the quiver exchange graph of the provided quiver.
 * `-e` Draws the exchange graph, with initial quiver given by (x1, ...). This
	 graph will be the slowest to compute, especially as the number of vertices
	 increases, as the symbolic manipulation is not very fast.
 * `-c` Draws the quiver exchange graph, but each quiver is decorated with the
	 fully-compatible quasi-Cartan companion. The graph will stop at a vertex if
	 the mutated quasi-Cartan is no longer fully-compatible.

The `-l` option can be added to the `-g` and `-e` options to specify that the
graph should be the labelled version, where each vertex is a labelled quiver
or seed and not considered up to permutations.

The `-n` option can be used to specify a maximum number of vertices to compute.
This allows the user to draw a section of an infinite graph, or stop after a
certain time if the computations are taking too long.

The `-r` option is not fully implemented, but tries to construct exchange graphs
which contain only those mutations which could apear in a maximal green
sequence. Currently this only prevents mutations at the source of a multiple
arrow.

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

Run `make` to compile all utilities.

The `qvdraw` script requires the three programs specified [above](#structure)
and so either keep the programs in the same folder, or ensure they are included
in your path.

### Dependencies

`qv2gml` and `gmlayout` require the following libraries and their headers to be
installed and accessible by the compiler:

 * [libqv][libqv site]
 * [OGDF][ogdf site]

`qv2tex` also requires:

 * [libqvrefl]
 * [ginac]

The Makefile can be modified to include the install locations of these
libraries, by changing the `INCLUDES` and `LFLAGS` variables.

[gml2pic site]: http://www.ogdf.net/doku.php/project:gml2pic
[gml2pic docs]: http://www.ogdf.net/doku.php/project:gml2pic-doc
[OGDF site]: http://www.ogdf.net/ogdf.php
[libqv site]: https://github.com/jwlawson/qv
[libqvrefl]: https://github.com/jwlawson/qvrefl
[ginac]: https://www.ginac.de/
