+++
title = "Parameters"
type="page"
creatordisplayname = "Ruman Gerst"
creatoremail = "ruman.gerst@leibniz-hki.de"
lastmodifierdisplayname = "Ruman Gerst"
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++

Parameters are in JSON format and have following structure:

{{<mermaid align="center">}}
graph LR;
Root["root : object"]-->Filesystem["filesystem : object"]
Filesystem --> FilesystemSource["source : string"]
Filesystem -.->|if source is 'json'| FSJsonData["json-data : string"]
Filesystem -.->|if source is 'directories'| FSInputDir["input-directory : string"]
Filesystem -.->|if source is 'directories'| FSOutputDir["output-directory : string"]
Root -->Algorithm["algorithm : object"]
Root -->Samples["samples : object"]
Root -->Runtime["runtime : object"]
Samples -.->|for each sample| SampleParams[" : object"]
Runtime -.->|optional| NumThreads["num-threads : integer"]
Runtime -.->|optional| FullRuntimeLog["full-runtime-log : boolean"]
Runtime -.->|optional| RequestsSkipping["request-skipping : boolean"]
{{< /mermaid >}}

# filesystem

Describes the virtual filesystem that is used by the MISA++ application.

## source

Determines how MISA++ imports the filesystem.

Following values are valid:

* `json` imports the filesystem from `json-data`
* `directories` imports the filesystem from `input-directory` and `output-directory`

## input-directory

If `source` is `directories`, input data is imported from the provided directory.
The folder structure must be consistent with the filesystem structure expected by
the MISA++ application (see [Parameter schema](../parameter-schema)).

Each data folder can contain a file `misa-metadata.json` in JSON format that
has equivalent effects to the `metadata` property in `json-data` (see below).

## output-directory

If `source` is `directories`, output data is exported to the provided directory.

## json-data

Imports the filesystem via the `json-data` property. It should be consistent with the
`filesystem` parameter schema (see [Parameter schema](../parameter-schema)).

The `json-data` property has following structure:

{{<mermaid align="center">}}
graph LR;
Root["json-data : object"]
Imported["imported : filesystem-entry"]
Exported["exported : filesystem-entry"]
Root --> Imported
Root --> Exported


FsEntry["filesystem-entry : object"]
FsEntry -.->|optional*| ExternalPath["external-path : string"]
FsEntry -.->|optional| Children["children : object"]
FsEntry -.->|optional| Metadata["metadata : object"]
{{< /mermaid >}}

## imported

An object of type `filesystem-entry`.
The `external-path` property **must** be set to a valid path.

## exported

An object of type `filesystem-entry`.
The `external-path` property **must** be set to a valid path.

## filesystem-entry

### external-path

Links the virtual filesystem entry to a physical filesystem location.
Required for `imported` and `exported`.

Entries in `children` automatically link to corresponding sub-folders in
the path unless `external-path` of the child overrides the previous definition.

### children

Map from directory name to a `filesystem-entry` type.

### metadata

Optional metadata for data import. The [Parameter schema](../parameter-schema)
contains information about the structure of `metadata`.

# algorithm

Parameters that are independent from sample.
The structure depends on the MISA++ application.
See [Parameter schema](../parameter-schema) for more information.

# sample

Map from sample name to sample parameters.
The structure of sample parameters depends on the MISA++ application.
See [Parameter schema](../parameter-schema) for more information.

# runtime

Global parameter for the MISA++ runtime.
The structure of sample parameters depends on the MISA++ application.
See [Parameter schema](../parameter-schema) for more information.

Following parameters are defined by MISA++ Core itself and are always present:

## num-threads

Number of threads. Must be at least `1`.

## full-runtime-log

If `true`, a fully detailed runtime log (see [Runtime log](../runtime-log))
is created. If `false`, only an overview is generated.
Defaults to `false`.

## request-skipping

If `true`, algorithms are informated that existing results should be re-used and
not overwritten. Depends on the algorithm implementation.
Defaults to `false`.
