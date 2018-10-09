import qbs
import qbs.FileInfo

Project {

    StaticLibrary{
        name:"armbuild"
        Depends { name: "cpp" }
        cpp.positionIndependentCode: false

        Export{
            Depends { name: "cpp" }

            cpp.defines:  [
                "__QBS_ARMBUILD",
            ]

//            type: ["application", "hex","bin","size", "elf", "disassembly"]

            Rule {
                id: hex
                inputs: ["application"]
                prepare: {
                    var args = ["-O", "ihex", input.filePath, output.filePath];
                    var cmd = new Command(FileInfo.path(product.cpp.compilerPath) + "/arm-none-eabi-objcopy", args);
                    cmd.description = "converting to hex: " + FileInfo.fileName(input.filePath);
                    cmd.highlight = "linker";
                    return cmd;

                }
                Artifact {
                    fileTags: ["hex"]
                    filePath: FileInfo.baseName(input.filePath) + ".hex"
                }
            }

            Rule {
                id: size
                inputs: "application"
                alwaysRun: true
                Artifact {
                    fileTags: "size"
                    filePath: "-"
                }
                prepare: {
                    var args = [input.filePath];
                    var cmd = new Command(FileInfo.path(product.cpp.compilerPath) + "/arm-none-eabi-size", args);
                    cmd.description = "File size: " + FileInfo.fileName(input.filePath);
                    cmd.highlight = "linker";
                    return cmd;
                }
            }
        }
    }




}
