Description
===========

**Protoconf** is header only configuration helper.

The program usualy starts from argument parsing, configuration file creation and configuration class, to store it in memory. It is not hard but time consuming work.

This project should make it easier and less time consuming work with configuration files. So the brand new project could be started faster.

It is based on protobuf project which is capable to convert messages to JSON format. So all you need - create *scheme.proto* file, run code generation, and you got it, start using it. The helper functions are templates, so any protobuf schema object will be ok.

The only situation when you need library **libProtoconf** is to get the version string, other way just use **protoconf.h** heafer file.

PROS:

   + Creates c++ configuration class with all setters and getters for all fields based on schema.
   + Creates c++ interface to load/store configuration from/to file in JSON format.
   + Creates configuration template file if missing.
   + Allows updating configuration schema, and it is still will be possible to work with older version parsers (if you follow protobuf update policy).
   + Configuration JSON file line swap is allowed
   + Comments supported using // for entire or partial line 
   + Allows using old configuration with newer parser, default values will be used for missing params.
   + This is protobuf, man, you gain ready to send configuration directly via network, or update it same way from remote host (no, network implememntation you should find on your own).

CONS:

   - Tested only with v2 proto files for now
   - During configuration write, original configuration will be completely overwritten, all manual modifications will be erased as the result

Building with Conan (Recomended)
================================

Build dependencies:

   * conan
   * ctags (optional)

.. code:: bash

     mkdir debug
     cd debug && conan install -s build_type=Debug ../.
     conan build ../.

..

If you need build packages for different architectires, use scripts

.. code:: bash

      python build-pc.py
      python build-arm.py

..

Building extras
===============

**-o tags=True**        Tags file generation on

**-o codecov=True**     Enable code coverage

**-o clangcheck=True**  Enable clang static analizer build

**-o asancheck=True**   Enable build with Address Santizer

**-o tsancheck=True**   Enable build with Thread Santizer

**-o usancheck=True**   Enable build with Undefined Santizer

Building without Conan
======================

Install dependencies:

   * protobuf with protoc
   * gtest
   * ctags (optional)

See conanfile.py for more information on versions

Building Debug
==============

Go inside **src** directory.

.. code:: bash

    cmake -DWITH_CONAN=OFF -DCMAKE_BUILD_TYPE=Debug CMakeLists.txt

    make
..

Building Release
================

Go inside **src** directory.

.. code:: bash

    cmake -DWITH_CONAN=OFF -DCMAKE_BUILD_TYPE=Release CMakeLists.txt

    make

..

Building extras
===============

**-DWITH_CONAN=ON**     [default = **ON**]     if need build and package with conan

**-DWITH_TAGS=ON**      [default = **OFF**]    if need tags file generation

**-DWITH_CODECOV=ON**   [default = **OFF**]    Enable code coverage

**-DWITH_ASAN=ON**      [default = **OFF**]    Enable build with Address Santizer

**-DWITH_TSAN=ON**      [default = **OFF**]    Enable build with Thread Santizer

**-DWITH_USAN=ON**      [default = **OFF**]    Enable build with Undefined Santizer

Usage
=====

.. code:: c++

       #include <Protoconf/protoconf.h>

       result = protoconf::save(path, conf);
       ASSERT_TRUE(result);

       result = protoconf::load(path, conf);
       ASSERT_TRUE(result);

..


Example
=======

    See **Schema** subproject for protobuf schema used in tests.

    See **Example** subproject for example.

    Output example:

.. code:: plain

    {
     "descrValName": "This is simple value description",
     "valName": "",
     "descrValUint": "This is dedicated to test integer value",
     "valUint": 10,
     "descrStatisticsInt": "Statistics with integer values",
     "statisticsInt": [
      0,
      1,
      2,
      3,
      4
     ],
     "descrStatisticsFloat": "Statistics with float values",
     "statisticsFloat": [
      -0.5,
      0.5,
      1.5,
      2.5,
      3.5
     ],
     "descrV1conf": "This describes protocol version 1",
     "v1conf": {
      "descrEnable": "If protocol version 1 configuration is used",
      "enable": false,
      "descrName": "This value is special for protocol version 1 name",
      "name": ""
     },
     "descrV2conf": "This describes protocol version 2",
     "v2conf": {
      "descrEnable": "If protocol version 2 configuration is used",
      "enable": false,
      "descrName": "This value is special for protocol version 2 name",
      "name": "",
      "descrCorpus": "Enum values",
      "corpus": "UNIVERSAL",
      "descrData": "Data sequence",
      "datas": [
       {
        "descrId": "Data id",
        "id": "999999999",
        "descrHash": "Data hash",
        "hash": "ADKJEFKJEF",
        "descrValue": "Data value",
        "value": "0"
       },
       {
        "descrId": "Data id",
        "id": "222222222",
        "descrHash": "Data hash",
        "hash": "AAAAAAA",
        "descrValue": "Data value",
        "value": "111111"
       }
      ]
     },
     "descrBig": "This is big description\n with new lines \n lene \n line"
    }

..

