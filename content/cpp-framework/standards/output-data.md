+++
title = "Result folder"
type="page"
creatordisplayname = "Ruman Gerst"
creatoremail = "ruman.gerst@leibniz-hki.de"
lastmodifierdisplayname = "Ruman Gerst"
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++

MISA++ framework applications write their output into a folder that contains
all neccessary information to repeat the analysis.

The folder has following structure:

{{<mermaid align="center">}}
graph LR;
A["Result folder : directory"] --> B["Parameters : file"]
A --> C["Module Info : file"]
A --> D["Parameter schema : file"]
A --> E["Runtime log : file"]
A --> G["Attachments : folder"]
G --> H["Attachment serialization schemata : file"]
G -.->|optional|I["imported : folder"]
G -.->|optional|J["exported : folder"]
I -->|For each sample| K["Input data attachments : folder"]
J -->|For each sample| L["Output data attachments : folder"]
A -->|For each sample|F["Sample result data : folder"]
{{< /mermaid >}}

# Parameters

A file `parameters.json` in JSON format.
A copy of the input parameter file.
See [Parameters](../parameters) for more information.

# Module Info

A file `misa-module-info.json` in JSON format.
The same as the output of of `<module> --module-info`.
See [Module Info](../module-info) for more information.

# Parameter schema

A file `parameter-schema.json` in JSON format.
See [Parameter schema](../parameter-schema) for more information.

# Runtime log

A file `runtime-log.json` in JSON format.
See [Runtime log](../runtime-log) for more information.

# Attachments

A folder `attachments`.

## Attachment serialization schemata

A file `serialization-schemas.json` that contains serialization schemas for
all attached objects.
See [Attachments](../attchments) for more information.

## imported

An optional folder `imported`.

### Input data attachments

A structure of folders that follows the data structure defined by the MISA++ application.
Folders contain JSON files (extension `.json`).
The files contain attached objects assigned to the data via the directory structure.
See ["Attachments"](../attachments) for more information.

## exported

An optional folder `exported`.

### Input data attachments

A structure of folders that follows the data structure defined by the MISA++ application.
Folders contain JSON files (extension `.json`).
The files contain attached objects assigned to the data via the directory structure.
See ["Attachments"](../attachments) for more information.

# Sample result data

A structure of folders that follows the data structure defined by the MISA++ application
unless folders have been redirected via the filesystem.
See [Parameters](../parameters) for more information.
