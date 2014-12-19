/*
 * Description:
 * This file contains common doxygen documentation, it is DEFINITELY NOT
 * intended to be involved in any source code build.
 */

/**
 * @defgroup Util Utilities
 * Utility libraries which are mend to be used across the entire system of the
 * development environment. These libraries should not be dependent on any other
 * component, and they should be versatile.
 *
 * @defgroup MCUDataFiles MCU data files
 * @ingroup Util
 * Library for working with Intel Hex files, Motorola® S-Record files, and
 * binary files.
 *
 * @defgroup DbgFile Debug meta-data files
 * @ingroup Util
 * Library for working with:
 *   - SDCC's .cdb files (http://sourceforge.net/apps/trac/sdcc/wiki/CDB%20File%20Format)
 *   - \<in future others\>
 *
 * @defgroup LicenseCertificate License certificates
 * @ingroup Util
 * Library for validating license certificates.
 *
 * @defgroup AdjSimProcDef Processor definition
 * @ingroup Util
 * Library for working with processor definition files used Adjustable Simulator.
 */

/**
 * @defgroup GUI Graphical User Interface
 * This group contains components of the graphical user interface.
 *
 * @defgroup SimWidgets Simulator sidgets
 * @ingroup GUI
 * Widgets related to the MCU simulator.
 */

/**
 * @defgroup Sim Simulator engine
 * Modules of the simulator engine.
 *
 * @defgroup Sim Simulator control
 * @ingroup SimControl
 * Simulator control unit.
 *
 * @defgroup MCUSim MCU simulators
 * @ingroup Sim
 * Simulators of micro-controller units.
 *
 * @defgroup AVR8 8-bit AVR simulator
 * @ingroup MCUSim
 * Simulator of microcontrollers based on 8-bit AVR processor.
 *
 * @defgroup PIC8 8-bit PIC simulator
 * @ingroup MCUSim
 * Simulator of microcontrollers based on 8-bit PIC processor.
 *
 * @defgroup PicoBlaze PicoBlaze soft-processors simulator
 * @ingroup MCUSim
 * Simulator of PicoBlaze soft-processors.
 *
 * @defgroup AdaptableSim Adaptable Simulator
 * @ingroup MCUSim
 * Simulator for user defined processors.
 *
 * @defgroup McuSimCfgMgr MCU simulator configuration manager
 * @ingroup MCUSim
 * MCU simulator configuration management.
 */

/**
 * @defgroup Compiler Built-in compiler collection
 *
 * @defgroup CompilerC C compiler
 * @ingroup Compiler
 * This group contains components of the C compiler.
 *
 * @defgroup Assembler Assembler
 * @ingroup Compiler
 * This group contains components of the built-in assemblers.
 *
 * @defgroup Avr8Asm 8-bit AVR assembler
 * @ingroup Assembler
 * This group contains components of the 8-bit AVR assembler.
 *
 * @defgroup Pic8Asm 8-bit PIC assembler
 * @ingroup Assembler
 * This group contains components of the 8-bit PIC assemble.
 *
 * @defgroup Mcs51Asm MCS-51 assembler
 * @ingroup Assembler
 * This group contains components of the MCS-51 assemble.
 *
 * @defgroup PicoBlazeAsm PicoBlaze assembler
 * @ingroup Assembler
 * This group contains components of the PicoBlaze assembler.
 */

