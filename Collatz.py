

def CollatzGlideCounts( n ):
    d = 0
    m = 0
    t = n
    while t > n or (d+m) < 1:
        if t % 2:
            m = m + 1
            t = 3 * t + 1
        else:
            d = d + 1
            t = t / 2
    return [d+m,d,m]

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
