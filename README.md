= Slaughter: A reliable process terminator

== Introduction

Slaughter is a small tool that tries its best to terminate a process reliably.
It first sends SIGTERM, then an array of other signals, until the process
finally terminates. It waits a configured period of time between signalling.

== Building

When checking out from git, you first need to run

    autoreconf --install
    automake

Then, simply issue

    ./configure
    make
    make install

to install the binary, `sl`.
