#include "main.h"

/*
    Searches for an external variable in a linked list of variables and updates its value if necessary.
    @param node: Pointer to a Bits structure containing the variable to search for.
    @param varsList: Pointer to the head of a linked list of variables.
*/
void writeToFiles(Bits* head, variables* varsList, const char *filename,int* IC,int* DC)
{
    writeVarsToFile(varsList, filename);
    writeExternalVarsToFile(varsList, filename);
    exportCodeFile(head,varsList,filename,IC,DC);
}

void searchForExtern(Bits *node, variables* varsList)
{
    while (varsList != NULL)
    {
        if(strcmp(node->sourceCode,varsList->variablesName) == 0)
        {
            if(varsList->typeOfUpdate == 2)
            {
                insertVariable(&varsList, createTempVariable(node->sourceCode, node->number, DATA, 3));
                changeBitsFromIndexToIndex(node,12,13,"01");
                break;
            }
        }
        varsList = varsList->next;
    }
}
/*
    Updates external variables in the temporary variables list with values from the global variables list.
    @param VarsList: Pointer to the head of the linked list containing global variables.
    @param TempVarsList: Pointer to the head of the linked list containing temporary variables.
*/
void variablesEntryExternal( variables* VarsList,  variables* TempVarsList)
{
     variables* tempHead = TempVarsList;
    while (VarsList != NULL)
    {
        while (TempVarsList != NULL)
        {
            if(strcmp(VarsList->variablesName,TempVarsList->variablesName) == 0)
            {
                if(TempVarsList->typeOfUpdate == 1)
                {
                    TempVarsList->variablesValue = VarsList->variablesValue;
                    VarsList->typeOfUpdate = ENTRY;
                }
                else
                {
                    VarsList->typeOfUpdate = EXTERNAL;
                }
            }
            TempVarsList = TempVarsList->next;
        }
        TempVarsList = tempHead;
        VarsList = VarsList->next;
    }
}
/*
    Writes external variables and their values to a file.
    @param varsList: Pointer to the head of the linked list containing variables.
    @param filename: Name of the file to write to.
*/
void writeExternalVarsToFile( variables* varsList, const char *filename) {
    FILE *file;
    char fullFilename[256];
    int wroteSomething = 0; 
    sprintf(fullFilename, "%s.ext", filename); 
    file = fopen(fullFilename, "w");
    if (file == NULL) {
        messageIssued("Error opening file!\n");
        return;
    }
    while (varsList != NULL)
    {
        if(varsList->typeOfUpdate == 3)
        {
            fprintf(file, "%s\t%04d\n", varsList->variablesName, varsList->variablesValue);
            wroteSomething = 1;
        }
        varsList = varsList->next;

    }
    fclose(file);
    if (!wroteSomething) {
        remove(fullFilename);
    }
}
/*
    Exports assembled code to a file in object file format.
    @param head: Pointer to the head of the linked list containing assembled code.
    @param varsList: Pointer to the head of the linked list containing variables.
    @param filename: Name of the file to export to.
    @param IC: Pointer to the Instruction Counter.
    @param DC: Pointer to the Data Counter.
*/
void exportCodeFile(Bits* head, variables* varsList, const char *filename,int* IC,int* DC) {
    FILE *file;
    char fullFilename[256];
    int wroteSomething = 0; 
    char coded_str[8];
    int result = 0;
    int ICClac = countInstruction(head);
    int DCClac = *DC + countEntrys(varsList);
    sprintf(fullFilename, "%s.ob", filename); 
    file = fopen(fullFilename, "w");
    if (file == NULL) {
        messageIssued("Error opening file!\n");
        return;
    }
    fprintf(file, "\t%d %d\t\n", ICClac,DCClac);/*head of the file*/
    while (head != NULL)
    {
        result = strcmp(head->sourceCode, "");
        if(result == 0)
        {
            head = head->next;
        }
        else
        {
            fprintf(file, "%04d %s\n", head->number,encoding(head, coded_str));
            wroteSomething = 1;
            head = head->next;
        }

    }
    fclose(file);
    if (!wroteSomething) {
        remove(fullFilename);
    }
}
/*
    Writes entry variables and their values to a file.
    @param varsList: Pointer to the head of the linked list containing variables.
    @param filename: Name of the file to write to.
*/
void writeVarsToFile( variables* varsList, const char *filename) {
    FILE *file;
    char fullFilename[256]; 
    int wroteSomething = 0; 
    sprintf(fullFilename, "%s.ent", filename); 
    file = fopen(fullFilename, "w");
    if (file == NULL) {
        messageIssued("Error opening file!\n");
        return;
    }

    while (varsList != NULL)
    {
        if(varsList->typeOfUpdate == 1)
        {
            fprintf(file, "%s\t%04d\n", varsList->variablesName, varsList->variablesValue);
            wroteSomething = 1;
        }
        varsList = varsList->next;
    }
    fclose(file);
    if (!wroteSomething) {
        remove(fullFilename);
    }
}
