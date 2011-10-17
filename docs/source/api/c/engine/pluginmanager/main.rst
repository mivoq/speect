.. index:: Plug-ins (C API)

.. _pluginmanager_main_c:

========
Plug-ins
========

The plug-in based system can be divided into three parts:

    * The domain-specific system,
    * a plug-in manager, and
    * the plug-ins.

The :ref:`domain-specific system <dso_c>` loads the :ref:`plug-ins
<plugin_def_c>` and creates :ref:`plug-in objects <plugin_object_c>` via
the :ref:`plug-in manager <plugin_manager_c>`. 


Also see the :ref:`plugins_topic` topic guide.

	
.. toctree::
   :maxdepth: 2

   plugin_manager
   plugin_def
   plugin_object
   dso
  
