#! /usr/bin/python
from ClassContainer import ClassContainer
import os
import sys
import re

def checkUnusedPrivateParameter(pathCode, logFile):
    exectutablePath = "exe"
    outputPath = "{0}/.unusedParameter".format(pathCode)
    result = os.popen("clang++ {0}/*.cpp -o {1} -std=c++11 -Wunused-private-field \
                      &> {2}".format(pathCode, exectutablePath, outputPath)).read()
    logFile.write("<span class=\"shj_b\">Unused private parameter\n")
    numOfUnused = 0
    with open(outputPath, "r") as file:
        for line in file:
            if line.find("-Wunused-private-field") > -1:
                numOfUnused += 1
                unusedParameter = line.split()[-5]
                unusedParameterFile = line.split()[0].split('/')[-1]
                logFile.write("<span class=\"shj_r\">" + pathCode + ": Unused parameter " + unusedParameter + " in " + \
                      unusedParameterFile)

        if numOfUnused == 0:
            logFile.write("<span class=\"shj_g\">ACCEPT")

    try:
        os.remove(outputPath)
    except OSError:
        pass

    try:
        os.remove(exectutablePath)
    except OSError:
        pass

    return numOfUnused

def checkBadPractices(pathCode, logFile):
    outputPath = "{0}/.badPractices".format(pathCode)
    result = os.popen("cppcheck --suppressions-list=suppressFile.txt --enable=style \
                      --output-file={1} {0}".format(pathCode,outputPath)).read()

    logFile.write("\n<span class=\"shj_b\">Bad programming practices\n")
    linesNumber = 0
    with open(outputPath, "r") as file:
        for line in file:
            linesNumber += 1
            logFile.write("<span class=\"shj_r\">" + line)

        if linesNumber == 0:
            logFile.write("<span class=\"shj_g\">ACCEPT")

    try:
        os.remove(outputPath)
    except OSError:
        pass

    return linesNumber

def checkDuplicateLines(pathCode, logFile, threshold):
    outputPath = "{0}/.duplicatedLines".format(pathCode)
    result = os.popen('java -jar simian-2.5.10.jar {0}/*.cpp -threshold={2} &> {1}'\
                      .format(pathCode, outputPath, threshold)).read()

    logFile.write("\n<span class=\"shj_b\">Duplicated code\n")
    numDuplicated = 0
    with open("{0}/.duplicatedLines".format(pathCode), "r") as file:
        for line in file:
            if line.find("duplicate lines in the following files:") > -1:
                numDuplicated += 1
                logFile.write("<span class=\"shj_r\">==> " + line)
            elif line.find("Between") > -1:
                fileName = line.split()[-1].split('/')[-1]
                lineTrimed = ' '.join(line.split()[0:-1])
                logFile.write("<span class=\"shj_r\">" + lineTrimed + " " + fileName + "\n")

        if numDuplicated == 0:
            logFile.write("<span class=\"shj_g\">ACCEPT\n")

    try:
        os.remove(outputPath)
    except OSError:
        pass

    return numDuplicated

def main():
    if len(sys.argv) > 1:
        pathCodes = sys.argv[1]
        pathTemplate = sys.argv[2]
        pathResult = sys.argv[3]
    else:
        print("Error, bad path names")
        return
    
    with open("./IgnoreFileList.txt") as ignoreFile:
        ignoreFilesList = ignoreFile.readlines()

    ignoreFilesList = [x.strip() for x in ignoreFilesList] 

    with open(pathResult + "/static-errors", "w") as outputFile:
    
        pathCode = pathCodes 

        listFiles = os.listdir(pathCode)

        # All ending in .h and without dot in the begining
        patternHeader = re.compile("[^.].*\.h")
        listHeaders = [s for s in listFiles if patternHeader.match(s) != None]

        totalCCError = 0
        totalExtraMethods = 0
        totalExtraAttributes = 0
        totalExtraClases = 0
        extraClases = []
        camelCaseAllClass = list()

        # Log Outputs --------------------------------------------------------------
        with open("{0}/static-result.html".format(pathResult), "w") as logFile:

            logFile.write("\n\n<span class=\"shj_b\">STATIC ANALYSIS:\n")
            logFile.write("\n<span class=\"shj_b\">Extra public members\n")
            numPublicMembers = 0
            for file in listHeaders:

                fileNameHeader = pathCode + "/" + file
                fileNameTemplate = pathTemplate + "/" +  file[0:-2] + ".template"

                if (os.path.exists(fileNameTemplate)):

                    if file not in ignoreFilesList:
                        
                        containerCode = ClassContainer(fileNameHeader)
                        containerTemplate = ClassContainer(fileNameTemplate, True)

                        logFile.write("\n" + file + "\n")
                        numPublicMembers += 1
                        # print(directory + " =======> " + file)
                        totalCCError += containerCode.falseCamelCaseCheck()
                        for memberName in containerCode.camelCaseNoMatch:
                            camelCaseAllClass.append((containerCode.className,memberName)) 

                        totalExtraMethods += containerCode.checkWithTemplate(containerTemplate, \
                                                                            attributes = False, \
                                                                            methods = True, \
                                                                            outFile = logFile)
                        totalExtraAttributes += containerCode.checkWithTemplate(containerTemplate, \
                                                                                attributes = True, \
                                                                                methods = False, \
                                                                                outFile = logFile)

                else:
                    extraClases.append(file)
                    totalExtraClases += 1
                    continue

            if (numPublicMembers == 0):
                logFile.write("<span class=\"shj_g\">ACCEPT")

            logFile.write("\n<span class=\"shj_b\">Extra Classes\n")
            if len(extraClases) > 0:
                logFile.write("<span class=\"shj_r\">" + "\n".join(extraClases))
            else:
                logFile.write("<span class=\"shj_g\">ACCEPT\n")

            logFile.write("\n<span class=\"shj_b\">No Camel Case\n")
            if len(camelCaseAllClass) > 0:
                for className,memberName in camelCaseAllClass:
                    logFile.write("<span class=\"shj_r\">" + str(className) + " " + str(memberName) + "\n")
            else:
                logFile.write("<span class=\"shj_g\">ACCEPT\n")

            totalUnusedVars = checkUnusedPrivateParameter(pathCode, logFile)
            totalBadPractices = checkBadPractices(pathCode, logFile)
            totalDuplicatedLines = checkDuplicateLines(pathCode, logFile, threshold=9)

        # Write output file
        outputFile.write(str(totalCCError) + "," + \
                            str(totalExtraMethods) + "," + \
                            str(totalExtraAttributes) + "," + \
                            str(totalUnusedVars) + "," + \
                            str(totalExtraClases) + "," + \
                            str(totalBadPractices) + "," + \
                            str(totalDuplicatedLines) + " \n")

if __name__ == "__main__":
    main()
