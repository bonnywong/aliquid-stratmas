//$Id: file2h.c,v 1.2 2006/07/21 13:35:30 dah Exp $

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

// Simple application to generate char[] array definitions of files.
// Usage: file2h outfile.h file1.ext, file2.ext, ...
// Will generate variables on the form:
// char file1[] = {0x10, 0x30, ...

const char* progname = "file2h";

int main(int argc, char** argv)
{
     FILE* coutfile;
     FILE* houtfile;
     FILE* cur_infile;
     int i;
     int c;
     int bytecount = 0;
     char* preamble;
     char* postamble;


     if (argc > 0) { 
          progname = argv[0];
     }

     if (argc < 4) {
          fprintf(stderr, 
                  "%s: Usage: file2h outfile.cpp outfile.h file1.ext ...\n", 
                  progname);
          exit(1);
     }

     coutfile = fopen(argv[1], "wb");
     if (coutfile == NULL) {
          perror(progname);
          printf("Unable to find %s\n", argv[1]);
          exit(1);
     }

     houtfile = fopen(argv[2], "wb");
     if (houtfile == NULL) {
          perror(progname);
          printf("Unable to find %s\n", argv[2]);
          exit(1);
     }

     if (fprintf(houtfile, 
                 "/* %s was generated by %s \n"
                 " * for use with %s\n"
                 " */\n",
                 argv[2], progname, argv[1]) < 0) {
          perror(progname);
          exit(1);
     }

     if (fprintf(coutfile,
                 "/* %s was generated by %s \n"
                 " * To use, include %s\n"
                 " */\n",
                 argv[1], progname, argv[2]) < 0) {
          perror(progname);
          exit(1);
     }

     // Define lookup function;
     if (fprintf(coutfile,
                 "#include <cstring>\n"
                 "\n"
                 "const char* const file2h_lookup_table[%d] = {\n",
                 2*(argc - 3) + 1) < 0) {
          perror(progname);
          exit(1);
     }
  
     for (i = 3; i < argc; i++) {
          printf("Processing file %s\n", argv[i]);
          bytecount = 0;
          cur_infile = fopen(argv[i], "rb");
          if (cur_infile == NULL) {
               perror(progname);        
                   printf("Unable to find %s\n", argv[i]);
               exit(1);
          } else {
               const char* lookupName =
                   fs::path(argv[i]).filename().generic_string().c_str();
               if (fprintf(coutfile, 
                           "\t/* From %s */\n"
                           "\t\"%s\",\n"
                           "",
                           argv[i], lookupName) < 0) {
                    perror(progname);
                    exit(1);
               }

               bytecount = 0;
               while ((c = fgetc(cur_infile)) != EOF) {
                    if (bytecount % 8 == 0) {
                         preamble = "\t\"";
                    } else {
                         preamble = "";
                    }

                    if (bytecount % 8 == 7) {
                         postamble = "\"\n";
                    } else {
                         postamble = "";
                    }

                    if (fprintf(coutfile, "%s\\%03o%s", preamble,
                                (unsigned char) c, postamble) < 0) {
                         perror(progname);
                         exit(1);
                    }

                    bytecount++;
               }
               if (bytecount % 8 != 0) {
                    postamble = "\",\n";
               } else {
                    postamble = "\t,\n";
               }
               if (fprintf(coutfile, "%s", postamble) < 0) {
                    perror(progname);        
                    exit(1);
               }
        
               if (fclose(cur_infile) != 0) {
                    perror(progname);        
                    exit(1);
               }
          }
     }
  
     if (fprintf(coutfile, "\t0\n};\n") < 0) {
          perror(progname);        
          exit(1);
     }

     if (fprintf(coutfile, "%s",
                 "\n"
                 "const char* const file2h_lookup(const char* const file)\n"
                 "{\n"
                 "\tfor (int i = 0; file2h_lookup_table[i] != 0; i+=2) {\n"
                 "\t\tif (strcmp(file, file2h_lookup_table[i]) == 0) {\n"
                 "\t\t\treturn file2h_lookup_table[i+1];\n"
                 "\t\t}\n"
                 "\t}\n"
                 "\treturn 0;\n"
                 "}\n"
                 "\n") < 0) {
          perror(progname);
          exit(1);
     }

     if (fclose(coutfile) != 0) {
          perror(progname);        
          exit(1);
     }

     if (fprintf(houtfile, "%s",
                 "\n"
                 "const char* const file2h_lookup(const char* const file);\n") < 0) {
          perror(progname);
          exit(1);
     }

     if (fclose(houtfile) != 0) {
          perror(progname);        
          exit(1);
     }

     return 0;
}