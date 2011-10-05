.. index:: 
   single: Plug-ins (C API); Plug-in Manager

===============
Plug-in Manager
===============

.. todo:: where is default plug-in path.

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

.. doxybridge:: s_pm_get_plugin_path
