.. _installation:

============
Installation
============
This section describes how to install Speect from
source in a new location and customize that installation. 

Prerequisites
=============
In order to compile Speect you will need the following:
    * Linux,
    * an ANSI C/ISO C90 compliant compiler,
    * `CMake <http://www.cmake.org>`_, at least version 2.6
    * `GNU make <http:://www.gnu.org/software/make>`_, any recent version should work.


To build the Python bindings you will need: 
    * `SWIG <http://www.swig.org>`_, at least version 1.3.36,
    * `Python <http://www.python.org>`_, at least version 2.6.


Building the C API documentation, you will need: 
    * `Doxygen <http://www.doxygen.org>`_, at least version 1.6,

and the Python interface documentation:
    * `Sphinx <http://sphinx.pocoo.org>`_, at least version 1.0.

To include the HMM-based synthesis plug-ins for the HTS Engine 
    * `HTS Engine <http://hts-engine.sourceforge.net/>`_, at least version 1.02


If you have downloaded Speect from the
`Sourceforge project page <http://sourceforge.net/projects/speect>`_ untar it to a
location where you want to install it::

    tar -xzf speect-0.9.5.tar.gz


Configuration and Compilation
=============================

Currently the Speect Engine and Plug-ins are configured and built separately. This should
change in the next release. Also note that this release does not support configuration 
of local installation paths, Speect should be built and used "in-place" without
doing a:: 

    make install.


Speect Engine
-------------

Go to the engine directory (this applies to the git clone as well)::

    cd speect/engine

run CMake to configure the system, the standard configuration should work.::

    cmake .

or, if you do not want to build the Python wrappers, run CMake with the following flag::

    cmake -DSPEECT_WRAP_PYTHON=off

.. note::
   The Python interface module is located at :file:`speect/swig/python/speect/`

.. seealso::

    You can also do an interactive CMake (:command:`cmake -i`), 
    see :file:`speect/engine/INSTALL`.

and finally run the GNU make utility::

    make


Speect Plug-ins
---------------

Currently the Speect plug-ins require the setup of environment variables before doing a
configuration.

Set the :envvar:`SPEECT_ENGINE` enviroment variable to the full path of your 
Speect Engine directory (this is the root path to the Speect engine *source*
e.g. :file:`speect/engine` where CMake configuration files from the engine
configuration can be found).

for example in bash do::

    export SPEECT_ENGINE="/path/to/speect/engine"

Set the :envvar:`SPEECT_PLUGINS` enviroment variable to the full path of your 
plug-in directory.

for example in bash do::

    export SPEECT_PLUGINS="/path/to/speect/plugins"

.. seealso::
    :file:`speect/plugins/INSTALL`.

Go to the plug-ins directory::

    cd speect/plugins

run CMake to configure the system, the standard configuration should work::

    cmake .

or, if you want to build the HTS Engine plug-ins you need to add the paths
to the HTS Engine that you have installed somewhere::

    cmake -DHTS_ENGINE_INCLUDE=/path/to/hts/include -DHTS_ENGINE_LIB=/path/to/hts/lib/libHTSEngine.a

.. note::
    The format of the ``pdf`` files created have changed in the latest version of the HTS Engine,
    therefore a voice built using the tools of version 1.02 will not work using version 1.03.

and finally run the GNU make utility::

    make


Voicetools
----------
Voicetools is a set of scripts that can be used to create new voices. 

In order to use these utilities, the following actions need to be
performed:

 1. Ensure that all the required software specified in the :file:`voictools/README` are
    installed and functional.

 2. Ensure that all the scripts in the :file:`voicetools/scripts/` directory are
    executable and on the path. This can be done by executing::
    
	chmod +x voictools/scripts/*

    and adding the scripts directory to the :envvar:`PATH` environment variable.

 3. Ensure that all the modules in the :file:`voicetools/modules/` directory are on
    the Python path. This can be done by adding the modules directory
    to the :envvar:`PYTHON_PATH` environment variable.


Documentation
=============

The Speect Engine API is documented in the source code header files in the doxygen format, 
and is built from the :file:`speect/engine` directory::

     cd speect/engine

     doxygen Doxyfile

Note that the output of doxygen will be located at :file:`speect/docs/_built/html/c_api/html`

The Speect Python modules API documentation is gathered from the Speect Python modules itself,
and therefore the Speect Python modules must be on your Python import path. This can be 
done as follows in bash::

    export SPEECT_PYTHON="/path/to/speect/swig/python"

    export PYTHONPATH="$SPEECT_PYTHON:$PYTHONPATH"

Now sphinx can be executed from the :file:`speect/docs/` path::

    cd speect/docs/

    make html

The documentation will be located at :file:`speect/docs/_built/html/`



