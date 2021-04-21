*************************************************************
* Converted SOJ Saturn Program Libraries and Sample Code    *
* ver 1.10 BETA 11-11-94 reh (C) 1994 Sega of America, Inc. *
*                                                           *
* Disclaimer:                                               *
* These files are sample code. Sega of America, Inc.        *
* and the author(s) accept no responsibility                *
* for any problems which may arise from using               *
* this code.                                                *
*                                                           *
* These file may be used and/or modified                    *
* provided the disclaimer mentioned above is                *
* maintained, and the appropriate Non Disclosure            *
* Agreement with Sega of America, Inc. and the              *
* party or parties using/modifying the code exist.          *
* This file may not be distributed to any party/parties     *
* which do not have the appropriate Non Disclosure          *
* Agreement with Sega of America, Inc.                      *
*************************************************************

* Files used for conversion and testing are from
  the SOJ Saturn Software Program Libraries ver 1.1. Disk version 94/11/11

* CDC replacement is by Cyber Warrior X

* Libraries and sample code tested on Production systems w/VCD
* and Saturn CartDev Rev. A and B


* This library is BETA


--------------
Directory Tree
--------------
SEGALIB - In the LIB directory:
	  .A - Converted GNU archives; library files

SEGASMP - Sample code that uses the libraries.
          \DBG simple debugger sample code tested
	  \BUP backup library sample code tested
	  \SPR VDP1 sprite based sample code tested
          \SCL VDP2 scrolling sample code tested.

SEGAGAM1 - Sample game code tested. This is a cd-based test that uses 
	   virtually every system on the SATURN. It is fully tested. VCD 
	   scripts are included for your enjoyment.
          

--------------
Helpful Notes
--------------
1) lib\libsat.a are all the libraries in one archive file.

2) the libraries were built with gcc version 10.2

3) Refer to the following documents to learn how to use
   the library routines:

   - Program Libary User's Guide 1 / CD Library     		ST-136
   - Program Libary User's Guide 2 / Graphics Library     	ST-157
   - Program Libary User's Guide 3      			ST-135

--------
Caveats
--------
1) This is a beta release. 
   While most of the code samples have been thoroughly tested, please report 
any and all bugs


--------
To-Dos
--------
1) Fix compiler warnings
2) Fix peripheral library bugs
3) Translate comments to English
