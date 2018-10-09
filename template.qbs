import qbs
import qbs.FileInfo

//import "modules/vendor/freescale/mk66f18/mk66f18.qbs" as mculib
//import "modules/arm/cmsis/cmsis.qbs" as mculib

Project {
    minimumQbsVersion: "1.7.1"
    name:"template"

    property bool isArm: qbs.targetPlatform===""

//    mculib {
//        name: "mculib"
//        condition: isArm
//    }

    CppApplication {
        Depends {
            name: "board_teensy36"
            condition: project.isArm
        }

        type: ["application", "hex","bin","size", "elf", "disassembly"]
        cpp.positionIndependentCode: false


        property string mapName:  {return project.isArm?"-Map":"-map";}
        cpp.linkerFlags: [mapName,  product.destinationDirectory + "/" +product.name + ".map",]
        cpp.cxxLanguageVersion:"c++11"
        cpp.cLanguageVersion:"c11"

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
