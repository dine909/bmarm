import qbs

Module {
    Depends { name: "cpp" }
    name: "fp"
    property string fp: "none"

    Properties {
        condition: core_cortex_fp.fp==="hard"
        cpp.defines:  [
            "CORE_M4",
        ]
        cpp.driverFlags: [
            "-mfloat-abi=hard",
            "-mfpu=fpv4-sp-d16",
        ]
    }
    Properties {
        condition: core_cortex_fp.fp==="soft"
        cpp.defines:  [
            "CORE_M3",
        ]
        cpp.driverFlags: [
            "-mfloat-abi=softfp",
            "-mfpu=fpv4-sp-d16",
        ]
    }

}
