import qbs
import qbs.FileInfo

import "modules/teensy36/teensy36.qbs" as teensy36

Project {
    minimumQbsVersion: "1.7.1"
    name:"template"

    property bool isArm: qbs.targetPlatform===""

    teensy36 {
        name: "teensy36"
        condition: isArm
    }

    CppApplication {
        Depends {
            name: "teensy36"
            condition: project.isArm
        }
        type: ["application", "hex","bin","size", "elf", "disassembly"]
        cpp.positionIndependentCode: false


        property string mapName:  {return project.isArm?"-Map":"-map";}
        cpp.linkerFlags: [mapName,  product.destinationDirectory + "/" +product.name + ".map"]

        cpp.cxxLanguageVersion:"c++14"

        Group {
            name: "application"
            prefix: "src/"
            files: [
                "*.c",
                "*.C",
                "*.cpp",
                "*.h",
                "*.S",
            ]
        }
    }
}
