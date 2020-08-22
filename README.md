# Colour Palette Creator

A C++ program for generating a colour palette based off a user supplied image.

Uses stb_image.h and stb_image_write.h created by Sean T. Barrett and licenced under the public domain.

### Usage
1. When prompted enter the full filepath to the image you wish to edit
(e.g. C:\\User\\Your name\\Pictures\\image.jpg )
Note program will not work if there is a space in the filepath at the moment
2. Enter the palette size when prompted, this is how many colours you want in the palette
Program will create a file called output.jpg in the directory where the program is stored.

### Todo

* Add error handling for file names
* Allow custom save locations for output file
* Improve performance
* Add a file selection gui?
* Fix issue with spaces in filepath

Created by Tom England, 2020
