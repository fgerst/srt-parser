# srt_parser
Simple C program to modify and verify errors on SubRip subtitles

 *This is the final project of a C language seminar on university. It's a command line program that checks for usual errors on subtitles
in SubRip format (usually with a .srt extension).*

**In the SubRip format, the sequence of subtitles are formatted like this:**

![Captur2](https://user-images.githubusercontent.com/45186471/59172079-8ea47b00-8b1c-11e9-8413-71444b2002dc.JPG)

**Example of a SubRip subtitle:**

![Captura](https://user-images.githubusercontent.com/45186471/59171938-cfe85b00-8b1b-11e9-8772-64504a2e9d2f.JPG)


## **The parameters the program can receive:**

- -f "filein" ← file is the file name to process
- -o "fileout" ← fileout is the file name where changes will be saved
- -b index ← deletes the subtitle on index
- -i startmilis endmillis "text" ← inserts a new subtitle text, starting on startmilis 
   to endmilis
- -v ← validates the subtitle looking for the following errors:
  - index values are not ordered consecutive numbers
  - index numbers don't start on 1
  - subtitles are overlapped (subtitle i end_time is greater than subtitle i+1 start_time)
  - a subtitle lasts less than 1 second
  - a subtitle lasts more than 7 seconds
  - subtitle line too long (more than 36 characters)
  - too many characters per second (more than 25)
  - subtitle has more than 2 lines of text
  - less than 75 miliseconds between subtitle i end and subtitle i+1 start.

## **The project requirements and the way the parameters are processed:**
- f parameter is mandatory and cannot repeat
- o parameter is mandatory if the file is modified with other parameters (all except -v) and cannot repeat
- the rest of parameters are opcional, and all need to be processed in order
- parameters are received in any order
- if there are more than one -f or -o parameter a help message is displayed and the exit code is 1
- if a mandatory parameter is not received the previous message is displayed adn the exit code is 2
- if a invalid parameter is received the help message is displayed and exit code is 3
- if the file in or file out cannot be opened or saved the exit code is 4.

**An example of a program invocation would be like this**
> main -v -f "pine_barrens.srt" -o "pine_barrens_out.srt" -i 7000 8200 "Did you wrap the package?" -v -b 3 -v
