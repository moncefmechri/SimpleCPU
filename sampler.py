#!/usr/bin/python2

import sys
from collections import Counter, defaultdict

import TraceReplayer


def align(value, alignment):
    return value & ~(alignment - 1)


class Metadata(object):
    def __init__(self, window_id, access_time):
        self.window_id = window_id
        self.access_time = access_time


class Sampler(object):
    def __init__(self, window_size, sampling_rate, cache_line_size=64):
        self.window_size = window_size
        self.sampling_rate = sampling_rate
        self.cache_line_size = cache_line_size
        self.watchpoint_set = set()
        self.watchpoint_metadata = {}
        self.time = 0
        self.window_id = 0
        self.time_until_next_watchpoint = 0 #first access is sampled
        self.time_until_next_window = window_size
        self.rdist_hists = defaultdict(Counter)
        self.nb_instructions_per_window = Counter()

    def __record_watchpoint(self, address):
        assert(address in self.watchpoint_set)
        assert(address in self.watchpoint_metadata)

        metadata = self.watchpoint_metadata[address]
        rdist = self.time - metadata.access_time - 1
        self.rdist_hists[metadata.window_id][rdist] += 1

        del self.watchpoint_metadata[address]
        self.watchpoint_set.remove(address)

    def __add_watchpoint(self, address, window_id, access_time):
        assert(address not in self.watchpoint_set)
        assert(address not in self.watchpoint_metadata)

        self.watchpoint_set.add(address)

        metadata = Metadata(window_id, access_time)
        self.watchpoint_metadata[address] = metadata

    def log_instructions(self, nb_instructions):
        self.nb_instructions_per_window[self.window_id] += nb_instructions

    def access(self, address, nb_instructions_since_last_access=0):
        aligned_address = align(address, self.cache_line_size)
        if aligned_address in self.watchpoint_set:
            self.__record_watchpoint(aligned_address)

        if self.time_until_next_watchpoint == 0:
            self.__add_watchpoint(aligned_address, self.window_id, self.time)
            self.time_until_next_watchpoint = self.sampling_rate
        else:
            self.time_until_next_watchpoint -= 1


        if self.time_until_next_window == 0:
            self.window_id += 1
            self.time_until_next_window = self.window_size
        else:
            self.time_until_next_window -= 1

        self.time += 1

    def finalize(self, clear=True):
        for address in self.watchpoint_set:
            metadata = self.watchpoint_metadata[address]
            rdist = sys.maxint
            self.rdist_hists[metadata.window_id][rdist] += 1

        if clear:
            self.watchpoint_set.clear()
            self.watchpoint_metadata.clear()

        self.rdist_hists.default_factory = None


def sample_trace_file(filename, window_size, sampling_rate):
    replayer = TraceReplayer.TraceReplayer(filename)

    s = Sampler(window_size, sampling_rate)
    for nb_instructions, accesses in replayer:
        s.log_instructions(nb_instructions)
        if accesses: #to handle the last record
            for access in accesses:
                s.access(access)

    s.finalize()

    return s.rdist_hists, s.nb_instructions_per_window
