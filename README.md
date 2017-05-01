Bit-Parallel Approximate Matching of Circular Strings with *k* Mismatches
=========================================================================

Contents
--------

* Algorithms for approximate string matching with *k* mismatches:
    * [abm.c](abm.c) Approximate Boyer-Moore.
    * [abndm.c](abndm.c) Approximate BNDM (based on Shift-Add).
    * [asb.c](asb.c) Approximate SBNDM.
    * [bsa.c](bsa.c) Backward Shift-Add.
    * [naive.c](naive.c) A naive k-mismatches algorithm.
    * [sa.c](sa.c) Shift-Add.
    * [tuabndm.c](tuabndm.c) Tuned Approximate BNDM (based on Backward Shift-Add).
    * [tusa.c](tusa.c) Tuned Shift-Add.
    * [twsa.c](twsa.c) Two-way Shift-Add.

* Algorithms for approximate circular string matching with *k* mismatches:
    * [cacb.c](cacb.c) Circular Approximate CBNDM.
    * [cacbr.c](cacbr.c) Circular Approximate CBNDM with reversed bit-vectors.
    * [casb.c](casb.c) Circular Approximate SBNDM.
    * [cbsa.c](cbsa.c) Circular Backward Shift-Add.
    * [cnaive.c](cnaive.c) A naive k-mismatches algorithm for circular strings.

* Test data (`n=1MB` prefixes of texts from the [SMART](https://www.dmi.unict.it/~faro/smart/) corpus):
    * [1MB/english/bible.txt](1MB/english/bible.txt) KJV bible.
    * [1MB/genome/ecoli.txt](1MB/genome/ecoli.txt) Genome of E. coli.
    * [1MB/rand256/rand256.txt](1MB/rand256/rand256.txt) Uniform random data with byte alphabet.
    * Moreover, pregenerated pattern sets are included in the subdirectories.

* Scripts
    * [1MB/pat-gen.py](1MB/pat-gen.py) Pattern generator.
    * [1MB/gen-all.sh](1MB/gen-all.sh) Script to generate pattern sets.
    * [run-tests.sh](run-tests.sh) Runs a set of tests.

Usage
-----

The algorithms should compile straight out of the box on most UNIX-like systems
by issuing `make`. A successful compilation produces a total of 13 test programs
that share the same command-line interface (implemented in [main.c](main.c)):

```bash
$ ./cbsa
Usage: ./cbsa [-k err] [-p preps] [-e execs] textfile <patterns
```

* Command-line arguments
    * `-k` specifies the number of allowed mismatches (default: 0).
    * `-p` the number of times to execute the preprocessing routine.
    * `-e` the number of times to execute the search routine.
    * `textfile` a file containing the text.
    * `patterns` a file of newline-delimited patterns to search for in the text.

The output of the program shows the input parameters, the number of approximate
occurrences found in the text, and the *user* time elapsed during preprocessing
and searching. Thus, `-p` and `-e` are useful for benchmarking purposes.

### Example ###

Search *"well"*, *"jello"* and both in the text *"hello world*" with 2 allowed
mismatches using the Backward Shift-Add algorithm:

```bash
$ echo "hello world" >text  
$ echo "well" >pattern
$ ./bsa -k2 text <pattern
alg=./bsa T=text(12) np=1 m=4 k=2 occ=2 p=0.0000s(0/1) e=0.0000s(0/1)
$ echo "jello" | ./bsa -k2 text 
alg=./bsa T=text(12) np=1 m=5 k=2 occ=1 p=0.0000s(0/1) e=0.0000s(0/1)
$ echo "jello" | cat pattern - | ./bsa -k2 text
alg=./bsa T=text(12) np=2 m=4..5 k=2 occ=3 p=0.0000s(0/1) e=0.0000s(0/1)
```

Note that `-k0` performs an exact search. For example, let us search for the
exact *circular* pattern *"lol"* with the Circular Backward Shift-Add algorithm:

```bash
$ cat text
hello world
$ echo "lol" | ./cbsa -k0 text 
alg=./cbsa T=text(12) P=1 m=3 k=0 occ=1 p=0.0000s(0/1) e=0.0000s(0/1)
```

Finally, benchmark the Backward Shift-Add algorithm by searching a set of 1000
patterns of length `m=20` in the KJV bible while allowing 3 mismatches:

```bash
$ wc -l 1MB/english/bible.txt.20.pat                      
1000 1MB/english/bible.txt.20.pat
$ ./bsa -k3 -p100 -e100 1MB/english/bible.txt <1MB/english/bible.txt.20.pat
alg=./bsa T=1MB/english/bible.txt(1048576) np=1000 m=20 k=3 occ=6122 p=0.0001s(1/100) e=0.8041s(8041/100)
```
