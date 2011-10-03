.. _voicemanager/manager:


.. index::
   single: Voices (C API); Voice Manager

=============
Voice Manager
=============

The *Voice Manager* controls the loading/unloading of a voice and it's
related resources and then provides the voice to the rest of the
Speect Engine. The loaded voice's data is chached for reuse. All of
this is controlled through one function:


.. doxybridge:: s_vm_load_voice
