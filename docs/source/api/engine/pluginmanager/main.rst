.. _pluginmanager/main:

.. index:: ! Plug-ins (C API)

========
Plug-ins
========

The plug-in based system can be divided into three parts:

    * The domain-specific system,
    * a plug-in manager, and
    * the plug-ins.

The :doc:`domain-specific system <dso>` loads the :doc:`plug-ins
<plugin_def>` and creates :doc:`plug-in objects <plugin_object>` via
the :doc:`plug-in manager <plugin_manager>`. 


Also see the :doc:`../../../../topics/plugins_topic` topic guide.

	
.. toctree::
   :maxdepth: 2

   plugin_manager
   plugin_def
   plugin_object
   dso
  
