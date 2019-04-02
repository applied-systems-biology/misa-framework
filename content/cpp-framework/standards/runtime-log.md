+++
title = "Runtime log"
type="page"
creatordisplayname = "Ruman Gerst"
creatoremail = "ruman.gerst@leibniz-hki.de"
lastmodifierdisplayname = "Ruman Gerst"
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++

The runtime log contains detailed information about individual tasks executed
by a MISA++ application.

It is in JSON format and has following structure:


{{<mermaid align="center">}}
graph LR;
Root["root : object"]-->Entries["entries : object"]
Entries -->|for each thread| ThreadEntry[" : array of task-entry"]
TaskEntry["task-entry : object"] --> Name["name : string"]
TaskEntry --> StartTime["start-time : number"]
TaskEntry --> EndTime["end-time : number"]
TaskEntry --> Unit["unit : string"]
{{< /mermaid >}}

# entries

A map from `thread$` where `$` is the thread number to a list of `task-entry`.

# task-entry

## name

Name of the task.

## start-time

Time in `unit` relative to the MISA++ application start time when the task was started.

## end-time

Time in `unit` relative to the MISA++ application start time when the task was ended.

## unit

Unit of `start-time` and `end-time`.
