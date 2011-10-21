.. index:: 
   single: Topic Guides; Style Guide

.. index:: Style Guide

.. _style_guide_topic:

===========
Style Guide
===========

Setting a coding style makes it easier to read and change code as
everyone knows what to expect. The recommendations are based on
established standards collected from a number of sources, individual
experience, and local requirements/needs. There are several reasons
for introducing a new guideline rather than just referring to
others. Main reason is that these guides are far too general in their
scope and that more specific rules (especially naming rules) need to
be established.

The rules in this guide are not set in stone and variations that
enhances readability allowed. The main goal of the recommendation is
to improve readability and thereby the understanding and the
maintainability and general quality of the code. It is impossible to
cover all the specific cases in a general guide and the programmer
should be flexible.


.. index:: 
   single: Style Guide; Naming Conventions


Naming Conventions
==================

The reasons for using a naming convention include the following are to
reduce the effort needed to understand the source code and to enhance
source code appearance.

General naming rules
--------------------

	* **Use sensible, descriptive names.**
	  Do not use short cryptic names or names based on internal
	  jokes. It shall be easy to type a name without looking up
	  how it is spelt.  Exception: Loop variables and variables
	  with a small scope (less than 20 lines) may have short names
	  to save space if the purpose of that variable is obvious.

	* **Only use english names.**
	  It is confusing when mixing languages for names. English is
	  the preferred language because of its spread in the software
	  market and because most libraries used already use English.

	* **Variables with a large scope shall have long names, variables with a small scope can have short names.** 
	  Scratch variables used for temporary storage or indices are
	  best kept short. A programmer reading such variables shall
	  be able to assume that its value is not used outside a few
	  lines of code. Common scratch variables for integers are ``i``,
	  ``j``, ``k``, ``m``, ``n`` and for characters ``c`` and ``d``.

	* **Use name prefixes for identifiers declared in different modules.** 
	  This avoids name clashes. 

	* **No names with leading and trailing underscores.**
	  Names with leading and trailing underscores are reserved for
          system purposes and should not be used for any user-created
          names.


Speect naming rules
-------------------

The Speect library naming rules can be broken up into the following sections:

    
.. rubric:: Data types

================  ======    =======    ==================
Data type         Prefix    Postfix    Example
================  ======    =======    ==================
typedef           s\_                  s_hash_element
struct            s\_                  s_hash_element
union             s\_                  s_hash_element
enum              s\_                  s_hash_element
function pointer  s\_       \_fp       s_list_free_fp
object            S                    SMapHashTable
class             S         Class      SMapHashTableClass
================  ======    =======    ==================

Class and object names shall begin with an upper case letter and words
shall begin with an upper case letter ("CamelCase").


.. rubric:: Defines

=================  ======  ==============
Define             Prefix  Example
=================  ======  ==============
Macro              S\_     S_MALLOC
Constant           S\_     S_PI
=================  ======  ==============


.. rubric:: Functions

=====================   =====================================================================   ======  =========================
Function                Format                                                                  Prefix  Example
=====================   =====================================================================   ======  =========================
Speect private/static   Underscore "_" between each word                                        \_s\_   _s_load_voice_data_config
Private/static          Underscore "_" between each word                                        \_      _find_name
Speect global/extern    Underscore "_" between each word                                        s\_     s_list_index
Global/extern           Underscore "_" between each word                                                name_finder
Speect class methods    Start with object/class name, capitalize first character of each word   S       SItemLastDaughter
=====================   =====================================================================   ======  =========================

Class method names shall begin with the class's object name, for
example ``SItem``, and the method shall begin with an upper case
letter and words shall begin with an upper case letter ("CamelCase"),
for example the "last daughter" method of ``SItem`` is named
``SItemLastDaughter``.


.. rubric:: Variables

Only Speect class and object variables have a naming convention:

=============  ======================================= ======= ================
Variable       Format                                  Postfix Example
=============  ======================================= ======= ================
Speect object  CamelCase with first letter small caps          ebmlPlugin
Speect class   CamelCase with first letter capitalized Class   G2PRewritesClass
=============  ======================================= ======= ================


.. index:: 
   single: Style Guide; Indent Style

Indent Style
============

The indent style is the indentation style used on compound
statements. A compound statement is a list of statements enclosed by
braces. There are many common ways of formatting braces. The one used
in the Speect library is known as the Allman_ style as defined by
Wikipedia Indent Style. The style can be activated in *Emacs cc-mode*
with::

	(c-mode . "bsd")

