import qbs
import qbs.FileInfo
import qbs.TextFile

Module {

    property stringList memory: []
    property stringList origin: []

    Rule {
        name: "chipGen"
        inputs: []
        multiplex: true
        alwaysRun: true

        Artifact {
            filePath: "chip_linker.ld"
            fileTags: ["orderedLinkerScript_chip"]
        }
        prepare: {
            var cmd = new JavaScriptCommand();
            cmd.description =  "generating chip variant";
            cmd.highlight = "chip_generation";
            cmd.sourceCode = function() {
                var ofile = new TextFile(output.filePath, TextFile.WriteOnly);
                ofile.write("/* Linker script to configure memory regions. */       \n");
                ofile.write("MEMORY                                                 \n");
                ofile.write("{                                                      \n");
                for(var o=0;o<product.chipGen.origin.length;o+=3){
                    var lo=function(){for(var p=0;p<product.chipGen.memory.length;p++){if(product.chipGen.memory[p]===product.chipGen.origin[o]){return p; } }; return -1 }();
                    if(lo!=-1)
                    {
                        ofile.write("\t" + product.chipGen.origin[o].toUpperCase() + "\t(");
                        ofile.write(product.chipGen.origin[o+1].toLowerCase() + ") :\tORIGIN = ");
                        ofile.write(product.chipGen.origin[o+2].toLowerCase() + ",\t LENGTH = ");
                        ofile.write(product.chipGen.memory[lo+1].toLowerCase() + "\n");
                    }
                }

                ofile.write("}                                                      \n");
                ofile.close();
            }
            return cmd;
        }
    }
}
