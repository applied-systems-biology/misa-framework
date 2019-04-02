+++
title = "Attachments"
type="page"
creatordisplayname = "Ruman Gerst"
creatoremail = "ruman.gerst@leibniz-hki.de"
lastmodifierdisplayname = "Ruman Gerst"
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++

MISA++ comes with a system to attach quantification results and other
information to input and output data. The information is stored within
objects that are serialized into JSON format, including additional
information such as the object type.

Attachments are saved in the `attachments/imported` and
`attachments/exported` folders within a [MISA++ result folder](../output-data).
The files are structured according to the filesystem (see [Parameters](../parameters)).

An attachment file has the following structure:

{{<mermaid align="center">}}
graph LR;
Root["root : object"]-->|for each attachment|A[" : misa-serializable"]
Root --> RootL[location : misa-location]
S[misa-serializable : object]
S --> Sid[misa:serialization-id : string]
S --> Shierarchy[misa:serialization-hierarchy : array of string]
S -.-> O["Other properties"]
L[misa-location : misa-serializable]
L --> FsLocation[filesystem-location : string]
L --> FsULocation[filesystem-unique-location : string]
L --> FsILocation[internal-location : string]
L -.-> O2["Other properties"]
{{< /mermaid >}}

# root

The root of an attachment file is a JSON object that maps from
`misa:serialization-id` to the attached object.
It has an additional entry `location` that maps to a `misa:location`.

# misa-serializable

## misa:serialization-id

Unique identifier of the object type.
Follows the [Serialization ID](../serialization-id) standard.

## misa:serialization-hierarchy

A list of strings the describes the inheritance hierarchy of the object.
The first entry is always `misa:serializable`.
The last entry is always the current `misa:serialization-id`.
The hierarchy is ordered from the most basic type to the current one.

# misa-location

Inherits all properties from `misa:serializable`.

## filesystem-location

Absolute path of the folder that contains the input or output data.
Different parts of the data have the same `filesystem-location`.

## filesystem-unique-location

Absolute path to a file or folder within the the `filesystem-location`.
Unique for each part of the data.

## internal-location

Internal MISA++ filesystem location.
