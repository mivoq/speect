.. index:: Building and Installing Speect

.. _build_install:

==============================
Building and Installing Speect
==============================

This document describes how to install Speect from
source in a new location and customize that installation.

    #. `Prerequisites`_
    #. `Optional`_
    #. `Configuration and Compilation`_
    #. `Configuration Options in CMake`_
    #. `Installation`_


Prerequisites
=============

In order to compile Speect you will need the following:
    * an ANSI C/ISO C90 compliant compiler,
    * CMake (http://www.cmake.org), at least version 2.8


Optional
========

To build the Python bindings you will need: 
    * SWIG (http://www.swig.org), at least version 1.3.40,
    * Python (http://www.python.org), at least version 2.6.

To include the HMM-based synthesis plug-ins for the HTS Engine: 
    * HTS Engine (http://hts-engine.sourceforge.net), version dependent 
      on the voices being used.


.. _configuration_build:

Configuration and Compilation
=============================

CMake must be run to generate a build system for Speect. CMake
provides both a command-line tool and interactive interfaces. Advanced
users may wish to use the command-line tool but here we document the
CMake interactive interface.

With Speect the build system is placed in a separate tree (known as an
*out-of-source* build) to the source tree. It is preferable to do an
out-of-source build so that your source tree remains unchanged. An
in-source build puts generated files in your source tree, and can
sometimes lead to unwanted behaviour if you forget to clean the source
tree after you have made changes to the code.  With an out-of-source
build you can just delete the whole tree. Also an out-of-source build
makes it easy to have multiple builds with different configurations
sharing the same source tree. Once a single in-source build has been
created it is the only build tree that can be associated with that
source tree. The default directory to place different builds is
``speect/build``.

CMake can be run from the command line on these platforms, or you can
use the CMake graphical interface (``cmake-gui``) if you have it
installed. The current working directory should be set to the desired
binary tree location in which the build system should be
generated. CMake will usually choose the system C compiler
automatically but it can be told to use specific compilers through the
``CC`` environment variable.

A typical build for Speect looks like this::

    $ ls -d speect
    speect/
    $ cd speect/build
    $ ccmake ../
    $ make

The ``ccmake`` tool is a curses-based dialog that may be used to
interactively configure Speect.  When it appears press ``c`` on the
keyboard to run the initial configuration of the Speect build system.
Eventually a set of configuration options will appear. These may be
edited using the arrow-keys and the ``ENTER`` key for navigation. See
below (`Configuration Options in CMake`_) for details on the meaning
of each of these options.

Once the options have been set as desired press ``c`` again to
reconfigure. New options may appear when earlier options are
adjusted. Keep adjusting options and pressing ``c`` until the desired
configuration is reached. Finally press ``g`` to actually generate the
build system. Now that the build system has been generated just run
``make`` or ``gmake`` to build Speect.

Alternatively one can pass CMake all the desired options through the
command line, for example::

    $ ls -d speect
    speect/
    $ cd speect/build
    $ cmake ../ -DERROR_HANDLING=off -DHTS_ENGINE_INCLUDE_104=/path/to/hts/include \
     -DHTS_ENGINE_LIB_104=/path/to/hts/lib/libHTSEngine.a
    $ make	      


.. note::

   The ``ccmake`` curses dialog is the most commonly used interactive
   interface for CMake on UNIX-like platforms, so these instructions
   assume it is available. Some system administrators may not have
   installed curses in which case ``ccmake`` will not be available. On
   these platforms one may use the command ``cmake -i`` in place of
   ccmake and follow the on-screen instructions to configure
   Speect. See the CMake documentation for further details.


Configuration Options in CMake
==============================

Several configuration options are available to customize the Speect
build system.  These options are configured through an interactive
CMake interface as described above.  Note that not all options are
available on all platforms, and some options are available only when
other options are set to a particular value.

The interactive CMake interface provides brief documentation for every
option. Some options have more meaning than can be described in one
sentence, so additional documentation is provided :ref:`here <cmake_options>`.


Installation
============

.. _python_path_installation:

Speect can be used from the build tree or it can be installed.  When
using the Speect Python bindings from the build tree one needs to tell
Python where to find Speect. This is done by adding the
``speect/build/swig/python`` path to the :envvar:`PYTHONPATH`
environment variable.

Installing Speect from a source distribution requires first that it be
compiled in a build tree. See the `Configuration and Compilation`_
section above for details. Once Speect has been compiled in a build
tree one may build the install target to actually put Speect in an
installation tree. If Speect was built using a CMake Makefile
generator then this is done by running ``make install`` from the top
of the build tree. The installation process will install all files in
a directory structure rooted at the directory specified by
:cmake:`CMAKE_INSTALL_PREFIX`.

.. warning::
   
   The ``make install`` command has an option to specify a destination
   directory (``DESTDIR`` = "...") on the command line or through an
   environment variable. If ``DESTDIR`` is specified then Speect needs
   the environment variable :envvar:`SPCT_PLUGIN_PATH` set to the
   ``DESTDIR``/:cmake:`CMAKE_INSTALL_PREFIX`/speect/plugins directory
   to be able to find the plug-ins (see :ref:`default plug-in path
   <default_plugin_path>`).

.. note::
   
   The Python wrappers are currently not installed.
