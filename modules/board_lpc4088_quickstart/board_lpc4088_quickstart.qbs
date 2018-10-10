import qbs
import qbs.FileInfo

Module{
    Depends { name: "cpp" }

    Depends {
        name: "chip_18xx_43xx"
    }

    cpp.includePaths: [
        path+"/src",
    ]

    Group {
        name: "LPC4088 QuickStart Board"
        prefix: "src/"
        files: [
           "*.c",
           "*.cpp",
           "*.h",
        ]
    }
}
