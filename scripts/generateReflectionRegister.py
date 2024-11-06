import os
import re
import sys
import json
import argparse
import hashlib

from pathlib import Path
from abc import ABC, abstractmethod
from subprocess import Popen, PIPE, STDOUT, check_output


HEADER_EXT = ".hpp"
IMPL_EXT = ".cpp"
HASH_PATH = os.path.abspath(os.path.dirname(__file__)) + r"\reflection_hash"

C_ARG = "REFLECT_CLASS"
F_ARG = "REFLECT_METHOD"
P_ARG = "REFLECT_FIELD"
E_ARG = "REFLECT_ENUM"
Q_ARG = "REFLECT_CONSTRUCTOR"
M_ARG = "RTTR_ENABLE"

class ClassObject(object):

    def __init__(self):
        self.name = None
        self.parents = []
        self.reflectable = False

        self.private_properties = []
        self.public_properties = []
        self.protected_properties = []

        self.private_methods = []
        self.public_methods = []
        self.protected_methods = []

class ClassData(ABC):

    def __init__(self, _access=None, _data_type=None, _name=None):
        self.name = _name
        self.data_type = _data_type
        self.access = _access
        self.class_name = None

        self.generate_with_namespace = True

    def __str__(self) -> str:
        return f"{self.access} {self.data_type} {self.name}"

    def nspace(self):
        if self.generate_with_namespace:
            return "rttr::"
        return ""

    @abstractmethod
    def register_string(self):
        pass

class ClassProperty(ClassData):

    def __init__(self, _access=None, _data_type=None, _name=None):
        super(ClassProperty, self).__init__(_access, _data_type, _name)

    def register_string(self):
        return f'.property("{self.name}", &{self.class_name}, {self.nspace()}registration::{self.access}_access);'

class ClassMethod(ClassData):

    def __init__(self, _access=None, _data_type=None, _name=None):
        super().__init__(_access, _data_type, _name)

    def register_string(self):
        return f'.method("{self.name}", &{self.class_name}::{self.name}, {self.nspace()}registration::{self.access}_access);'

class Argument(ClassData):

    def __init__(self, _data_type=None, _name=None, _default_value=""):
        super().__init__(None, _data_type, _name)
        self.default_value = _default_value
    
    def __str__(self) -> str:
        return f'{self.data_type} {self.name} {self.default_value}'
    
    def register_string(self):
        return super().register_string()

class ClassConstructor(ClassData):

    def __init__(self, _access=None, _name=None, _args=[]):
        super().__init__(_access, None, _name)
        self.args = _args

    def __str__(self) -> str:
        return f'constructor of class {self.name} with args {self.args}'
    
    def register_string(self):
        return super().register_string()

def md5_hash_file(filename, hash):
    assert Path(filename).is_file()
    with open(str(filename), "rb") as f:
        for chunk in iter(lambda: f.read(4096), b""):
            hash.update(chunk)
    return hash

def md5_hash_dir(directory, hash):
    assert Path(directory).is_dir()
    for path in sorted(Path(directory).iterdir(), key=lambda p: str(p).lower()):
        hash.update(path.name.encode())
        if path.is_file():
            hash = md5_hash_file(path, hash)
        elif path.is_dir():
            hash = md5_hash_dir(path, hash)
    return hash

def md5_file(filename):
    return md5_hash_file(filename, hashlib.md5()).hexdigest()

def md5_dir(directory):
    return md5_hash_dir(directory, hashlib.md5()).hexdigest()

def parse_header_file(filename):
    executable = os.path.join(os.path.dirname(__file__), "header-parser.exe")
    command = [executable, f"{filename}", "-c", f"{C_ARG}", "-f", f"{F_ARG}", "-p", f"{P_ARG}", "-m", f"{M_ARG}", "-q", f"{Q_ARG}"]
    parsing_subprocess = Popen(command, stdout=PIPE, stdin=PIPE, stderr=PIPE, text=True)
    stdout_data = parsing_subprocess.communicate(input="data_to_write")[0]
    
    if stdout_data:
        return json.loads(stdout_data)
    return None

