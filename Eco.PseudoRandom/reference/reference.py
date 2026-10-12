"""Independent reference model of the PRNG contract (see IEcoPseudoRandomService1.h)."""

DEFAULT_MASK = {32: 0x80200003, 64: 0xD800000000000000}


def width_mask(n):
    return (1 << n) - 1


def resolve(mask, bits):
    """Return (mask, register width n) for the given param and call width."""
    if mask == 0:
        mask = DEFAULT_MASK[bits]
    return mask, mask.bit_length()


def fib_step(state, mask, bits):
    mask, n = resolve(mask, bits)
    s = state & width_mask(n)
    if s == 0:
        raise ValueError("zero state")
    fb = bin(s & mask).count("1") & 1
    return ((s << 1) | fb) & width_mask(n)


def galois_step(state, mask, bits):
    mask, n = resolve(mask, bits)
    s = state & width_mask(n)
    if s == 0:
        raise ValueError("zero state")
    lsb = s & 1
    s >>= 1
    if lsb:
        s ^= mask
    return s


def xorshift_step(state, mask, bits):
    s = state & width_mask(bits)
    if s == 0:
        raise ValueError("zero state")
    m = width_mask(bits)
    if bits == 32:
        s ^= (s << 13) & m
        s ^= s >> 17
        s ^= (s << 5) & m
    else:
        s ^= (s << 13) & m
        s ^= s >> 7
        s ^= (s << 17) & m
    return s


ALGOS = {"fibonacci": fib_step, "galois": galois_step, "xorshift": xorshift_step}


def sequence(algo, seed, mask, bits, count):
    step, out, s = ALGOS[algo], [], seed
    for _ in range(count):
        s = step(s, mask, bits)
        out.append(s)
    return out


def period(algo, seed, mask, bits):
    step, s, n = ALGOS[algo], seed, 0
    while True:
        s = step(s, mask, bits)
        n += 1
        if s == seed:
            return n


if __name__ == "__main__":
    # xorshift sanity against Marsaglia's published first values for seed 1
    assert sequence("xorshift", 1, 0, 32, 3) == [270369, 67634689, 2647435461]
    for algo in ("fibonacci", "galois"):
        for mask in (0xB400, 0xD008, 0x80200003 & 0xFFFF):
            try:
                print(algo, hex(mask), "period", period(algo, 1, mask, 32))
            except ValueError as e:
                print(algo, hex(mask), e)
