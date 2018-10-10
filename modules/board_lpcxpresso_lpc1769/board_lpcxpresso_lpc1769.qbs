import qbs
import qbs.FileInfo

Module{
    Depends { name: "cpp" }
    Depends { name: "chip_lpc17xx_lpc40xx" }

    chip_lpc17xx_lpc40xx.chip: "LPC1769"


    cpp.includePaths: [
        path+"/src",
    ]

    Group {
        name: "Board LPCXpresso LPC1769"
        prefix: "src/"
        files: [
           "*.c",
           "*.cpp",
           "*.h",
        ]
    }
}
