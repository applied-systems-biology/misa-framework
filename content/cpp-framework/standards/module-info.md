+++
title = "Module info"
type="page"
creatordisplayname = "Ruman Gerst"
creatoremail = "ruman.gerst@leibniz-hki.de"
lastmodifierdisplayname = "Ruman Gerst"
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++

The module information is accessible via the command line parameter `<module> --module-info`
and as JSON file in MISA++ output folders (see [Result folder](../output-data)).

It is data in JSON format with following structure:

{{<mermaid align="center">}}
graph LR;
Root["root : object"]-->Id["id : string"]
Root -->Version["version : string"]
Root -.->|Optional| Name["name : string"]
Root -.->|Optional| Description["description : string"]
Root -.->|Optional| Citation["citation : string"]
Root -.->|Optional| URL["url : string"]
Root -.->|Optional| License["license : string"]
Root -.->|Optional| Organization["organization : string"]
Root -.->|Optional| IsExternal["is-external : boolean"]
Root -.->|Optional| Authors["authors : array of string"]
Root -.->|Optional| Dependencies["dependencies : array of module info"]
{{< /mermaid >}}

# id

Unique identifier of the module.
Should be lower-case and only consist of letters `a-z`, `0-9` and `-`.
Should be consistent with the module executable name.

# version

Version of the module.

# name

Optional short and descriptive name.

# description

Optional short description.

# citation

Optional citation referencing a publication that should be cited if a user
wants to credit the module.

# url

Optional website URL.

# license

Optional name of the license.

# organization

Optional name of the organization that developed the module.

# is-external

If `true`, the module info is marked as non-MISA++ dependency.
Should be never true for information about a MISA++ module.
Defaults to false if not defined.

# authors

Optional list of authors.

# dependencies

Optional list of module info JSON objects that lists all dependencies.
