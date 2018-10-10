import qbs
import qbs.FileInfo

Module{
    Depends { name: "cpp" }
    Depends { name: "chip_18xx_43xx" }

    chip_18xx_43xx.chip: "LPC4088"


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
