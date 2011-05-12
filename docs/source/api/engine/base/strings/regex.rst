.. index:: 
   single: Strings (C API); Regular Expressions

.. _regex:

===================
Regular Expressions
===================

Provides *utf-8* aware regular expressions. The Speect regular expression syntax is a bit
different from the POSIX or Perl syntaxes. The supported meta-character (operators) are
mostly the same, that is: . | ( ) [ ] ? + * ^ $ 
 
Syntax Reference
================

============= ===========
Metacharacter Description 
============= ===========
.             Matches any single character.
\|            The alternation operator matches either the expression before or the expression after the operator. For example, abc|def matches "abc" or "def".
()            Defines a marked group.
[]            A bracket expression. Matches a single character that is contained within the brackets. \
              For example, [abc] matches "a", "b", or "c". [a-z] specifies a range which matches any \
              lowercase letter from "a" to "z". These forms can be mixed: [abcx-z0-9] matches "a", "b", \
              "c", "x", "y", "z" or single digits "0" to "9".
[^ ]          Matches a single character that is not contained within the brackets. For example, \
              [^abc] matches any character other than "a", "b", or "c". [^a-z] matches any single \
              character that is not a lowercase letter from "a" to "z".
?             Matches the preceding element zero or one time.
\+            Matches the preceding element one or more times.
\*            Matches the preceding element zero or more times.
^             Matches the starting position within the string.
$             Matches the ending position of the string.
============= ===========

Escaping (literal character inclusion) is supported using the \\-character. 
POSIX character classes are not supported. Unlike the POSIX or Perl variants,
the Speect regular expression engine always matches the whole string, not any part of it.
That is, regular expression "a" matches the string "a", but not the string
"blah", whereas a POSIX or Perl regular expression would match both
strings. To get the latter behaviour, simply add ".*" before and
after the string, i.e. ".*a.*".

The regular expressions are matched using a purely NFA
(nondeterministic finite automaton) based approach. No backtracking
algorithm is provided.

Interface
=========

.. doxybridge:: s_regex_comp

.. doxybridge:: s_regex_match

.. doxybridge:: s_regexsub_num_groups

.. doxybridge:: s_regexsub_group

.. doxybridge:: s_regex_flags
   :type: typedef enum

