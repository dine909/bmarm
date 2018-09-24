import qbs
import qbs.FileInfo

Project {
    Application {

        name : "template"
        //        targetName: targetFile
        consoleApplication: true
        cpp.positionIndependentCode: false


        Depends { name: "teensy36" }
        type: ["application", "hex","bin","size", "elf", "disassembly"]

        Group {
            name: "application"
            prefix: "src/"
            files: [
                "*.c",
                "*.cpp",
                "*.h",
            ]
        }
        Group {
            name: "linker scripts"
            files: ["lib/mk66f18/MK66FX1M0xxx18_flash.ld"]
            fileTags: ["linkerscript"]
        }
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

    StaticLibrary {
        name: "teensy36"
        Depends { name: "cpp" }
        Depends { name: "mk66f18" }

        cpp.includePaths: [
            "platform/teensy36/board",
            "platform/teensy36/board/src"
        ]

        Group {
            name: "board"
            prefix: "platform/teensy36/board/"
            files: [
                "*/*.c","*.c",
                "*/*.cpp","*.cpp",
                "*/*.h","*.h",
            ]
        }

        Export {
            Depends { name: "cpp" }
            cpp.includePaths: product.cpp.includePaths
            cpp.defines:product.cpp.defines
            cpp.assemblerFlags: product.cpp.assemblerFlags
            cpp.driverFlags: product.cpp.driverFlags
            cpp.commonCompilerFlags: product.cpp.commonCompilerFlags
//            cpp.architecture: product.cpp.architecture
            cpp.linkerFlags:product.cpp.linkerFlags


        }
    }
    StaticLibrary {
        name: "mk66f18"
        Depends { name: "cpp" }

//        qbs.architecture: "arm"
//        cpp.architecture: "armv4t"

        cpp.assemblerFlags: ["-D__STARTUP_CLEAR_BSS", "-D__STARTUP_INITIALIZE_NONCACHEDATA"]

        cpp.includePaths: ["lib/mk66f18/CMSIS",
            "lib/mk66f18/drivers",
            "lib/mk66f18/startup",
            "lib/mk66f18/utilities"
        ]

        cpp.defines:  ["NDEBUG","CPU_MK66FX1M0VLQ18","TWR_K65F180M","TOWER","DEBUG"]

        Group {
            name: "sdk"
            prefix: "lib/mk66f18/"
            files: [
                "*/*.S",
                "*/*.c",
                "*/*.cpp",
                "*/*.h",
            ]
        }

        cpp.driverFlags: [
            "-mcpu=cortex-m4"   ,
            "--specs=nano.specs",
            "--specs=nosys.specs",
            "-mfloat-abi=hard",
            "-mfpu=fpv4-sp-d16",
            "-fno-exceptions",
            "-mthumb",
            "-mapcs",
            "-MMD",
            "-MP",
            "-fno-common",
            "-ffunction-sections",
            "-fdata-sections",
            "-ffreestanding",
            "-fno-builtin",
            "-static"
        ]
        cpp.cxxFlags: [
            "-fno-rtti"
        ]


        cpp.cLanguageVersion: "c11"
        cpp.linkerFlags: ["--gc-sections", "-static", "-z", "muldefs","-lm","-lc","-lgcc","-lnosys"]

        Export {
            Depends { name: "cpp" }
            cpp.includePaths: product.cpp.includePaths
            cpp.defines:product.cpp.defines
            cpp.assemblerFlags: product.cpp.assemblerFlags
            cpp.driverFlags: product.cpp.driverFlags
            cpp.commonCompilerFlags: product.cpp.commonCompilerFlags
//            cpp.architecture: product.cpp.architecture
            cpp.linkerFlags:product.cpp.linkerFlags

        }
    }
}
