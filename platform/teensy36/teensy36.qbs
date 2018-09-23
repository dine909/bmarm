import qbs
import qbs.FileInfo

Product
{
    name: "ARM GCC"
    Depends { name: "cpp" }
    qbs.architecture: "arm"
    cpp.architecture: "armv4t"
    targetName: targetFile
//    property string toolchainPathPrefix: cpp.compilerPath

    cpp.defines:["NDEBUG","CPU_MK66FX1M0VLQ18","TWR_K65F180M","TOWER"]
    cpp.assemblerFlags: ["-D__STARTUP_CLEAR_BSS", "-D__STARTUP_INITIALIZE_NONCACHEDATA"]

    cpp.warningLevel: 'all'
    cpp.includePaths: ["lib/mk66f18/CMSIS",
        "lib/mk66f18/drivers",
        "lib/mk66f18/startup",
        "lib/mk66f18/utilities",
        "platform/teensy36/board",
        "platform/teensy36/board/src"
    ].concat(base)

    Group {
        name: "linker scripts"
        files: ["../../lib/mk66f18/MK66FX1M0xxx18_flash.ld"]
        fileTags: ["linkerscript"]
    }

    Group {
        name: "sdk"
        prefix: "../../lib/mk66f18/"
        files: [
            "*/*.S",
            "*/*.c",
            "*/*.cpp",
            "*/*.h",
        ]
    }

    Group {
        name: "board"
        prefix: "board/"
        files: [
            "*/*.c","*.c",
            "*/*.cpp","*.cpp",
            "*/*.h","*.h",
        ]
    }


    Properties {

        condition: qbs.buildVariant === "debug"
        //        cpp.defines: commonDefines // else case for the Properties chain
        cpp.optimization: "debug"
        cpp.driverFlags: [
            "-mcpu=cortex-m4"   ,
            "--specs=nano.specs",
            "--specs=nosys.specs",
            "-mfloat-abi=hard",
            "-mfpu=fpv4-sp-d16",
            "-mthumb",
            "-mapcs",
            "-MMD",
            "-MP",
            "-fno-common",
            "-ffunction-sections",
            "-fdata-sections",
            "-ffreestanding",
            "-fno-builtin"
        ]

        cpp.commonCompilerFlags: [
            "-Wall",
            "-std=gnu99",
        ]
        cpp.debugInformation: true
    }
    cpp.linkerFlags: [

        //        "-T../../lib/mk66f18/MK66FX1M0xxx18_flash.ld",
    ]

    Rule {
        id: hex
        inputs: ["application"]
        prepare: {
            var args = ["-O", "ihex", input.filePath, output.filePath];
            var cmd = new Command("/opt/local/bin/arm-none-eabi-objcopy", args);
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
            var cmd = new Command("/opt/local/bin/arm-none-eabi-size", args);
            cmd.description = "File size: " + FileInfo.fileName(input.filePath);
            cmd.highlight = "linker";
            return cmd;
        }
    }

}
