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
 */

/**
 * @defgroup GUI Graphical User Interface
 * This group contains components of the graphical user interface.
 */

/**
 * @defgroup Sim Simulator engine
 * Modules of the simulator engine.
 *
 * @defgroup MCUSim MCU simulators
 * @ingroup Sim
 * Simulators of micro-controller units.
 *
 * @defgroup AVR8 8-bit AVR simulator
 * @ingroup MCUSim
 * Simulator of microcontrollers based in 8-bit AVR processor.
 *
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