#include "main.h"

/*
    Main function for the assembly program.
    @param argc: The number of command-line arguments.
    @param argv: Array of command-line arguments.
    @return: Returns 0 upon successful execution.
*/
int main(int argc, char *argv[])
{
    int z;
    int j;
    int result;
    int IC;
    int DC;
    char originalFileName[MAX_LINE_LENGTH];    /* Will contain the file name without .as*/
    char logFileName[MAX_LINE_LENGTH];    /* Will contain the file name of the log*/
    int num_of_files = 0;
    char* macrosNames; /*--------*/

    /*new list method*/
    Bits* head = NULL;
    variables* variavlesHead = NULL;
    variables* TempvariavlesHead = NULL;
    command commands[] =                /* An array of structs representing an action and its numerical data */
    {
        {"mov",  0, 2, {1, 1, 1, 1}, {1, 1, 1, 0}},
        {"cmp",  1, 2, {1, 1, 1, 1}, {1, 1, 1, 1}},
        {"add",  2, 2, {1, 1, 1, 1}, {1, 1, 1, 0}},
        {"sub",  3, 2, {1, 1, 1, 1}, {1, 1, 1, 0}},
        {"not",  4, 1, {0, 0, 0, 0}, {1, 1, 1, 0}},
        {"clr",  5, 1, {0, 0, 0, 0}, {1, 1, 1, 0}},
        {"lea",  6, 2, {0, 1, 1, 0}, {1, 1, 1, 0}},
        {"inc",  7, 1, {0, 0, 0, 0}, {1, 1, 1, 0}},
        {"dec",  8, 1, {0, 0, 0, 0}, {1, 1, 1, 0}},
        {"jmp",  9, 1, {0, 0, 0, 0}, {1, 0, 1, 0}},
        {"bne",  10, 1, {0, 0, 0, 0}, {1, 0, 1, 0}},
        {"red",  11, 1, {0, 0, 0, 0}, {1, 1, 1, 0}},
        {"prn",  12, 1, {0, 0, 0, 0}, {1, 1, 1, 1}},
        {"jsr",  13, 1, {0, 0, 0, 0}, {1, 0, 1, 0}},
        {"rts",  14, 0, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {"hlt", 15, 0, {0, 0, 0, 0}, {0, 0, 0, 0}}
    };
    if (argc < 2)
    {
        printf("Usage: %s filename1 filename2 filename3....\n", argv[0]);
        return 1;
    }
    num_of_files = argc - 1;
    macrosNames = (char*)malloc(0); /*----------*/
    for (z = 1; z <= num_of_files; z++)
    {
        printf("\n-----------%s (file number %d)---------\n",argv[z],z);
        /*macro*/
        if(main_macro(argv[z], macrosNames) == 1)
        {
            printf("Problems was found in the macro. Therefore, the program will not continue until you fix them.\n");
            continue;
        } /*---------*/
        /*end of macro*/
        /*first stage*/
        IC = DC = 0;
        variavlesHead = NULL;
        head = NULL;
        TempvariavlesHead = NULL;
        for (j = 0; j < 100; ++j)
        {
            insertBitsNode(&head, createNode(&IC, 0, 0, "",0));
        }
        printf("Reading %s:\n", argv[z]);
        result = preStageOne(&IC,&DC,"after_macro.am",commands,&head,&variavlesHead,&TempvariavlesHead, macrosNames);
        strcpy(originalFileName, argv[z]); /* Will copy the name of the current file */
        removeSubstring(originalFileName, ".as");
        strcpy(logFileName, originalFileName);
        strcat(logFileName, ".log");
        remove(logFileName); /*remove the log file if it was already exsist*/
        if (result != 0)
        {
            messageIssued("An errors was found in the code reading. Therefore, export file production will not continue until you fix them.\n");
        }
        else
        {
            stageTwo(head,commands,variavlesHead,TempvariavlesHead);
            printList(head);
            /*make output files*/            
            writeToFiles(head,TempvariavlesHead,originalFileName,&IC,&DC);
            printVariableList(TempvariavlesHead);
            printVariableList(variavlesHead);
        }
        rename("errors.log", strcat(originalFileName, ".log"));
        remove("after_macro.am");
        freeList(&head);
        freeVariableList(&variavlesHead);
        freeVariableList(&TempvariavlesHead);
    }
    return 0;
}