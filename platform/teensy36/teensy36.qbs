import qbs
import qbs.FileInfo
import "../../lib/mk66f18/mk66f18.qbs" as Sdk

//import "lib.qbs" as lib

Project  {

    property string projectName : "template"
    property pathList projectFiles: []

    property bool USE_USB_device: false
    property bool USE_USB_host: false
    property bool USE_USB_otg: false


    Sdk {
        USB_device : USE_USB_device
        USB_host   : USE_USB_host
        USB_otg    : USE_USB_otg
    }

    Product {

        //    qbsSearchPaths: ["../../lib//teensy36"]
        //    qbsSearchPaths: ["platform","libs/mk66f18/mk66f18"]
        name: projectName
        property pathList appFiles: projectFiles

        //        targetName: targetFile
        consoleApplication: true
        cpp.positionIndependentCode: false
        cpp.linkerFlags: ["-Map=" + product.destinationDirectory + "/" +product.name + ".map"]
        cpp.includePaths: [
            "board",
            "board/src"
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
        Group {
            name: "application"
            prefix: "../../src/"
            files: appFiles
        }
        Depends { name: "cpp" }
        Depends { name: "mk66f18" }
        Depends { name: "usb"}
        Depends { name: "bin"}

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
            id: elf
            inputs: ["application"]
            prepare: {
                var args = [input.filePath, output.filePath];
                var cmd = new Command("cp", args);
                cmd.description = "copying elf: "+FileInfo.fileName(input.filePath);
                cmd.highlight = "linker";
                return cmd;

            }
            Artifact {
                fileTags: ["elf"]
                filePath: FileInfo.baseName(input.filePath) + ".elf"
            }
        }
        Rule {
            id: disassmbly
            inputs: ["application"]
            prepare: {
                var args = [input.filePath, '-D','-S'];
                var cmd = new Command(FileInfo.path(product.cpp.compilerPath) + "/arm-none-eabi-objdump", args);
                cmd.stdoutFilePath = output.filePath;
                cmd.description = "Disassembly listing for " + cmd.workingDirectory;
                cmd.highlight = "disassembler";
                cmd.silent = false;
                return cmd;

            }
            Artifact {
                fileTags: ["disassembly"]
                filePath: FileInfo.baseName(input.filePath) + ".lst"
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
        Group {
            name: "linker scripts"
            files: ["../../lib/mk66f18/MK66FX1M0xxx18_flash.ld"]
            fileTags: ["linkerscript"]
        }

    }
}
