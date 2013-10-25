wbranch
=======

Simple tree structure for serialization.

Nested structure:

    The instance of this class is able to have references of
    itself or another instances.

Serializer, Desirializer:

    The instance of this class is able to serialize to 
    std::wstring, and deserialize from it. The resulted 
    string is like JSON format, but It is not compatible to
    JSON.


Require
=======

This library requires only std::wstring and std::vector.


Attention
=========

When you have no reason , you should choose another/cool
library(ex. libjson) to makes your happy. :D
This project not supports all of them.


Author
======

dyama <dyama@member.fsf.org>

