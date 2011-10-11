.. index:: 
   single: Plug-ins (C API); Plug-in Manager

.. _plugin_manager_c:

===============
Plug-in Manager
===============

The *Plug-in Manager* provides methods for loading plug-ins that are
then available to the Speect Engine and all other loaded plug-ins.
Loaded plug-ins are chached for reuse.


Summary
=======

.. doxybridge-autosummary::
   :nosignatures:
   
   s_pm_load_plugin
   s_pm_get_plugin_path
   

Functions
=========

.. doxybridge:: s_pm_load_plugin

.. seealso::
    
   :ref:`Default plug-in path <default_plugin_path>`.

.. doxybridge:: s_pm_get_plugin_path

.. seealso::
    
   :ref:`Default plug-in path <default_plugin_path>`.
