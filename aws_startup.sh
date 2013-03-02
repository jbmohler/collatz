#!/bin sh

sudo aptitude update
sudo aptitude install g++ make m4
sudo aptitude install python-dev
sudo aptitude install mercurial
mkdir work
cd work/
hg clone https://jbmohler@bitbucket.org/jbmohler/collatz
cd collatz/
./build_mpir.sh

LD_LIBRARY_PATH=./local/lib/ ./scripts/seed.py -m 24
