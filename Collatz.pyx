
cdef extern from "mpir.h":
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

def CollatzGlideCounts(n):
    cdef int d = 0
    cdef int m = 0
    cdef char * __n
    s = str(n)
    __n = s
    cdef mpz_t t, _n
    mpz_init(_n)
    mpz_set_str(_n, __n, 10)
    mpz_init_set(t, _n)
    while mpz_cmp(t, _n)>0 or (d+m) < 1:
        if 1 == mpz_tstbit(t, 0):
            m = m + 1
            mpz_mul_ui(t, t, 3)
            mpz_add_ui(t, t, 1)
        else:
            d = d + 1
            mpz_fdiv_q_2exp(t, t, 1)
    mpz_clear(t)
    mpz_clear(_n)
    return (d+m,d,m)

def CollatzGlideString( n ):
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

def GlideLogLine(n):
    t,d,m = CollatzGlideCounts(n)

    if n == 0:
        return "%i,%i,%i,%i\n" % (d, n, 0, t)
    else:
        return "%i,%i,%i,%i\n" % (d, n, len(str(n)), t)

def GlideLogLines(l):
    return [GlideLogLine(n) for n in l]
