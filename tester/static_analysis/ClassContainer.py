#! /usr/bin/python
import os
import re

class ClassContainer:
    className = ""
    atributes = list()
    methods = list()
    camelCaseNoMatch = list()

    def __init__(self, fileName, template = False):
        # Cleanning up atributes
        self.className = ""
        self.atributes = list()
        self.methods = list()
        self.camelCaseNoMatch = list()
        flagVisibility = ""
        flagType = ""
        tempPath = ".clean.h"

        # If is template, ignore compiler cleanup
        if not template:
            # Remove all comments in code and create pre-processed file
            os.system("gcc -fpreprocessed -dD -E " + fileName + " > .clean.h")

            # Regex across multiple lines
            file = open(tempPath, "r+")
            buffer = file.read()
            # [.-;]
            buffer = re.sub("\([^;]*\)", "()", buffer, flags=re.DOTALL) # Delete (*)

            file.seek(0) # Restarting position
            file.truncate() # Eliminating all remaing
            file.write(buffer)
            file.close()

        else:
            # False pre-processed file
            os.system("cp " + fileName + " .clean.h")

        with open(tempPath, 'r') as f:
            lines = f.readlines()

            for line in lines:
                # Pre-clean
                line = line.replace(":", " ") # Change : by space if exists
                line = line.replace("{", " ") # Change { by space if exists
                line = line.strip() # Clean space and \n chars

                if line and not line.startswith(("#","}","enum","using")): # Empty string is false
                    if line.startswith("class") and line.endswith(";"):
                        # Ignore
                        continue

                    if line.startswith("class"):
                        self.className = line.split(" ")[1] # Class Name
                        continue

                    if line.startswith("public:") or line.startswith("public "):
                        flagVisibility = "public"
                        continue

                    if line.startswith("protected"):
                        flagVisibility = "protected"
                        continue

                    if line.startswith("private"):
                        flagVisibility = "private"
                        continue

                    # Detectin methods
                    if re.search("\(.*\)", line) is not None:
                        flagType = "method"
                    else:
                        flagType = "atribute"

                    # Staring cleaner
                    line = re.sub("~", " ~", line) # Space before ~
                    line = line.split("=")[0] # Eliminate inicializations
                    line = line.replace(";", "") # Delete ;
                    line = line.replace("*", " ") # substitute * with space
                    line = re.sub("\(.*\)", "", line) # Delete (*)
                    line = line.rstrip() # Eliminate final spaces
                    line = line.split(" ")[-1] # Only the name

                    if flagType == "method":
                        self.methods.append((flagVisibility, line))
                    else:
                        self.atributes.append((flagVisibility, line))

        try:
            os.remove(tempPath)
        except OSError:
            pass

    # Print the container information
    def printContainer(self):
        print("======== class ==========")
        print(self.className)

        print("======== atributes ==========")
        print(*self.atributes, sep="\n")

        print("======== methods ==========")
        print(*self.methods, sep="\n")

    # Count number of no camelCase mebers
    def falseCamelCaseCheck(self):
        noCCCounter = 0

        for visibility,name in self.atributes:
            if name[0].isupper():
                noCCCounter += 1
                self.camelCaseNoMatch.append(name)

        for visibility,name in self.methods:
            if (name[0].isupper() and name != self.className):
                noCCCounter += 1
                self.camelCaseNoMatch.append(name)

        return noCCCounter

    # Internal method. Compare two list of tuples.
    def _checkTowList(self, listCode, listTemplate, outFile=None):
        countExtraList = 0

        for vCode,nCode in listCode:
            flagIsIn = False
            for vTemplate,nTemplate in listTemplate:
                if vTemplate == vCode and (nTemplate == nCode or nTemplate == "ALL"):
                    flagIsIn = True

            if not flagIsIn:
                if outFile is not None:
                    outFile.write("    " + vCode + " " + nCode + "\n")
                countExtraList += 1

        if countExtraList == 0 and outFile is not None:
            outFile.write("    OK\n")

        return countExtraList

    # Compare the current class vs a template container, return the count of diferences
    def checkWithTemplate(self, templateContainer, attributes=True, methods=True, outFile=None):
        if not self.className == templateContainer.className:
            print ("Error: Template class name doesn't match with code class name: " + self.className + " != " +templateContainer.className)
            return False

        counter = 0 
        if attributes:
            if outFile is not None:
                outFile.write("  Atributes:\n")
            counter += self._checkTowList(self.atributes, templateContainer.atributes,outFile)
        if methods:
            if outFile is not None:
                outFile.write("  Methods:\n")
            counter += self._checkTowList(self.methods, templateContainer.methods,outFile)

        return counter