def read_class_data(header_dict):
        reflectable_classes = []

        for element in header_dict:
            if element.get("type") == "namespace":
                members = element.get("members")

                for member in members:
                    if member.get("type") == "class":
                        class_object = ClassObject()
                        class_object.name = member.get("name")

                        for parent in member.get("parents"):
                            pname = parent.get("name")
                            class_object.parents.append(pname.get("name"))
                        
                        for class_member in member.get("members"):
                            member_type = class_member.get("type")
                            access = class_member.get("access")
                            name = class_member.get("name")

                            if class_member.get("type") == "macro":
                                if name == "RTTR_ENABLE":
                                    class_object.reflectable = True
                                continue
                            
                            if member_type == "constructor":
                                arg_array = []
                                arguments = class_member.get("arguments")
                                for arg in arguments:
                                    arg_type = arg.get("type").get("name")
                                    arg_name = arg.get("name")
                                    arg_default = arg.get("defaultValue")
                                    arg_array.append(Argument(arg_type, arg_name, arg_default))

                                constructor = ClassConstructor(access, name, arg_array)

                            if member_type == "property":
                                data_type = class_member.get("dataType").get("name")

                                prop = ClassProperty(access, data_type, name)
                                prop.class_name = class_object.name
                                if access == "public":
                                    class_object.public_properties.append(prop)
                                elif access == "private":
                                    class_object.private_properties.append(prop)
                                else:
                                    class_object.protected_properties.append(prop)
                                continue

                            if member_type == "function":
                                print(class_member)
                                return_type = class_member.get("returnType").get("name")

                                method = ClassMethod(access, return_type, name)
                                method.class_name = class_object.name
                                if access == "public":
                                    class_object.public_methods.append(method)
                                elif access == "private":
                                    class_object.private_methods.append(method)
                                else:
                                    class_object.protected_methods.append(method)
                                continue
                        
                        if class_object.reflectable:
                            reflectable_classes.append(class_object)
                        else:
                            del class_object

        return reflectable_classes

def generate_registration_block(class_object:ClassObject):
    for prop in class_object.public_properties:
        pass

if __name__ == "__main__":
    include_dir = None
    source_dir = None

    ###################
    ## Parse CLI
    ###################

    parser = argparse.ArgumentParser(
        prog="generateReflectionInfo",
        description="This program generates reflection description of c++ classes",
        epilog="=================================================="
    )

    parser.add_argument(
        "-i", "--include", 
        help="Project include dir path",
        type=str
    )

    parser.add_argument(
        "-s", "--source", 
        help="Project source dir path",
        type=str
    )

    args = parser.parse_args()
    include_dir = args.include
    source_dir = args.source

    ###################
    ## Implement
    ###################

    if not os.path.exists(HASH_PATH):
        os.mkdir(HASH_PATH)

    need_to_generate = []
    for root, subfolders, files in os.walk(include_dir):
        current_path = root

        for dir in subfolders:
            full_path = os.path.join(root, dir)
            relative_path = Path(full_path).relative_to(include_dir)
            hash_path = os.path.join(HASH_PATH, relative_path)
            
            if not os.path.exists(hash_path):
                os.mkdir(hash_path)

        for file in files:
            filepath = os.path.join(root, file)
            if filepath.endswith(HEADER_EXT):
                relative_path = Path(filepath).relative_to(include_dir)
                source_path = os.path.join(source_dir, relative_path).replace(HEADER_EXT, IMPL_EXT)
                hash_path = os.path.join(HASH_PATH, relative_path) + ".hash"
                
                # Generate hash file if not already
                if not os.path.isfile(hash_path):
                    file = open(hash_path, "w")
                    filehash = md5_file(filepath)
                    file.write(filehash)
                    file.close()

                    header_dict = parse_header_file(filepath)
                    if header_dict:
                        need_to_generate = read_class_data(header_dict)

                else: # Generate new hash if file has been changed
                    file = open(hash_path, "r+")
                    data = file.read()
                    filehash = md5_file(filepath)

                    # Hashes not equal
                    if not filehash == data:
                        print(f"File {filepath} has changes, generating new hash...")
                        file.seek(0)
                        file.write(filehash)
                        file.truncate()
                        file.close()

                        header_dict = parse_header_file(filepath)
                        if header_dict:
                           need_to_generate = read_class_data(header_dict)

    for class_object in need_to_generate:
        generate_registration_block(class_object)

