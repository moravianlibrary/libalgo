#include "File.h"

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <ctype.h>

#include "libalgo/source/const/Const.h"
#include "libalgo/source/exceptions/ErrorBadData.h"
#include "libalgo/source/exceptions/ErrorFileRead.h"

TFileLines File::loadFileToLines(const char * file_name)
{
        //Load lines from text file
        char buffer[BUFF];
        FILE * file = NULL;
        TFileLines file_lines;

        //Try to open file
        file = fopen(file_name, "r");

        if (file != NULL)
        {
                //Process all lines
                for (; fgets(buffer, BUFF, file);)
                {
                        //Remove empty lines
                        bool empty_line = true;

                        for (char * c = buffer; *c != '\0'; c++)
                        {
                                //Are there other chars?
                                if (!isspace((char) *c))
                                {
                                        //Line not empty
                                        empty_line = false;
                                        break;
                                }
                        }

                        //Do not add an empty line
                        if (empty_line) continue;

                        //Throw /r or /n: create a raw line without line terminators
                        char * raw_line = strtok(buffer, "\r\n");

                        //Change separator to "."
                        char * separator = strchr(raw_line, ',');

                        if (separator) *separator = '.';

                        //Add non empty line to the list
                        file_lines.push_back(raw_line);
                }

                //Close file
                fclose(file);
        }

        //File could not be opened
        else
        {
                //Throw exception
                throw ErrorFileRead("ErrorFileRead: can not open file. ", file_name);
        }

        //Return content of the file
        return file_lines;
}

TFileWords File::loadFileToWords(const char * file_name)
{
        //Load items from text file
        unsigned int lines = 0;
        char buffer[BUFF];
        FILE * file;
        TFileWords file_words;

        //Try to open file
        file = fopen(file_name, "r");

        if (file != NULL)
        {
                //Process all lines
                for (; fgets(buffer, BUFF, file);)
                {
                        //Remove empty lines
                        bool empty_line = true;

                        for (char * c = buffer; *c != '\0'; c++)
                        {
                                //Are there other chars?
                                if (!isspace((char) *c))
                                {
                                        //Line not empty
                                        empty_line = false;
                                        break;
                                }
                        }

                        //Do not add an empty line
                        if (empty_line) continue;

                        //Resize words
                        file_words.resize(lines + 1);

                        //Delimit line to words
                        char * word = strtok(buffer, " \t;\r\n");

                        //Process each line
                        for (int i = 0; word; i++, word = strtok(0, " \t;\r\n"))
                        {
                                //Change separator to "."
                                char * separator = strchr(word, ',');

                                if (separator) *separator = '.';

                                //Add word to the list
                                file_words[lines].push_back(word);
                        }

                        //Increment lines
                        lines++;
                }

                //Close file
                fclose(file);
        }

       //File could not be opened
        else
        {
                //Throw exception
                throw ErrorFileRead("ErrorFileRead: can not open file. ", file_name);
        }

        //Return content of the file
        return file_words;
}

void File::commaToDot(char ** text)
{
        //Change decimal separator: replace , with .
        char * separator = strchr(*text, ',');

        if (separator) *separator = '.';

        //for ( ;**text != '\0' &&  **text != ','; *(text++) );
        //if (**text == ',')
        //	**text ='.';
}


