
.. index:: EBML, Extensible Binary Meta Language

`Extensible Binary Meta Language (EBML) <http://ebml.sourceforge.net>`_
is a generalized file format for any kind of data, aiming to be a binary
equivalent to XML. It provides a basic framework for storing data in XML-like
tags.

.. _python_plugins_ebml:

:mod:`speect.ebml` -- Extensible Binary Markup Language
=======================================================

.. module:: speect.ebml
   :synopsis: Extensible Binary Markup Language format reader/writer class


EBML Reader
-----------

.. autoclass:: SEbmlRead


.. autosummary::

   SEbmlRead.ebml_version
   SEbmlRead.reader_version
   SEbmlRead.max_id_width
   SEbmlRead.max_size_width
   SEbmlRead.doc_type_version
   SEbmlRead.doc_type_reader_version
   SEbmlRead.doc_type
   SEbmlRead.peek_id
   SEbmlRead.skip_element
   SEbmlRead.read_uint
   SEbmlRead.read_sint
   SEbmlRead.read_double
   SEbmlRead.read_str
   SEbmlRead.read_object
   SEbmlRead.open_container
   SEbmlRead.container_exhausted


.. autoattribute:: SEbmlRead.ebml_version

.. autoattribute:: SEbmlRead.reader_version

.. autoattribute:: SEbmlRead.max_id_width

.. autoattribute:: SEbmlRead.max_size_width

.. autoattribute:: SEbmlRead.doc_type_version

.. autoattribute:: SEbmlRead.doc_type_reader_version

.. automethod:: SEbmlRead.doc_type()

.. automethod:: SEbmlRead.peek_id()

.. automethod:: SEbmlRead.skip_element()

.. automethod:: SEbmlRead.read_uint()

.. automethod:: SEbmlRead.read_sint()

.. automethod:: SEbmlRead.read_double()

.. automethod:: SEbmlRead.read_str()

.. automethod:: SEbmlRead.read_object()

.. automethod:: SEbmlRead.open_container()

.. automethod:: SEbmlRead.container_exhausted()


EBML Writer
-----------

.. autoclass:: SEbmlWrite


.. autosummary::


   SEbmlWrite.write_uint
   SEbmlWrite.write_sint
   SEbmlWrite.write_double
   SEbmlWrite.write_str
   SEbmlWrite.write_object
   SEbmlWrite.start_container
   SEbmlWrite.end_container


.. automethod:: SEbmlWrite.write_uint()

.. automethod:: SEbmlWrite.write_sint()

.. automethod:: SEbmlWrite.write_double()

.. automethod:: SEbmlWrite.write_str()

.. automethod:: SEbmlWrite.write_object()

.. automethod:: SEbmlWrite.start_container()

.. automethod:: SEbmlWrite.end_container()
