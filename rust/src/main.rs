
struct BitSuffix {
    bitcount: i32,
    exemplar: i64,
    sigstr: Vec<u8>,
}

fn run_collatz(ex: i64) -> (i32, Vec<u8>) {
    // only operates on odd positive integers
    assert!(ex > 0 and ex % 2 == 1);

    let mut cc = ex;
    let mut sig = Vec::new();
    let mut bc = 0;

    sig.push(1);
    cc = cc * 3 + 1;

    while cc > ex {
        if cc % 2 == 1 {
            sig.push(1);
            cc = cc * 3 + 1;
        } else {
            sig.push(0);
            cc = cc / 2;
            bc += 1;
        }
    }

    return (bc, sig);
}

fn main() {
    let mut bitcount = 1;
    let mut sigs : Vec<BitSuffix> = Vec::new();

    fn compute_and_push(ex : i64, newsigs : &mut Vec<BitSuffix>) {
        let results = run_collatz(ex);
        let xx = BitSuffix {bitcount: results.0, exemplar: ex, sigstr: results.1};
        println!("{:0}: {:1} -- {:?}", xx.exemplar, xx.bitcount, xx.sigstr);
        newsigs.push(xx);
    };

    loop {
        println!("Check new bit patterns of lenth {}", bitcount);

        let mut newsigs : Vec<BitSuffix> = Vec::new();
        let base = 1 << (bitcount - 1);

        if bitcount == 1 {
            // seed this
            compute_and_push(1, &mut newsigs);
        } else {
            for sig in sigs.iter() {
                if sig.bitcount >= bitcount {
                    compute_and_push(base+sig.exemplar, &mut newsigs);
                }
            }
        }

        sigs.extend(newsigs);

        bitcount += 1;

        if bitcount == 18 {
            break;
        }
    }

    println!("Hello, world!");
}
