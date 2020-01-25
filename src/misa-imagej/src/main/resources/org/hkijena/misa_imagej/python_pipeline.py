#!/usr/bin/env python3

import glob
import json
import os
import platform
import subprocess

module_paths = []
detected_modules = {}


def setup_module_paths():
    # Add user config
    if platform.system() == "Linux":
        if "XDG_CONFIG_HOME" in os.environ:
            module_paths.append(os.environ["XDG_CONFIG_HOME"] + "/MISA-ImageJ/misa-modules/")
        else:
            module_paths.append(os.environ["HOME"] + "/.config/MISA-ImageJ/misa-modules/")
    elif platform.system() == "Windows":
        module_paths.append(os.environ["APPDATA"] + "/MISA-ImageJ/misa-modules/")

    # Add environment paths
    if "MISA_MODULE_LINK_PATHS" in os.environ:
        for path in os.environ["MISA_MODULE_LINK_PATHS"].split(";"):
            module_paths.append(path)

    # Add global config
    if platform.system() == "Linux":
        module_paths.append("/usr/lib/misaxx/modules")
        module_paths.append("/usr/local/lib/misaxx/modules")
        module_paths.append("/usr/local/lib32/misaxx/modules")
        module_paths.append("/usr/local/lib64/misaxx/modules")


def get_module_id(module_link):
    try:
        module_info_json = subprocess.check_output([module_link["executable-path"], "--module-info"]).decode("utf-8")
        data = json.loads(module_info_json)
        return data["name"]
    except:
        return None


def load_modules():
    for module_path in module_paths:
        for module_link_file in glob.iglob(module_path + "/*.json"):
            with open(module_link_file, "r") as f:
                module_link = json.load(f)
                if "executable-path" in module_link and os.path.exists(module_link["executable-path"]):
                    id = get_module_id(module_link)
                    if id is not None:
                        detected_modules[id] = module_link


print("<#> MISA++ Python pipeline runner")
print("<#> Loading modules ...")
setup_module_paths()
load_modules()

print("<#> Loading pipeline.json ...")
nodes = []
edges = {} # Edges as list target -> [sources]
cache_links = {} # Target node -> [{ source, destination }]
node_modules = {}
with open("pipeline.json", "r") as f:
    pipeline = json.load(f)
    for node in pipeline["nodes"]:
        nodes.append(node)
        node_modules[node] = pipeline["nodes"][node]["module-name"]

        if not node_modules[node] in detected_modules:
            raise Exception("Could not find required module " +  node_modules[node])

    for edge in pipeline["edges"]:
        target = edge["target-node"]
        source = edge["source-node"]
        if not target in edges:
            edges[target] = set()
        edges[target].add(source)
        if "source-cache" in edge and "target-cache" in edge and "sample" in edge:
            if not target in cache_links:
                cache_links[target] = []
            cache_links[target].append({ "source" : source + "/exported/" + edge["sample"] + "/" + edge["source-cache"],
                                         "target" : target + "/imported/" + edge["sample"] + "/" + edge["target-cache"] })


print("<#> Solving dependencies ...")


def traverse(nodes, edges):
    result = []

    while len(result) != len(nodes):
        for node in nodes:
            if node in result:
                continue
            if not node in edges or len(edges[node] - set(result)) == 0:
                result.append(node)
                break

    return result


ordered_nodes = traverse(nodes, edges)
for node in ordered_nodes:
    print("<#> Preparing to start node " + node)
    if node in cache_links:
        for link in cache_links[node]:
            sourcedir = os.getcwd() + "/" + link["source"]
            targetdir = os.getcwd() + "/" + link["target"]
            sourcedir = sourcedir.rstrip("/")
            targetdir = targetdir.rstrip("/")
            if not os.path.exists(targetdir):
                os.makedirs(targetdir, exist_ok=True)
            if os.path.exists(targetdir):
                if os.path.islink(targetdir):
                    os.remove(targetdir)
                elif os.path.isdir(targetdir):
                    os.rmdir(targetdir)

            os.symlink(sourcedir, targetdir)
    print("<#> Starting node " + node)
    cd_backup = os.getcwd()
    os.chdir(node)
    subprocess.run([detected_modules[node_modules[node]]["executable-path"], "--parameters", "parameters.json"])
    os.chdir(cd_backup)
