
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
    let mut sigs = Vec::new();

    let mut compute_and_push = |ex| {
        let results = run_collatz(ex);
        let xx = BitSuffix {bitcount: results.0, exemplar: ex, sigstr: results.1};
        println!("{:0}: {:1} -- {:?}", xx.exemplar, xx.bitcount, xx.sigstr);
        sigs.push(xx);
    };

    loop {
        println!("Check new bit patterns of lenth {}", bitcount);

        let base = 1 << bitcount;

        if bitcount == 1 {
            // seed this
            compute_and_push(1);
        } else {
            let myslice = sigs[0 .. sigs.len()-1];
            for sig in myslice.into_iter() {
                if sig.bitcount > bitcount {
                    compute_and_push(base+sig.exemplar);
                }
            }
        }

        bitcount += 1;

        if bitcount == 6 {
            break;
        }
    }

    println!("Hello, world!");
}