and has a tabstop of 4 spaces. For example:

.. code-block:: c

   if (x == NULL)
   {
       tmp--;
       if (tmp == 0)
       {
           cnt--;
	   if (cnt == 0)
	   {
	       break;
	   }
	   else
	   {
	       cnt--;
	   }
       }
   }



.. index:: 
   single: Style Guide; File Organization

File Organization
=================

A file consists of various sections that should be separated by
several blank lines. Although there is no maximum length limit for
source files, files with more than about 1000 lines are cumbersome to
deal with and should be the exception.


File and directory naming
-------------------------

.. rubric:: Header file names

Avoid header filenames that are the same as C library filenames. The
statement ``#include "math.h"`` should include the standard C library
math header file and not a Speect header file.


.. rubric:: Directory naming

Directory names should be short, concise and all small caps. The
:ref:`directory_layout_topic` is a good example to follow.


Program files
-------------

Program files consists of header files and source code files. As a
rule, header files should be used to give interface
specifications and source files should implement that interface.


.. rubric:: Nesting of header files

Header files should not be nested, in other words a header file's
included modules should describe other headers needed for that header
so that it can be used on it's own.


.. rubric:: Included modules 

All standard C library includes should be before any Speect library
includes.


.. rubric:: Inclusion guard

Header files should be guarded from duplicate inclusion by testing for
the definition of a value, for example:

.. code-block:: c

   #ifndef _SPCT_OBJECT_H__
   #define _SPCT_OBJECT_H__

where ``object.h`` is the header file's name.


.. index:: 
   single: Style Guide; File Layout

File Layout
===========

