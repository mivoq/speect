.. _voicemanager/main:

.. index:: ! Voices (C API)


======
Voices
======

Voices and voice-management system for Speect Engine.


.. index::
   single: Voices (C API); Voice Manager

Voice Manager
=============

The *VoiceManager* provides methods for loading voices that
are then available to the Speect Engine. The loaded voice's
data is chached for reuse.

.. doxybridge:: s_vm_load_voice


.. index::
   single: Voices (C API); SVoice

SVoice
======

Voice objects.

Summary
-------

.. doxybridge-autosummary::
   :nosignatures:

   SVoiceSynthUtt
   SVoiceReSynthUtt
   SVoiceGetName
   SVoiceGetDescription
   SVoiceGetGender
   SVoiceGetLanguage
   SVoiceGetLangCode
   SVoiceGetVersion
   SVoiceGetDataKeys
   SVoiceDataIsPresent
   SVoiceGetData
   SVoiceSetData
   SVoiceDelData
   SVoiceGetFeatureKeys
   SVoiceFeatureIsPresent
   SVoiceGetFeature
   SVoiceSetFeature
   SVoiceDelFeature
   SVoiceGetFeatProcKeys
   SVoiceFeatProcIsPresent
   SVoiceGetFeatProc
   SVoiceSetFeatProc
   SVoiceDelFeatProc
   SVoiceGetUttProcKeys
   SVoiceUttProcIsPresent
   SVoiceGetUttProc
   SVoiceSetUttProc
   SVoiceDelUttProc
   SVoiceGetUttTypesKeys	
   SVoiceUttTypeIsPresent
   SVoiceGetUttType
   SVoiceSetUttType
   SVoiceDelUttType


Definitions
-----------

.. toctree::
   :hidden:
   
   SVoice_detail


.. doxybridge:: SVoice
   :type: speect object
   :members: none
   :inheritance: SObject.SVoice

.. doxybridge:: SVoiceClass
   :type: speect class
   :members: none
   :inheritance: SObjectClass.SVoiceClass
 

.. seealso:: :doc:`SVoice_detail`.


.. doxybridge:: s_voice_info
   :type: typedef struct


Functions
---------

.. doxybridge:: SVoiceSynthUtt

.. doxybridge:: SVoiceReSynthUtt

.. doxybridge:: SVoiceGetName

.. doxybridge:: SVoiceGetDescription

.. doxybridge:: SVoiceGetGender

.. doxybridge:: SVoiceGetLanguage

.. doxybridge:: SVoiceGetLangCode

.. doxybridge:: SVoiceGetVersion

.. doxybridge:: SVoiceGetDataKeys

.. doxybridge:: SVoiceDataIsPresent

.. doxybridge:: SVoiceGetData

.. doxybridge:: SVoiceSetData

.. doxybridge:: SVoiceDelData

.. doxybridge:: SVoiceGetFeatureKeys

.. doxybridge:: SVoiceFeatureIsPresent

.. doxybridge:: SVoiceGetFeature

.. doxybridge:: SVoiceSetFeature

.. doxybridge:: SVoiceDelFeature

.. doxybridge:: SVoiceGetFeatProcKeys

.. doxybridge:: SVoiceFeatProcIsPresent

.. doxybridge:: SVoiceGetFeatProc

.. doxybridge:: SVoiceSetFeatProc

.. doxybridge:: SVoiceDelFeatProc

.. doxybridge:: SVoiceGetUttProcKeys

.. doxybridge:: SVoiceUttProcIsPresent

.. doxybridge:: SVoiceGetUttProc

.. doxybridge:: SVoiceSetUttProc

.. doxybridge:: SVoiceDelUttProc

.. doxybridge:: SVoiceGetUttTypesKeys	

.. doxybridge:: SVoiceUttTypeIsPresent

.. doxybridge:: SVoiceGetUttType

.. doxybridge:: SVoiceSetUttType

.. doxybridge:: SVoiceDelUttType



