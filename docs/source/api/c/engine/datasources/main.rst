.. index:: ! Data Sources (C API)


.. _datasources/main:

============
Data Sources
============

Provides a generic interface to different data sources, where a data
source can be anything that can be read from and/or written to. Speect
provides two levels of interfaces to data sources, one for structured
data and one for unstructured data.

The structured data interface is for data that is in a specific
format, at a higher abstract level, for example reading data of a
serialized object, whereas the unstructured data interface is for any
type of data, for example reading an integer from a file.  The
structured data interface typically makes use of the unstructured data
interface on a lower level.


.. toctree::
   :maxdepth: 2	   

   unstructured 
   structured

