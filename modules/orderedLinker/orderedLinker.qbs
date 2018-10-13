import qbs
import qbs.FileInfo
import qbs.TextFile

Module{

    Rule {
        id: linkerGen
        inputs: [
            "orderedLinkerScript",
            "orderedLinkerScript_0",
            "orderedLinkerScript_chip",
            "orderedLinkerScript_chip_family",
            "orderedLinkerScript_board",
            "orderedLinkerScript_cmsis",
            "orderedLinkerScript_1",
            "orderedLinkerScript_2",
            "orderedLinkerScript_3",
            "orderedLinkerScript_4",
            "orderedLinkerScript_5",
        ]
                alwaysRun: true
        multiplex: true

        Artifact {
            fileTags: "linkerscript"
            filePath: FileInfo.joinPaths(product.buildDirectory,"linker.ld")
        }
        prepare: {
            var cmd = new JavaScriptCommand();
            cmd.description =  output.fileName;
            cmd.highlight = "linkergen";
            cmd.sourceCode = function() {
                var args = [];

                function concatif(x){
                    a=inputs[x];
                    if(a!=undefined){
                        args=args.concat(a)
                    }
                }

                concatif("orderedLinkerScript");
                concatif("orderedLinkerScript_0");
                concatif("orderedLinkerScript_chip");
                concatif("orderedLinkerScript_chip_family");
                concatif("orderedLinkerScript_board");
                concatif("orderedLinkerScript_cmsis");
                concatif("orderedLinkerScript_1");
                concatif("orderedLinkerScript_2");
                concatif("orderedLinkerScript_3");
                concatif("orderedLinkerScript_4");
                concatif("orderedLinkerScript_5");

                var ofile = new TextFile(output.filePath, TextFile.WriteOnly);
                for (f in args){
                    var file = new TextFile(args[f].filePath);
                    var content = file.readAll();
                    ofile.write("/* "+args[f].filePath+" */\n\n")
                    file.close()
                    ofile.write(content);
                }
                ofile.close();
            }
            return [cmd];
        }
    }

}
