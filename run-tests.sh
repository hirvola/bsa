#!/bin/sh

p=1
e=3
T=3000

# String matching with k mismatches
for text in ./1MB/genome/ecoli.txt ./1MB/english/bible.txt ./1MB/rand256/rand256.txt; do
    for alg in ./naive ./abm ./abndm ./tusa ./twsa ./bsa; do
        for k in 1 2 3; do
            for m in 8 12 16 20; do
                echo "$ $alg -p$p -e$e -k$k $text < $text.$m.pat" >&2
                timeout $T $alg -p$p -e$e -k$k $text < $text.$m.pat
            done
        done
    done
done

# Circular string matching with k mismatches
for text in ./1MB/genome/ecoli.txt ./1MB/english/bible.txt ./1MB/rand256/rand256.txt; do
    for alg in ./casb ./cacb ./cbsa; do
        for k in 1 2 3; do
            for m in 8 12 16 20; do
                echo "$ $alg -p$p -e$e -k$k $text < $text.$m.pat.rot" >&2
                timeout $T $alg -p$p -e$e -k$k $text < $text.$m.pat.rot
            done
        done
    done
done
