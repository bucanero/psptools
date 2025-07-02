PSP Registry Parser v2 by Skylark (with minimal help from FreePlay :-P)
--------------------------------------------------------------------

ABOUT
-----

These are command-line programs that you can use to view the contents of your SYSTEM.DREG/SYSTEM.IREG files, which are the PSP's system registry. These files contain all sorts of information about system settings, network connection setup, etc.

These new versions allow you to check if the data stored in the registry is valid, as well as force the registry to mark data as valid regardless of what it actually is.

The program's output was tested on a simulated 2.00 loaded in MPH Firmware Loader, then on an actual 1.50.

SETUP
-----

Installation is simple; just copy all of the files in this ZIP file to a folder somewhere on your computer. It doesn't matter where, so long as you copy all of them.

To set the programs up, you'll need to use something like FileAssistant to copy the two files from flash1:/REGISTRY into the same folder as this program. FileAssistant v0.4 works on any PSP up to v2.60, and you can get that from http://psp-news.dcemu.co.uk/fileasistant.shtml .

To copy flash1 using that program, press L until "Disk" is highlighted at the bottom, then press R, then pick Flash 1, then press O. Then Press L until "Copy" is highlighted, then press [] to highlight the SYSTEM.DREG and SYSTEM.IREG files, then press R to copy them to the MS.

USING
-----

There are three main ways of running the programs:

1. Display-only mode. Open up a command prompt, change over to the folder that has the programs in it, and just type "parsedreg2". It should spit out an XML-formatted file to the console.

2. File output mode. Performed exactly as #1, but you instead type "parsedreg2 > insert_a_filename_here.xml". It'll save the file to whatever name you put into 'insert_a_filename_here'.

3. Registry fixing mode. Thanks to Skylark, we've discovered that the PSP uses a modified SHA1 hash to mark data in the registry as valid. He wrote another program that generates such a hash for any data you want, thus making it "valid" data.
	To run this mode, type "fixupdreg2". That's it. It'll fix any invalid registry entries.

HELP!
-----

If you have any questions about this, I'm sure one of us will be online. Go to www.fanjita.org and click "Live Chat", or use your favorite IRC client to connect to irc.toc2rta.com , channel #fanjita. You can also e-mail FreePlay at FreePlayPSP@gmail.com . He's a bit addicted to the IRC channel :P

NOTES
-----

There's something in the "pspreghtmlizer" folder that can turn the XML output into a nicely-formatted web page. Check it out.

The "src" folder contains the source code, for those of you on who are on Linux or who just want to modify the program.