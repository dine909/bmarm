import qbs
import qbs.FileInfo

Project {
    minimumQbsVersion: "1.7.1"
    name:"template"

    property bool isArm: qbs.targetPlatform.contains("board_")
    property string board: qbs.targetPlatform
//    Depends { name: "board" }

    CppApplication {
        Depends {
            name: project.board
            condition: project.isArm
        }

        type: ["application", "hex","bin","size", "elf", "disassembly"]


        property string mapName:  {return project.isArm?"-Map":"-map";}
        cpp.linkerFlags: [mapName,  product.destinationDirectory + "/" +product.name + ".map",]
        cpp.cxxLanguageVersion:"c++14"
        cpp.cLanguageVersion:"c11"
        cpp.positionIndependentCode: false

        Group {
            name: "Application "
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
