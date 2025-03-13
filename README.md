## Quicksand: Chess engine written in C++

Initially C was used to write the engine but the language has been changed to C++ in favor of lambda expressions. The engine is also UCI compilant although the implementation is very scarce.

## Compiling and Running

On Linux/BSD:

    make && ./quicksand

On Windows (using MinGW):

    make windows && quicksand.exe

The engine does not provide a GUI interface, you can challenge the bot on [this](lichess.org/@/testbot24) Lichess account. You could also use your choice of chess GUI that is compatible with the UCI protocol.
