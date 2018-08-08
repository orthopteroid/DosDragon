# DosDragon
Elderly (2001) DOS Turbo C code for building and rendering a 2D line art animated dragon

This project combines animated model rendering with a model design tool, all for the
DOS VGA graphics mode - ca. early 1990's graphics technology. This project's intention was
to develop special purpose modelling and rendering codes for just a dragon - there
were no general purpose intentions here!

The dragon itself is
a hand-assembled display-list implemented in dragon.c, which reads geometry from
arrays #include-ed from the filesystem. The geometry files are built using the tool.c
utility, specifically built for the modelling components of the dragon.

The code uses some BIOS tricks borrowed from other codes to draw on vsync and
read mouse positions but most of the (retained-ish) rendering-mode code is
hand rolled (and may smell like a weekend project from the middle kingdom). Fast forward
18 years or so and it would unfortunately appear that such vsync tricks appear not
to work very well with dosbox running on a laptop...

![dragon movie](dragon.gif)

May you be inspired!
