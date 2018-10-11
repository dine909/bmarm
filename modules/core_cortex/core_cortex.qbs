import qbs

Module {
    Depends { name: "cpp" }

    property string core: "none"

    cpp.driverFlags: [
        "-mcpu=cortex-"+core,
        "-mthumb",
    ]
    Properties {
        condition: core_cortex.core==="m4"
        cpp.defines:  [
            "CORE_M4",
        ]
    }
    Properties {
        condition: core_cortex.core==="m3"
        cpp.defines:  [
            "CORE_M3",
        ]
    }

}
