
cdef extern from "mpir.h" nogil:
    ctypedef void* mpz_t
    ctypedef unsigned int mpir_ui
    ctypedef unsigned int mp_bitcnt_t

    void mpz_init(mpz_t)
    void mpz_init_set(mpz_t, mpz_t)
    void mpz_clear(mpz_t)
    void mpz_set_str(mpz_t, char*, int)
    void mpz_mul_ui(mpz_t, mpz_t, mpir_ui)
    void mpz_add_ui(mpz_t, mpz_t, mpir_ui)
    void mpz_fdiv_q_2exp(mpz_t, mpz_t, mpir_ui)
    int mpz_cmp(mpz_t, mpz_t)
    int mpz_tstbit(mpz_t, mp_bitcnt_t)

cdef int _GlideDivides(mpz_t n) nogil:
    cdef int d = 0
    cdef int m = 0
    cdef mpz_t temp
    mpz_init_set(temp, n)
    while mpz_cmp(temp, n)>0 or (d+m) < 1:
        if 1 == mpz_tstbit(temp, 0):
            m = m + 1
            mpz_mul_ui(temp, temp, 3)
            mpz_add_ui(temp, temp, 1)
        else:
            d = d + 1
            mpz_fdiv_q_2exp(temp, temp, 1)
    mpz_clear(temp)
    return d

def GlideDivides(n):
    cdef char * __n
    cdef int d
    cdef mpz_t _n
    s = str(n)
    __n = s
    with nogil:
        mpz_init(_n)
        mpz_set_str(_n, __n, 10)
        d = _GlideDivides(_n)
        mpz_clear(_n)
    return d

def GlideDividesList(nlist):
    return [(GlideDivides(n), n) for n in nlist]

def GlideString( n ):
    cnt = 0
    string = ""
    t = n
    while t > n or cnt < 1:
        if t % 2:
            t = 3 * t + 1
            string += "1"
        else:
            t = t / 2
            string += "0"
        cnt = cnt + 1
    return string

def Glide(n, compress=False):
    cnt = 0
    t = n
    while t > n or cnt < 1:
        yield t
        if t % 2:
            t = 3 * t + 1
            if compress:
                t //= 2
        else:
            t //= 2
        cnt = cnt + 1
    yield t

def GlideLogLine(n):
    d = GlideDivides(n)
    t = 0 # nada

    if n == 0:
        return "%i,%i,%i,%i\n" % (d, n, 0, t)
    else:
        return "%i,%i,%i,%i\n" % (d, n, len(str(n)), t)

def GlideLogLines(l):
    return [GlideLogLine(n) for n in l]
