# Calculon

Calculon is a command line calculator supporting typical operations on integers,
doubles, and strings. The operations it supports are addition, subtraction,
multiplication, division, integer division, modulos, exponents, and string
concatenation. Variables may be used to store intermediate results.

Calculon was created as part of data structures and algorithms course at The
University of Alabama.

## Setup

Simply running `make` will compile the executable, "calculon".

## Usage

Calculon takes a text file as input; the file may be provided through standard
input or by supplying the name of the file as a command line argument. As such,
the following ways of invoking Calculon from a POSIX shell are equivalent:

    ./calculon in.txt

#

    cat in.txt | calculon

Calculon will output the value of the last expression in the file. The input has
fairly strict formatting requirements; expressions must be terminated with a
semicolon, and individual elements must be separated by whitespace.

Variables can be declared using this syntax: "var name = [expression]". For
example, running `echo 'var a = 1 ; 5 * ( a + a ) ; ' | calculon` outputs `10`.

Calculon makes a distinction between integers and floating point numbers. As
such, the expression `9 / 4` will perform integer division and result in a value
of `2`. Integer promotion occurs as expected; `9.0 / 9 / 2` results in `0.5` and
`9.0 / ( 9 / 2 )` results in `2.25`.

When run with the "-d" flag, Calculon will print the last expression to standard
output in postfix notation instead of evaluating it. For example, `echo '1 * 2 -
3 / 4 ; ' | ./calculon -d` prints `1 2 * 3 4 / -`
