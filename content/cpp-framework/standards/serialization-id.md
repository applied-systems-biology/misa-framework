+++
title = "Serialization ID"
type="page"
creatordisplayname = "Ruman Gerst"
creatoremail = "ruman.gerst@leibniz-hki.de"
lastmodifierdisplayname = "Ruman Gerst"
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++

A serialization ID uniquely identifies an object type.
It consists of two parts separated by a colon:

{{<mermaid align="center">}}
graph LR;
M["Module"] --> C[":"]
C --> T["Type"]
{{< /mermaid >}}

# Module

Lower-case string that consists only of `a-z`, `0-9` and `-`.
Should be consistent with the module ID (see [Module Info](../module-info)).

# Type

Lower-case string that consists only of `a-z`, `0-9`, `-`, `_` and `/` and should form
a valid path.

The first segment of the path should be one of the following categories:

* `attachments` for quantification data and other attachments
* `patterns` for data patterns
* `descriptions` for data descriptions

`misa:serialization-id` is excluded from this rule.
