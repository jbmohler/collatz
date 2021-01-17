extern crate itertools;
extern crate argparse;
use std::io::SeekFrom;
use std::io::prelude::*;
use std::fs::File;
use std::path::Path;
use argparse::{ArgumentParser, Store};

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
    exemplar: u64,
    bitcount: u32,
}

const EX_OFFSET: usize = 0;
const EX_WIDTH: usize = 10;
const BC_OFFSET: usize = EX_WIDTH + 1;
const BC_WIDTH: usize = 6;
const REC_WIDTH: usize = EX_WIDTH + BC_WIDTH + 2;

impl BitSuffix {
    fn into_bytes(&self) -> [u8; REC_WIDTH] {
        let mut readbuf = [32u8; REC_WIDTH];
        readbuf[REC_WIDTH - 1] = 10;
        format_hex(&mut readbuf[EX_OFFSET..(EX_OFFSET+EX_WIDTH)], self.exemplar);
        format_decimal(&mut readbuf[BC_OFFSET..(BC_OFFSET+BC_WIDTH)], self.bitcount);
        return readbuf;
    }

    fn from_bytes(buf: &[u8; REC_WIDTH]) -> BitSuffix {
        let i = read_hex(&buf[EX_OFFSET..(EX_OFFSET+EX_WIDTH)]);
        let bitcount = read_decimal(&buf[BC_OFFSET..(BC_OFFSET+BC_WIDTH)]);
        return BitSuffix { bitcount: bitcount, exemplar: i };
    }
}

fn run_collatz(ex: u64) -> BitSuffix {
    // only operates on odd positive integers
    assert!(ex > 0 && ex % 2 == 1);

    let cc_ex: u128 = ex.into();
    let mut cc = cc_ex;
    let mut bc = 0;

    if cc >= 1_u128 << 126 {
        panic!("pre-emptively catch potential overflow {} (value {})", ex, cc);
    }

    cc = cc * 3 + 1;

    while cc > cc_ex {
        if cc >= 1_u128 << 126 {
            panic!("pre-emptively catch potential overflow {} (value {})", ex, cc);
        }
        if cc % 2 == 1 {
            cc = cc * 3 + 1;
        } else {
            cc = cc / 2;
            bc += 1;
        }
    }

    return BitSuffix { exemplar: ex, bitcount: bc };
}

fn main() {
    let mut cutoff = 25;
    let mut filename = "collatz_exemplars.txt".to_string();
    {  // this block limits scope of borrows by ap.refer() method
        let mut ap = ArgumentParser::new();
        ap.set_description("Compute collatz glide bitcounts for 1..2^cutoff");
        ap.refer(&mut cutoff)
            .add_option(&["-c", "--cutoff"], Store,
            "cutoff");
        ap.refer(&mut filename)
            .add_option(&["--file"], Store,
            "");
        ap.parse_args_or_exit();
    }


    let mut bitcount = 1;
    //let mut sigs : Vec<BitSuffix> = Vec::new();
    let path = Path::new(&filename);
    let display = path.display();

    let mut outfile = match File::create(&path) {
        Err(why) => panic!("couldn't create {}: {}",
                           display,
                           why),
        Ok(file) => file,
    };

    let mut infile = match File::open(&path) {
        Err(why) => panic!("couldn't open for reading {}: {}",
                           display,
                           why),
        Ok(file) => file,
    };

    fn compute_and_push(ex : u64, ofile : &mut File) {
        let xx = run_collatz(ex);
        ofile.write_all(&xx.into_bytes()).unwrap();
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
            let mut readbuf = [0u8; REC_WIDTH];
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