/**
 * @defgroup tests Automated tests
 * This group contains automated unit test for various components of the MDS.
 *
 * @defgroup AutoTest libAutoTest
 * @ingroup tests
 * Library for automated testing environment using libCUnit.
 *
 * @defgroup testsCompiler Compiler
 * @ingroup tests
 * This group contains components of the MDS compiler.
 *
 * @defgroup testsCompilerC C Compiler
 * @ingroup testsCompiler
 *
 * @defgroup testsAssembler Assembler
 * @ingroup testsCompiler
 *
 * @defgroup testsAssemblerPicoBlaze PicoBlaze assembler
 * @ingroup testsAssembler
 *
 * @defgroup testsAssemblerAdaptable Adaptable assembler
 * @ingroup testsAssembler
 *
 * @defgroup testsMCUDataFiles MCUDataFiles
 * @ingroup tests
 *
 * @defgroup testsSimulator Simulator
 * @ingroup tests
 *
 * @defgroup testsMScript MScript
 * @ingroup tests
 *
 * @defgroup testsMCUSim MCU Simulator
 * @ingroup testsSimulator
 *
 * @defgroup testsMCUSimPicoBlaze PicoBlaze Simulator
 * @ingroup testsMCUSim
 *
 * @defgroup testsMCUSimAdaptable Adaptable Simulator
 * @ingroup testsMCUSim
 *
 * @defgroup testsDisasm Disassembler
 * @ingroup tests
 *
 * @defgroup testsDisasmPicoBlaze PicoBlaze Disassembler
 * @ingroup testsDisasm
 *
 * @defgroup testsDisasmAdaptable Adaptable Disassembler
 * @ingroup testsDisasm
 *
 * @defgroup testsAsmTranslator AsmTranslator
 * @ingroup tests
 */

/**
 * @defgroup Disassembler Disassembler
 *
 * @defgroup DisasmPicoBlaze PicoBlaze disassembler
 * @ingroup Disassembler
 */

/**
 * @defgroup AsmTranslator Asm. support library
 * Assembly language support library, this tool translates between various
 * dialects of a particular Assembly language,
 */

/**
 * @defgroup MoraviaScript Moravia Script
 * Moravia Script language implementation: lexical analyzer, syntax analyzer, etc.
 */

/**
 * @defgroup oscomp OS Compatibility.
 * Operating system compatibility library.
 */

/**
 * @mainpage Introduction
 *
 * @section HowTo How to use this documentation
 * Go to the <a href="./modules.html">modules section</a>, the documentation is
 * divided into separate groups there.
 *
 * @section Build Build
 *   - cmake . (on Windows: 'cmake -G "MSYS Makefiles" .')
 *   - make          # build executables and libraries
 *   - make doc      # build documentation
 *   - make test     # execute automated tests
 *   - make package  # build instalation packages
 *
 * @section Testing Testing
 * (This is not implemented yet...)
 *
 * @section Intro A word about the project
 *
 * @subsection TheInnovation The idea of innovation
 *   - Advanced simulation capabilities including simulation of devices
 *     peripheral to the microcontroller.
 *   - Multiplatform development tools allowing thousands, or tens of thousands,
 *     of engineers, scientist, and students, all around the world to use this
 *     kind of software and hardware tools also on GNU/Linux and
 *     Mac&nbsp;OS&nbsp;X operating systems.
 *   - <a href="http://en.wikipedia.org/wiki/Unified_Modeling_Language">UML</a>
 *     tools for microcontrollers, the most notable one is automated generation
 *     of finite state machines for microcontrollers. This probably poses a
 *     valuable potential in the field on industrial automation.
 *   - Extensibility, even by user (opened API for user defined simulation
 *     plug-ins).
 *   - High simulation speed on host machines capable of multiprocessing.
 *   - The project is supposed to bring some of the tools common to more a
 *     complex computers also to the world of microcontrolles, like code
 *     profilers, measurement of software metrics, real-time operating systems,
 *     etc., some of these are already available on the market but only some of
 *     them.
 *
 * @subsection BetterThanOthers What might make us better than the others
 *   - There are no professional grade IDEs capable of simulation of certain
 *     important peripherals, like LCD displays, various controllers, etc.,
 *     reliable and well written set of individual simulators for these devices
 *     could significantly ease the development process and thus make our
 *     products more attractive. These additional simulators would be sold as
 *     individual packages allowing customer to configure the IDE to fit his or
 *     her specific needs.
 *   - Most of the MCU simulators currently available are too slow, because they
 *     are mostly products of unexperienced developers.
 *   - There are almost no professional grade IDEs capable to run on other
 *     operating system than Microsoft®&nbsp;Windows®, our products would run
 *     Microsoft Windows, GNU/Linux, and Mac&nbsp;OS&nbsp;X, so there is a good
 *     chance that we could quickly cover this slowly emerging portion of the
 *     market.
 *   - Most of the development tools currently available are clumsy, there are
 *     not well written, slow, unstable, etc. That's caused by that the people
 *     writing them are usually ``a bit stupid'', or at least they lack the
 *     necessary experience and expertise
 */
