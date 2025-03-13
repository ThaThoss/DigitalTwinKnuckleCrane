For future issues with compiling using ixwebsocets. 


clone the repo to anywhere on the computer.
compile it using make from the top folder. 

Might have to install some standard libraries.


then use "sudo cp /path/to/libixwebsocket.a /usr/local/lib/" to move teh .a file to a global accessible place

use "sudo ldconfig" to update.

should be included in t make file like:


Even when placed in a global directory, you still need to link the library during compilation. Use the -l flag followed by the library name (without the lib prefix and .a extension) and the -L flag if it's not in a standard directory.

g++ -o my_program my_program.cpp -lixwebsocket -L/usr/local/lib




verify with:

ls /usr/local/lib/libixwebsocket.a



