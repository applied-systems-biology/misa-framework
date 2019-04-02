+++
title = "Glossary"
weight = 1
type="page"
creatordisplayname = "Ruman Gerst"
creatoremail = "ruman.gerst@leibniz-hki.de"
lastmodifierdisplayname = "Ruman Gerst"
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++

# MISA++ application

Application that uses the MISA++ CLI to create an executable from a [MISA++ worker module](#misa-worker-module).

# MISA++ worker module

A [MISA++ module](#misa-module) that contains a [module dispatcher](#module-dispatcher), a [module interface](#module-interface) and a [module info](#module-info) function.

# MISA++ module

A library that follows the [MISA++ public API standard](standards/public-api) and is built on MISA++ Core.

# Module interface

A class that inherits from `misaxx::misa_module_interface` and is part of the public API of the [module](#misa-module).

# Module dispatcher

A class that inherits from `misaxx::misa_module` and is part of the public API of the [module](#misa-module).

# Module info

A function `module_info()` in the namespace of the [MISA++ module](#misa-module) that returns a `misaxx::misa_module_info` instance (see [Module Info](standards/module-info)).
