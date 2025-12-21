In order to use the program you must download all files from "codefinal", download "c-wildwater.sh" and download "Makefile" and put them in the same folder, next from your you must execute the shell file by typing:
./[adress of the shell file] [adress of the data file] [action] [parameter]
typing -h in the action argument will display this:

Usage: ./c-wildwater.sh [csv_file] [action] [parameter]
Actions:
  histo: Statistics (parameters: max, src, real, all)
  leaks: Leak detection (parameter: Station identifier)
  -h: Display this help

The four possible parameters of histo will create a csv file and two png files.
  max generates: 
    Csv file in reverse alphabetical order that contains every plant's maximun water capacity.
    Png file that contains the 10 plants with the biggest water capacity.
    Png file that contains the 50 plants with the smallest water capacity.

  src generates:
    Csv file in reverse alphabetical order that contains every plant's collected water volume.
    Png file that contains the 10 plants with the most collected water.
    Png file that contains the 50 plants with the least collected water.

  real generates:
    Csv file in reverse alphabetical order that contains every plant's treated water volume.
    Png file that contains the 10 plants with the most treated water.
    Png file that contains the 50 plants with the least treated water.   

  src generates:
    Csv file in reverse alphabetical order that contains every plant's collected water volume.
    Png file that contains the 10 plants with the most collected water.
    Png file that contains the 50 plants with the least collected water.

  all generates:
    Csv file in reverse alphabetical order that contains every plant's maximun, collected and treated water volume.
    Png file that contains the 10 plants with the biggest volume of treated water.
    Png file that contains the 50 plants with the smallest volume of treated water.

leaks generates:
  If the leaks.csv already exits it will add another line containing the leaked volume, biggest leaked section, the parent of the biggest leaked section and the leak in that section.
  else it will create the file leaks.csv and do what was said above.

(In order to use the leaks command the plant's identifier must be between quotation marks).
