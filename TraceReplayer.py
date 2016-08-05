#!/usr/bin/python2

import struct


def read_n_uint64(f, n):
    if n < 1:
        raise RuntimeError("n must be greater or equal than 1")

    nb_bytes_to_read = n * 8
    bytes = f.read(nb_bytes_to_read)

    if not bytes:
        return None
    elif len(bytes) != nb_bytes_to_read:
        raise IOError("I/O error. Read {} bytes instead of {}".format(len(bytes), nb_bytes_to_read))

    format = "Q" * n

    return struct.unpack(format, bytes)

def read_record(f):
    ret = read_n_uint64(f, 2)
    if not ret:
        return None

    nb_instructions, nb_accesses = ret

    if nb_accesses > 0:
        accesses = read_n_uint64(f, nb_accesses)
        if not accesses:
            raise IOError("Unexpected EOF")
    else:
        accesses = None

    return nb_instructions, accesses

class TraceReplayer(object):
    def __init__(self, filename):
        self.f = open(filename, 'rb')

    def __iter__(self):
        record = read_record(self.f)

        while record:
            yield record
            record = read_record(self.f)
