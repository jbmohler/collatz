import collatz

def test_basics():
    assert collatz.GlideDivides(5) == 2
    assert collatz.GlideDivides(27) == 59
    assert collatz.GlideDivides(11) == 5

def test_list():
    assert len(collatz.GlideDividesList([3, 5, 27])) == 3

    for d, n in collatz.GlideDividesList([5, 9, 13]):
        assert d == 2

    l = collatz.GlideDividesList([n*2**8+39 for n in range(40)])
    assert set((8,)) == set([d for d, n in l])
