def get_bits(word, number, offset):
    mask = (1 << (number + offset)) - 1
    print("Mask:", format(mask, 'x'))
    offmask = (1 << offset) - 1
    print("Offmask:", format(offmask, 'x'))
    return (word & mask & ~offmask) >> offset
