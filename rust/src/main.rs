extern crate itertools;
use std::str;
use std::error::Error;
use std::io::SeekFrom;
use std::io::prelude::*;
use std::fs::File;
use std::path::Path;

struct BitSuffix {
    bitcount: u32,
    exemplar: u64,
    //sigstr: Vec<u8>,
}

impl BitSuffix {
    fn into_bytes(&self) -> [u8; 26] {
        let s = format!("{:12x} {:12}\n", self.exemplar, self.bitcount);
        let mut readbuf = [0u8; 26];
        readbuf.copy_from_slice(s.as_bytes());
        return readbuf;
    }

    fn from_bytes(buf: &[u8; 26]) -> BitSuffix {
        let s = str::from_utf8(buf).unwrap();
        let i = u64::from_str_radix(s[0..12].trim_start(), 16).unwrap();
        let bitcount = u32::from_str_radix(s[13..25].trim_start(), 10).unwrap();
        return BitSuffix { bitcount: bitcount, exemplar: i };
    }
}

//fn run_collatz(ex: u64) -> (u32, Vec<u8>) {
fn run_collatz(ex: u64) -> u32 {
    // only operates on odd positive integers
    assert!(ex > 0 and ex % 2 == 1);

    let mut cc = ex;
    //let mut sig = Vec::new();
    let mut bc = 0;

    //sig.push(1);
    cc = cc * 3 + 1;

    while cc > ex {
        if cc % 2 == 1 {
            //sig.push(1);
            cc = cc * 3 + 1;
        } else {
            //sig.push(0);
            cc = cc / 2;
            bc += 1;
        }
    }

    return bc;
}

fn main() {
    let cutoff = 18;
    let mut bitcount = 1;
    //let mut sigs : Vec<BitSuffix> = Vec::new();
    let path = Path::new("collatz_exemplars.txt");
    let display = path.display();

    let mut outfile = match File::create(&path) {
        Err(why) => panic!("couldn't create {}: {}",
                           display,
                           why.description()),
        Ok(file) => file,
    };

    let mut infile = match File::open(&path) {
        Err(why) => panic!("couldn't open for reading {}: {}",
                           display,
                           why.description()),
        Ok(file) => file,
    };

    fn compute_and_push(ex : u64, ofile : &mut File) {
        //let results = run_collatz(ex);
        //let xx = BitSuffix {bitcount: results.0, exemplar: ex, sigstr: results.1};
        let bitcount = run_collatz(ex);
        let xx = BitSuffix {bitcount: bitcount, exemplar: ex};
        ofile.write_all(&xx.into_bytes()).unwrap();
        //println!("{:0}: {:1} -- {:?}", xx.exemplar, xx.bitcount, xx.sigstr);
        //newsigs.push(xx);
    };

    loop {
        println!("Check new bit patterns of lenth {}", bitcount);

        //let mut newsigs : Vec<BitSuffix> = Vec::new();
        let base = 1 << (bitcount - 1);

        if bitcount == 1 {
            // seed this
            compute_and_push(1, &mut outfile);
        } else {
            infile.seek(SeekFrom::Start(0)).unwrap();
            let mut readbuf = [0u8; 26];
            loop {
                infile.read_exact(&mut readbuf).unwrap();

                let sig = BitSuffix::from_bytes(&readbuf);
                if sig.exemplar > base {
                    break;
                }
                if sig.bitcount >= bitcount {
                    compute_and_push(base+sig.exemplar, &mut outfile);
                }
            }
        }

        //sigs.extend(newsigs);

        bitcount += 1;

        if bitcount == cutoff {
            break;
        }
    }

    drop(infile);
    drop(outfile);

/*
    sigs.sort_by_key(|elt| elt.bitcount);

    for (k, grp) in &sigs.into_iter().group_by(|elt| elt.bitcount) {
        if k >= cutoff {
            break;
        }
        println!("bit {}: count {}", k, grp.into_iter().count());
    } */
}
