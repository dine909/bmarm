import qbs
import qbs.FileInfo

Module{
    Depends { name: "cpp" }
    Depends { name: "chip_lpc17xx_lpc40xx" }

    chip_lpc17xx_lpc40xx.chip: "LPC4088"


    cpp.includePaths: [
        path+"/src",
    ]

    Group {
        name: "Board LPC4088 QuickStart"
        prefix: "src/"
        files: [
           "*.c",
           "*.cpp",
           "*.h",
        ]
    }
}
