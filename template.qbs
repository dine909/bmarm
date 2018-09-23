import qbs
import "platform/teensy36/teensy36.qbs" as teensyApplication

Project {
    teensyApplication {
        condition: profile.contains("Teensy36")
        consoleApplication: true
        type: ["application", "hex","bin","size", "elf", "disassembly"]

         property string targetFile  : "template_0_1"

         Group {
             name: "application"
             prefix: "src/"
             files: [
                 "*.c",
                 "*.cpp",
                 "*.h",
             ]

         }

    }

//    CppApplication {
//        consoleApplication: true

//        Group {     // Properties for the produced executable
//            fileTagsFilter: product.type
//            qbs.install: false
//        }




        //        Group {
        //            name: "board"
        //            condition: profile.contains("Teensy36")
        //            prefix: "platform/teensy36/board/"
        //            files: [
        //                "*/*.c","*.c",
        //                "*/*.cpp","*.cpp",
        //                "*/*.h","*.h",
        //            ]
        //            cpp.includePaths: [cpp.includePaths,"platform/teensy36/board",
        //                                            "platform/teensy36/board/src",
        //                                           ]
        //        }

//    }

}
