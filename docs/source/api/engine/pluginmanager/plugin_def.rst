.. index:: 
   single: Plug-ins (C API); Plug-in Definition

==================
Plug-in Definition
==================

Definitions of plug-in functions and symbols. A plug-in **must** define at least one
externally visible symbol, named ``s_plugin_init``. This symbol must be a function
of the :c:type:`s_plugin_init_fp` type. The :doc:`plugin_manager` will look up this symbol
and call this function after the plug-in has loaded. From this function the Plug-in Manager
will get the :c:type:`s_plugin_params` defined by the plug-in. When the plug-in
is unloaded, the Plug-in Manager will call the :c:type:`s_plugin_exit_fp` to free up any
resources used by the plug-in.


Definitions
===========

.. doxybridge:: s_plugin_init_fp
   :type: typedef function pointer

.. doxybridge:: s_plugin_reg_fp
   :type: typedef function pointer

.. doxybridge:: s_plugin_exit_fp
   :type: typedef function pointer


Plug-in parameters
==================

.. doxybridge:: s_plugin_params
