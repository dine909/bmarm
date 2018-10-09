import qbs
import qbs.FileInfo
import "lib/mk66f18/mk66f18.qbs" as Sdk

Project {

    property bool USE_USB_device: false
    property bool USE_USB_host: false
    property bool USE_USB_otg: false

    property pathList externalIncludes: []

    Sdk {
        USB_device : USE_USB_device
        USB_host   : USE_USB_host
        USB_otg    : USE_USB_otg
        externalIncludes: project.externalIncludes
    }

    StaticLibrary{
        name:"teensy36"
        Depends { name: "cpp" }
        cpp.positionIndependentCode: false

        Export{
            Depends { name: "cpp" }
            Depends { name: "mk66f18" }
            Depends { name: "usb" }

            cpp.includePaths: [
                "board/src",
                "board",
            ]

            Group {
                name: "board"
                prefix: "board/"
                files: [
                    "*/*.c",      "*.c",
                    "*/*.cpp",    "*.cpp",
                    "*/*.h",      "*.h",
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
    }




}
