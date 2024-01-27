## Note
> module rscuad, update if there are new features.<br>
> make good code and make sure it's not redundancy.<br>
> don't use std namespacing in library 

## Create new feature
> read the <a href="https://github.com/R-SCUAD-PROGRESS/Guidelines">guideline</a> rscuad before add features

## Setup
> setup makefile to get path, you can add path in path.txt

## Testing modul

        $ make
        $ make clean
        $ ./test

## Checking memory leak
> run memory.sh and see if memory can drop or not
- for detail use make check
- install

        $ sudo apt-get install valgrind
- type:

        $ make check 

- and see in memoryleak.rpt and make sure 0 blocks in result

## Copyright
> RSCUAD TEAM
