#!/bin/python3

from os import getenv

class Config:
    def __init__(self, filename: str):
        self.filename: str = filename
    
    def open_conf(self):
        with open(self.filename, "r") as conf:
            self.conf: list[str] = conf.readlines()
            conf.close()
    
    def read_conf(self):
        for i in range(len(self.conf)):
            self.values: dict[str|int|bool] = {}
            nodelist: list[str] = self.conf[i].split("  ")
            startnode: str = nodelist[0]
            if startnode == "set":
                namenode: str = nodelist[1]
                valnode: str = nodelist[2]
                match namenode:
                    case "tab_stop":
                        self.values["tab_stop"] = int(valnode)
                    case "quit_times":
                        self.values["quit_times"] = int(valnode)
                    case "nu":
                        self.values["nu"] = true
                    case _:
                        raise Exception("Please supply a value to the \"set\" property.")
            elif startnode.startswith("--"):
                continue
            else:
                try:
                    exec(self.conf[i])
                except Exception as e:
                    print("[ERROR]: " + e)
    
    def make_conf(self, to: str):
        with open(to, "w") as conf:
            for key in self.values.keys():
                conf.write(key + "  " + self.values[key])

conf = Config(getenv("HOME") + "/.vinerc")
conf.open_conf()
conf.read_conf()
conf.make_conf(getenv("HOME") + "/.vineconf")
