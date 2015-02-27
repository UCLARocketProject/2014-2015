"""
ExcelLoader v1.0

Loads data from a text file into an excel sheet

Written by Alex Omar
2/27/2015
"""
try:
    import xlsxwriter
except:
    print "You need to download the xlsxwriter to interface with Excel. You can download it from https://xlsxwriter.readthedocs.org/index.html"
import re
import sys
import os

class excelLoader:

   

    #takes a filename and returns an open workbook to that sheet
    def initExcel(self, filename):
        if filename[-4:-1]!='xlsx':
            #maxsplit = 1 so I only split on period
            match = re.split('\.', filename, 1)
            filename = match[0] + ".xlsx"
        return xlsxwriter.Workbook(filename)

    #creates a new worksheet and loads it into the mWorksheets{}
    def getNewWorksheet(self, name):
        try:
            self.mWorksheets[name] = self.mWorkbook.add_worksheet(name)
        except:
            print "worksheet creation error. Worksheet might already exist"

    #finds data and prints it to the excel sheet
    def getData(self):
        accelFile = self.getFileData()
        for line in accelFile:
            try:
                self.dataList.append(re.split(', ', line))
            except:
                print "data reading error."
        accelFile.close()

    #writes the data from self.dataList to the excel file
    def writeData(self):
        row = 0
        for dataRow in self.dataList:
            col = 0
            for dataCol in dataRow:
                self.mWorksheets[self.defaultSheet].write(row, col, self.dataList[row][col])
                print self.dataList[row][col]
                col += 1
            row += 1

    #close the workbook
    def finish(self):
        self.mWorkbook.close()

    #returns an open file
    def getFileData(self):
        return open(self.TEXTFILE, 'r')
        
    def __init__(self, textfile, spreadsheet):
        self.TEXTFILE = textfile
        self.SPREADSHEET = spreadsheet
        self.defaultSheet = "Sheet1"
        self.mWorksheets = {}
        self.dataList = []
        self.getData()
        
        self.mWorkbook = self.initExcel(self.SPREADSHEET)
        #a new worksheet will be loaded into mWorksheets dict
        self.getNewWorksheet(self.defaultSheet)
        self.writeData()
        self.finish()
        
if __name__ == '__main__':
    badArgs = 0
    EXCEL = "C:\\Users\\alex\\Documents\\GitHub\\2014-2015\\SensorData\\testSpreadsheet1.xlsx"
    TEXTFILE = "C:\\Users\\alex\\Documents\\GitHub\\2014-2015\\SensorData\\RUN2-26.TXT"
    args = sys.argv
    if len(args) == 1:
        print "Missing Arguments."
        print "Usage: excelLoader.py [option] ... [-default | -c [source] [sink]| -d]"
        print "Try excelLoader.py -help for more information."
        badArgs = 1
    #command line and argument descriptions
    elif args[1] == "-help":
        print("Options:\n" +
              "-default : Runs the program on default settings with default arguments.\n" +
              "        This is used for debugging. It uses hardcoded filepaths\n" +
              "        and is not recommended.\n" +
              "-c [src] [sink]  : Set a custom data source (.txt) and data sink (.xlsx).\n" +
              "        Ex: \"-c C:\\Users\\Alex\\data.txt C:\\Users\\Alex\\Documents\\myExcel.xlsx\n" + 
              "-d    : Setting [sink] to \"%FILENAME% -d\" will set the path to Desktop.\n" +
              "        Ex: \"myExcelFile.xlsx -d\" (Mac and Windows only)\n" +
              "-j    : Tells a joke")
        badArgs = 1
    elif args[1] == "-default":
        pass
    elif args[1] == "-j":
        print "A common mistake people make when trying to design something completely foolproof is to underestimate the ingenuity of complete fools. - Douglas Adams"
        badArgs = 1
    #if they passed in -c get the source and sink
    elif args[1] == "-c":
        try:
            TEXTFILE = args[2]
        except:
            print "You need to pass a [src] for the data to read."
        try:
            if(args[3] == "-d"):
                print "You need to pass an Excel filename before -d"
                badArgs = 1
            EXCEL = args[3]
        except:
            print "you need to pass in a [sink]"
            badArgs = 1
        #if they used option -d
        try:
            if(arg[4] == '-d'):
                EXCEL = os.path.join(os.path.expanduser('~'), 'Desktop') + args[3]
        except:
            #they didnt pass another argument
            pass
    else:
        print "bad args"
        badArgs = 1
    #if the user didnt screw up then run the program
    if badArgs == 0:
        loader = excelLoader(TEXTFILE, EXCEL)
