import qbs
import qbs.FileInfo

Module{
    Depends { name: "cpp" }
    Depends { name: "orderedLinker" }



    //            type: ["application", "hex","bin","size", "elf", "disassembly"]
    cpp.includePaths: [
        path+"/include",
    ]
    Group {
        name: "Test Includes"
        files: [
            "include/*.h",
        ]
    }
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
