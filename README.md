# MDS-picoblaze-AVR-ide
Picoblaze IDE - simulator, compiler, dissassembler and more

MDS provides all the necessary functionality to develop software part of a PicoBlaze application, including source code editor,
assembler, disassembler, and simulator. Besides that there is also a number of tools and functions to make your work easier,
the sole purpose of MDS is to save your time and enable development of more complex applications.

![picoblz](https://user-images.githubusercontent.com/19152258/42370176-583eccc0-810c-11e8-92e8-cecbc2bfd626.jpg)

## Main features

  -  Text editor optimized for writing source code.
  -  Project manager for creating and maintaining your projects.
  -  Very fast simulator for all available versions of Picoblaze.
  -  Macro-assembler supporting wide range of output file formats, including MEM, HEX, and VHDL.
  -  PicoBlaze disassembler capable of reading from HEX, VHDL, MEM, and other file formats.
  -  Tool called Assembler Translator for compatibility with your current tools.
  -  Command line tools, and a number of graphical utilities.
  
## Assembler  
MDS macro-assembler for PicoBlaze is a two stage fast macro-assembler inspired by Intel assemblers and the C language. It supports
wide range of output data formats and a number of advanced features like macro processing, conditions, loops, etc. it is
meant to give you means to write code with run-time efficiency typical for the assembly language while giving you some of the comfort
of a more high-level language like C. 

Quick user guide:
https://moravia-microsystems.net/mds/others/QuickUserGuideAssembler.pdf

For instance you may use: 
 - if ( S0 == S1 ) to easy write conditions instead of compares and conditional jumps
 - for ( S0, 0 .. 9 ) for loops
 - #if my_constant > 20 for conditional compilation
 - abc macro x, y, z for defining your own macros, and more. 

  MDS assembler is enhanced with these features in hope it will help you save your time and make your work a little bit easier.
  MDS assembler also has a number of features for smooth and transparent debugging and it is regularly subjected to extensive automated
  testing to ensure its very functionality and to provide high reliability. Also this assembler supports all PicoBlaze versions
  publicly available at the time of its release. For these reasons we are convinced that this is the most advanced assembler 
  for PicoBlaze currently available on the market.
  
## Tutorial and example project  
  The aim of the tutorial project is to provide an easy way to explore the IDE without reading long documents. The tutorial project
  can be opened from the [Main Menu] -> [Help] -> [Tutorial Project].
  
  Demonstration project should introduce a new user into the basics of usage of this IDE, this generally
  covers the most common functions like assembling the code, running simulator, and so on.
  
  
  ## Requirements
  
  This software is compiled for x86 processors in both 32b and 64b variants with SSE instruction set, that means you need a processor equipped with SEE feature (most processors manufactured after the year 2003 are). For GNU/Linux, MDS also requires GLIBC version >= 2.17.

Supported host operating systems are:

   - Microsoft Windows 7 (32b and 64b),
   - Microsoft Windows XP (32b),
   - Ubuntu 13.10 and higher (32b and 64b),
   - CentOS 7 (64b),
   - openSUSE 13.1 and higher (32b and 64b),
   - Fedora 20 and higher (32b and 64b).
   
  ## Precompiled versions:
  
 https://moravia-microsystems.net/
