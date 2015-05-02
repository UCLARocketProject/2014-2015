#include <SD.h>
#include <SPI.h>

// This must be less than 8 characters (9 bytes)
// Extra byte for being such a good programmer
const int fileNameSize = 10;
char strFileName[fileNameSize];

void setup() {
  
  //this is needed for the duemilanove and uno without ethernet shield
  const int sdCardPin = 10;
  int loopCount = 0;

  //for testing. delete before launches.
  Serial.begin(9600);
  delay(1000);
  pinMode(10,OUTPUT);
  
  if (!SD.begin(sdCardPin)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return; 
  }
  Serial.println("card initialized.");
  getNewFileName();
}

void loop() {
  String test;
  test = "Kai";
  sdWrite(test);
}

/*
gives a new filename so we can open a new file each time
no failure code, defaults to run0fv.txt
*/
void getNewFileName() {
  Serial.println("started to get filename");
  strncpy(strFileName, "run0.txt", fileNameSize);
  int nameSize = 0;
  for(int i = 0; SD.exists(strFileName); nameSize = sprintf(strFileName, "run%d.txt", i)) {
    //catch potential overflow with the name size then we have an issue.
    //so we write to the file "runOvf.txt"
    Serial.print("checking filename ");
    Serial.println(strFileName);
    if(nameSize > 12) {
      strncpy(strFileName, "runOfv.txt", fileNameSize);
      return;
    }
   i++;
  }
  Serial.println("Got filename");
  return;
}

/*
Filename MUST be <=8 characters (not including the file extension) or the
file will not be created
*/
bool sdWrite(String data) {
  File dataFile = SD.open(strFileName, FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(data);
    dataFile.close();
    // print to the serial port too:
    Serial.println(data);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening file");
    dataFile.close();
  }
}