The layout of the source and header files should follow the sequence
of sections given below. Every section should be preceded with the
appropiate comment block for that section as shown, of course not all
files will require every section as given. All source and header files
in Speect follow this layout, which makes the navigation of the library
much easier as everything is consistent.


      **File header**

      .. literalinclude:: layout_example.h
         :language: c
	 :lines: 1-16

      Where all the ``$tags$`` are replaced with their appropriate entries.

    
      **Inclusion guard** (header files only)

      .. literalinclude:: layout_example.h
         :language: c
	 :lines: 18-19


      **Additional licenses**

      .. literalinclude:: layout_example.h
         :language: c
	 :lines: 22-27


      **Doxygen file description** (header files only)

      .. literalinclude:: layout_example.h
         :language: c
	 :lines: 30-33


      **Modules used** (``#include`` files)

      .. literalinclude:: layout_example.h
         :language: c
	 :lines: 36-40


      **Begin external c declaration** (header files only)

      .. literalinclude:: layout_example.h
         :language: c
	 :lines: 43-48

      Adds the :c:macro:`S_BEGIN_C_DECLS` so that the header file can
      be included in C++ projects.


      **Macros**

      .. literalinclude:: layout_example.h
         :language: c
	 :lines: 51-55


      **Defines**

      .. literalinclude:: layout_example.h
         :language: c
	 :lines: 58-62


      **Constants** (source files only, no global constants) 

      .. literalinclude:: layout_example.h
         :language: c
	 :lines: 65-69


      **Typedefs and Data types** (*structs* and *enums*)

      .. literalinclude:: layout_example.h
         :language: c
	 :lines: 72-76


      **External definitions**

      .. literalinclude:: layout_example.h
         :language: c
	 :lines: 79-83


      **Static function prototypes**  (source files only)

      .. literalinclude:: layout_example.h
         :language: c
	 :lines: 86-90


      **Function prototypes**  (header files only)

      .. literalinclude:: layout_example.h
         :language: c
	 :lines: 93-97


      **Function implementations**  (source files only)

      .. literalinclude:: layout_example.h
         :language: c
	 :lines: 100-104


      **Static function implementations**  (source files only)

      .. literalinclude:: layout_example.h
         :language: c
	 :lines: 107-111


      **Static class function implementations**  (source files only)

      .. literalinclude:: layout_example.h
         :language: c
	 :lines: 114-118


      **Class initialization**  (source files only)

      .. literalinclude:: layout_example.h
         :language: c
	 :lines: 121-125


      **End external c declaration** (header files only)

      .. literalinclude:: layout_example.h
         :language: c
	 :lines: 128-133

      Adds the :c:macro:`S_END_C_DECLS` so that the header file can
      be included in C++ projects (closes :c:macro:`S_BEGIN_C_DECLS`).


      **End inclusion gaurd** (header files only)

      .. literalinclude:: layout_example.h
         :language: c
	 :lines: 136-136

Comments
========

	* Only C style comments should be used, i.e.

	  .. code-block:: c

	     /* this is the right style of comment */
	     // this is not right


	* Commenting out blocks of code must be done with the ``#if 0`` and
	  ``#endif`` pre-processor statements. Therefore, do not nest comments.

	  .. code-block:: c

	     #if 0 /* indicate the reason the code below is commented out */
	     #define WKDS_SIZE  20  /* size */
	     #define WKDS_LENGTH 10 /* length */
	     #endif


	* Very short comments can appear on the same line as the code as long as
	  it is visually seperated with tabs or spaces. If more than one short
	  comment appears in a block of code they should all be tabbed to the
	  same column.

	  .. code-block:: c

	     if (a == NULL)
	     {
	     	b = TRUE;      /* special case  */
	     }
	     else
	     {
	     	b = FALSE;     /* standard case */
	     }

	* Use as much English as possible.


Declarations
============

Variables
---------

	* The pointer qualifier, ``*``, should be with the variable name rather than the variable type.

	  .. code-block:: c
	    
	     char *s;  /* correct */
	     char* s;  /* wrong   */


	* Declare only one variable per line and type.

	  .. code-block:: c

	     /* correct */
	     char *s;
	     char *t;

	     /* wrong */
	     char *s, *t;

	* For structure, union and enumaration declarations, each
          element should be alone on a line with a comment describing
          it.

	  .. code-block:: c
	  
	     struct rectangle
	     {
		int lngth;       /* rectangle length */
		int wdth;        /* rectangle width  */
	     }

	* Any variable whose initial value is important should be
	  explicitly initialized. Structure initializations should be
	  fully parenthesized with braces. Constants used to
	  intialize longs of floats should be explicitly long or
	  float.


Functions
---------

	* Each function in the header files should be fully documented
          in the *doxygen* Java style (all parameters and returns).

	  .. code-block:: c

	     /**
 	      * Set a character in a UTF-8 string and return the byte size.
	      * of the character.
	      * @param s The string.
	      * @param c The UTF-8 character.
	      * @return The byte size of the character.
	      */
	     int spct_utf8_setc(char *s, uint32 c);


	* Function return types and parameters must be explicitly
          defined and must not default to ``int``.

	* Avoid local declarations that overide declarations at
          higher levels.


Whitespace
==========

	* Use vertical and horizontal whitespace
	  generously. Indentation and spacing should reflect the block
	  structure of the code; e.g. there should be **2** blank lines
	  between the end of one function and the beginning of the
	  next.

	* A long string of conditionals should be split into seperate lines, for example:

	  .. code-block:: c
	     
	     if ((temp != NULL) && (c1_to == NULL) || (count < 2)) /* not good */

	  can be read better as

	  .. code-block:: c

	     /* good */

	     if ((temp != NULL)
	     	&& (c1_to == NULL)
		|| (count < 2))

	* Simularly, long *for* loops should be split onto different lines:

	  .. code-block:: c

	     for (curr = *iterator, count = 1;
	     	  curr != NULL;
		  curr = next(curr), count++)

	* Add whitespaces between variables and operators.

	  .. code-block:: c

	     c=(lnw*3)/(tmp+4);       	/* wrong */
	     c = (lnw * 3) / (tmp + 4); /* good  */

	* Keywords that are followed by expressions in parentheses
	  should be seperated from the left paranthesis by a blank
	  (the ``sizeof`` operator is an exception). Blanks should appear
	  after commas in argument lists to help seperate the
	  arguments visually.

Statements
==========


Simple Statements
-----------------

	* The null body of a *for* or *while* loop should be alone on a
	  line and commented so that it is clear that the null body is
	  intentional and not missing code.

	  .. code-block:: c
	  
	     while ((*dest)++ = (*src)++)
	     	   ; /* VOID */


	* Do not default the test for ``NULL``.

	  .. code-block:: c

	     if (f != NULL)

	  is more readable than

	  .. code-block:: c
	  
	     if (f)


.. _Allman: http://en.wikipedia.org/wiki/Indent_style#Allman_style
