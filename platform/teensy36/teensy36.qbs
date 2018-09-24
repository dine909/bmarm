import qbs
import qbs.FileInfo
//import "lib.qbs" as lib


Product {

//    qbsSearchPaths: ["../../lib//teensy36"]
    //    qbsSearchPaths: ["platform","libs/mk66f18/mk66f18"]

    //        targetName: targetFile
    consoleApplication: true
    cpp.positionIndependentCode: false

    cpp.includePaths: [
        "platform/teensy36/board",
        "platform/teensy36/board/src"
    ]

    Group {
        name: "board"
        prefix: "board/"
        files: [
            "*/*.c","*.c",
            "*/*.cpp","*.cpp",
            "*/*.h","*.h",
        ]
    }

    Depends { name: "cpp" }
    Depends { name: "mk66f18" }

//    Depends { name: "teensy36" }
    type: ["application", "hex","bin","size", "elf", "disassembly"]



    Properties {
        condition: qbs.buildVariant === "debug"
        cpp.optimization: "debug"
        cpp.debugInformation: true
    }

    Properties {
        condition: qbs.buildVariant === "release"
        cpp.optimization: "fast"
        cpp.debugInformation: false
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
