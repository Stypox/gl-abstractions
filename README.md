# C++ abstractions for OpenGL

Wraps the OpenGL functions to make them more understandable and reduce lines of code.

This repository is archived because this turned out to be a bad idea:
- the amount of OpenGL code in a game is actually not that much, so if it is well separated from the rest it doesn't matter if there are a bit more lines of code
- many abstractions that do make sense for really simple games do not hold up for more complex situations: in those cases many more OpenGL function parameters need customizations
