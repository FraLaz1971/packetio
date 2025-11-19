# CCSDS packets I/O
To compile on linux
###
`make`
###
To run on linux
###
`./writepackets`
###
And follow the instructions.
The run will create a file named `packets.ccsds` containing the generated packets
###
To run on linux
###
`./readpackets`
###
This will read the packets contained in the file `packets.ccsds`. At the end it gives
the total number of packets read.
###
To compile on MS windows using MSVC from the cmd command prompt
###
`nmake /f makefile.vc`
###
Or is possible to open the program in codeblocks IDE
###
To run on MS windows on CMD command prompt
###
`writepackets`
###
and follow the instructions, the same for
###
`readpackets`
###
and so on for all the programs. On MS Windows it's possible to open a menu to select the program to run
digiting
###
`menu`
###
from the command line cmd or on codeblocks from the tools item called menu.
###
the following line extract headers from science packets and save it on a log file temp.log
###
`grep -A 7 -B 23 "s.type: 21" bc-boa-flazzaro-20251016T082447.log > temp.log`
###

