# Introduction

This is me learning rust.  So far I have:

* fought with the borrow checker
* distinguished bytes and strings (with performance testing)
* learned about mutable references
* wrote file IO learning about errors
* learned about scope with a closure
* use crates specified in Cargo.toml

# The Collatz conjecture

See [Wikipedia](https://en.wikipedia.org/wiki/Collatz_conjecture)

My approach utilizes a deep understanding of Collatz glides and how they are
determined by the final binary digits of arithmetic sequences with common
difference of some power of 2.

# Rust installation & Configuration

Install rust:

    curl https://sh.rustup.rs -sSf | sh
    source $HOME/.cargo/env 

Prepare debian (or similar):

    sudo apt-get install build-essential
