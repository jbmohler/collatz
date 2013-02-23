import collatz

def test_possibles():
    assert list(collatz.possibleglides(2)) == ['0', '100']
    assert len(list(collatz.possibleglides(5))) == 5
    assert len(list(collatz.possibleglides(6))) == 5
    assert len(list(collatz.possibleglides(7))) == 8

def test_enums1():
    assert len(collatz.ofdivides(5)) == 2
    assert len(collatz.ofdivides(7)) == 3  # look at difference of 6-7 in test_possibles
