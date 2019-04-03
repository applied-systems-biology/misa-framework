+++
title = "Running"
weight = 20
type="page"
creatordisplayname = "Ruman Gerst"
creatoremail = "ruman.gerst@leibniz-hki.de"
lastmodifierdisplayname = "Ruman Gerst"
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++

To run a module, you can either run it using a graphical user interface such as the MISA++ ImageJ plugin or run it directly via the command line.

# Command line interface

A worker module is an executable that has a command line interface (CLI). You can always run `<module> --help` to show all parameters.

To run a workload, run `<module> --parameters <parameter file>`. It will start doing the tasks and exits after they are done. The CLI also allows you to quickly change some runtime parameters without editing the parameter file. You can override the number of threads using `--threads <number of threads>` and enable the complete runtime log with `--full-runtime-log`.

You can also query the version and full module info in JSON format using `--version` and `--module-info` respectively.

Each runnable module is able to create a description of its parameter file, input and output directory structure and data types. This parameter schema is generated using `--write-parameter-schema <file>`. No parameter file is necessary to generate a parameter schema.

All necessary information will also be written into the output directory after the module finishes its work.
