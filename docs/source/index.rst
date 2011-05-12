.. Speect documentation master file, created by
   sphinx-quickstart on Fri May  6 11:35:02 2011.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

.. _index:

=====
Intro
=====


.. toctree::
   :hidden: 

   contents
   copyright
   download


.. image:: .static/speect_logo_full.png
   :align: center
   :class: index_logo
 

Speect is a multilingual text-to-speech (TTS) system. It offers a full TTS
system (text analysis which decodes the text, and speech synthesis, which encodes
the speech) with various API's, as well as an environment for research and development 
of TTS systems and voices. The following speech synthesis technologies are currently
supported:

  * Unit Selection -- Concatenative halfphone sized unit selection, and
  * HMM-based synthesis -- HMM-based synthesis plug-ins for the `HTS Engine <http://hts-engine.sourceforge.net/>`_.


Speect is written in the C language, with a strict conformance to the
ISO/IEC 9899:1990 standard, thereby allowing for maximum portability
to different computing platforms. Platform specific system calls are
abstracted to allow ports to new platforms.

Speect also includes Python bindings for general customization and quick
implementation of new ideas, and uses a plug-in mechanism for extensions.
It is still under constant and active development, but can already be used
for creating TTS voices, and doing research.

Speect is being developed by the `Human Language Technologies <http://www.meraka.org.za/humanLanguage.htm>`_
group at the Meraka Institute, CSIR, South Africa. We use it in our projects
(see http://www.meraka.org.za/lwazi/) as a TTS server for multilingual
text-to-speech synthesis.

For more information or help please go to the `Speect Sourceforge support page <http://sourceforge.net/projects/speect/support>`_
or send an email to `speect-users@lists.sourceforge.net <mailto:speect-users@lists.sourceforge.net>`_.

.. only:: html
 
   .. container:: quicklinks

      .. container:: link_documentation
      	 
	 * :doc:`contents`

      .. container:: link_download
      	 
	 * :ref:`download`

       .. container:: link_license
      	 
	 * :doc:`copyright`
