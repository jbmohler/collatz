extern crate itertools;
use std::str;
use std::error::Error;
use std::io::SeekFrom;
use std::io::prelude::*;
use std::fs::File;
use std::path::Path;

fn format_hex(buf : &mut [u8], value : u64) {
    let mut t = value;
    for index in (0..buf.len()).rev() {
        let v = match t % 16 {
            0 => b'0',
            1 => b'1',
            2 => b'2',
            3 => b'3',
            4 => b'4',
            5 => b'5',
            6 => b'6',
            7 => b'7',
            8 => b'8',
            9 => b'9',
            10 => b'a',
            11 => b'b',
            12 => b'c',
            13 => b'd',
            14 => b'e',
            15 => b'f',
            _ => panic!("arithmetic confusion")
        };
        buf[index] = v;
        t >>= 4;
        if t == 0 {
            break;
        }
    }
}

fn format_decimal(buf : &mut [u8], value : u32) {
    let mut t = value;
    for index in (0..buf.len()).rev() {
        let v = match t % 10 {
            0 => b'0',
            1 => b'1',
            2 => b'2',
            3 => b'3',
            4 => b'4',
            5 => b'5',
            6 => b'6',
            7 => b'7',
            8 => b'8',
            9 => b'9',
            _ => panic!("arithmetic confusion")
        };
        buf[index] = v;
        t = t / 10;
        if t == 0 {
            break;
        }
    }
}

fn read_hex(buf : &[u8]) -> u64 {
    let mut t = 0u64;
    for index in 0..buf.len() {
        if buf[index] == b' ' {
            if t != 0 {
                panic!("invalid buffer format");
            }
        } else {
            let v = match buf[index] {
                b'0' => 0,
                b'1' => 1,
                b'2' => 2,
                b'3' => 3,
                b'4' => 4,
                b'5' => 5,
                b'6' => 6,
                b'7' => 7,
                b'8' => 8,
                b'9' => 9,
                b'a' => 10,
                b'b' => 11,
                b'c' => 12,
                b'd' => 13,
                b'e' => 14,
                b'f' => 15,
                _ => panic!("arithmetic confusion")
            };
            t = t * 16 + v;
        }
    }
    t
}

fn read_decimal(buf : &[u8]) -> u32 {
    let mut t = 0u32;
    for index in 0..buf.len() {
        if buf[index] == b' ' {
            if t != 0 {
                panic!("invalid buffer format");
            }
        } else {
            let v = match buf[index] {
                b'0' => 0,
                b'1' => 1,
                b'2' => 2,
                b'3' => 3,
                b'4' => 4,
                b'5' => 5,
                b'6' => 6,
                b'7' => 7,
                b'8' => 8,
                b'9' => 9,
                _ => panic!("arithmetic confusion")
            };
            t = t * 10 + v;
        }
    }
    t
}

struct BitSuffix {
    bitcount: u32,
    exemplar: u64,
    //sigstr: Vec<u8>,
}

impl BitSuffix {
    fn into_bytes(&self) -> [u8; 26] {
        let mut readbuf = [32u8; 26];
        readbuf[25] = 10;
        format_hex(&mut readbuf[0..12], self.exemplar);
        format_decimal(&mut readbuf[13..25], self.bitcount);
        return readbuf;
    }

    fn from_bytes(buf: &[u8; 26]) -> BitSuffix {
        let i = read_hex(&buf[0..12]);
        let bitcount = read_decimal(&buf[13..25]);
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
    let cutoff = 32;
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
