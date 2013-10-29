Simple Graphic Library
=======

Simple Graphic Library (SGL) is a lightweight CPU based alternative to OPENGL written from the scratch with a knowledge of OPENGL 3 problems.

SGL is written both in C and Java independently.

The C solution is partly dependent (for real time rendering) on GLUT main loop (only the transfer of buffer to GPU).
The Java solution is entirely independent and both buffer and main loop are part of the source code.


No future ports intended.


***

Running the shell script
========================

run:
----

build.sh -- *run testapp*

build.sh clean -- *clean & run testapp*

build.sh debug -- *run testapp in debug mode*

build.sh clean debug -- *clean & run testapp in debug mode*

using debug mode:
-----------------

**run** -- to run debug

**quit** -- to kill debug

**list** -- to print last executed code where exception happened

***
