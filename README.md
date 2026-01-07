AOC 2025 in C with a little bit of python.
An attempt to learn C, having never written it before aside from another advent of code, which you can also find on this github.

This was written at the recurse center in December 2025.

If you're interested in compiling or running any of this, here are instructions

To compile everything run

```bash
make
```

or

```bash
make all
```

To compile a specific day run

```bash
make day day=03
```
change 03 appropriately, days should be between 01 and 12.

To compile a specific part of a specific day, run

```bash
make part day=04 part=01
```
Again, change the day as necessary. part should be 01 or 02. Note there is no part 2 for day 12

If you'd like to run the code, you'll need to put your input files for each day in the appropriate directory with the name "input.txt". So your input file for the fourth day should be 04_print/input.txt

Warning: day 10 part 2 will take a bit of time (20-30 seconds for me) since I implemented no caching in the search.

To run everything:
```bash
make run
```
```bash
make run-day day=04
```
```bash
make run-part day=04 part=01
```
Change days and parts as you need.
