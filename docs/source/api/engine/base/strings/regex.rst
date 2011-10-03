.. index:: 
   single: Strings (C API); Regular Expressions

.. _regex:

===================
Regular Expressions
===================

UTF-8 aware regular expression and functions.

The Speect regular expression syntax is a bit different from the POSIX
or Perl syntax's. The supported meta-character (operators) are mostly
the same, that is: . | ( ) [ ] ? + * ^ $

Escaping (literal character inclusion) is supported using the
\-character. POSIX character classes are not supported. Unlike the
POSIX or Perl variants, the Speect regular expression engine always
matches the whole string, and not part of it. That is, regular
expression "a" matches the string "a", but not the string "blah",
whereas a POSIX or Perl regular expression would match both
strings. To get the latter behaviour, simply add ".*" before and after
the string, i.e. ".*a.*".

The regular expressions are matched using a purely NFA
(nondeterministic finite automaton) based approach. No backtracking
algorithm is provided.


Syntax Reference
================

=============   ===========
Metacharacter 	Description
=============   ===========
. 	        Matches any single character.
\| 	        The alternation operator matches either the expression before or the expression after the operator. For example, abc|def matches "abc" or "def".
( ) 	        Defines a marked group.
[ ] 	        A bracket expression. Matches a single character that is contained within the brackets. For example, [abc] matches "a", "b", or "c". [a-z] specifies a range which matches any lowercase letter from "a" to "z". These forms can be mixed: [abcx-z0-9] matches "a", "b", "c", "x", "y", "z" or single digits "0" to "9".
[^ ] 	        Matches a single character that is not contained within the brackets. For example, [^abc] matches any character other than "a", "b", or "c". [^a-z] matches any single character that is not a lowercase letter from "a" to "z".
? 	        Matches the preceding element zero or one time.
\+ 	        Matches the preceding element one or more times.
\* 	        Matches the preceding element zero or more times.
^  	        Matches the starting position within the string.
$ 	        Matches the ending position of the string. 
=============   ===========

Summary
=======

.. doxybridge-autosummary::
   :nosignatures:

   s_regex_comp
   s_regex_match
   s_regexsub_num_groups
   s_regexsub_group


Definitions
===========

.. doxybridge:: s_regex

.. doxybridge:: s_regexsub

.. doxybridge:: s_regex_flags
   :type: typedef enum


Compile
=======

.. doxybridge:: s_regex_comp


Match
=====

.. doxybridge:: s_regex_match


Query
=====

.. doxybridge:: s_regexsub_num_groups


Extract
=======

.. doxybridge:: s_regexsub_group
