+++
title = "Public API"
type="page"
creatordisplayname = "Ruman Gerst"
creatoremail = "ruman.gerst@leibniz-hki.de"
lastmodifierdisplayname = "Ruman Gerst"
lastmodifieremail = "ruman.gerst@leibniz-hki.de"
+++


The public API of a MISA++ module should follow a specific structure to make it easier for other developers to use it.


| File/Folder                                | Contains                                                                                                       |
|--------------------------------------------|----------------------------------------------------------------------------------------------------------------|
| ./include/\<module-name\>/accessors/         | Derivatives of misaxx::misa_cached_data                                                                        |
| ./include/\<module-name\>/attachments/       |  Derivatives of misaxx::misa_serializable or misaxx::misa_locatable (preferred). For example quantified data.  |
| ./include/\<module-name\>/caches/            | Derivatives of misaxx::misa_cache                                                                              |
| ./include/\<module-name\>/descriptions/      | Derivatives of misaxx::misa_data_description                                                                   |
| ./include/\<module-name\>/patterns/          | Derivatives of misaxx::misa_data_pattern                                                                       |
| ./include/\<module-name\>/utils/             | Anything else (non-serializable data types, converters, helpers, ...)                                          |
| ./include/\<module-name\>/module_interface.h | Only for worker modules: A misaxx::misa_module_interface derivative                                            |
| ./include/\<module-name\>/module.h           | Only for worker modules: A misaxx::misa_module derivative                                                      |
| ./include/\<module-name\>/module_info.h           | A function `module_info()` that returns a misaxx::misa_module_info                                                      |
