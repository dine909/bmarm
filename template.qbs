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

        cpp.cxxLanguageVersion:"c++14"
        cpp.cLanguageVersion:"c11"

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
