#include "main.h"

/*
    Searches for variables in the variables list and updates the corresponding bits in the assembly code.
    @param headList: Pointer to the head of the linked list containing the assembly code.
    @param node: Pointer to the current node in the assembly code.
    @param varsList: Pointer to the head of the linked list containing variables.
    @return: Returns 1 if a variable match is found and the corresponding bits are updated, otherwise returns 0.
*/
int searchAndChangeVaribles(Bits *headList,Bits *node, variables* varsList) {
    int match = -1;
    int hash_match = -1;
    char binary12[13];
    char binary14[15];
    char hash_string[MAX_LINE_LENGTH];
    while (varsList != NULL)
    {
        if((node->sourceCode[0]) =='#')
        {
            strcpy(hash_string, node->sourceCode);
            removeHash(hash_string);
            if(isAsmbliNumber(hash_string) == 1)
            {
                hash_match = 1;
            }
            else
            {
                hash_match = strcmp(hash_string, varsList->variablesName);
            }
        }
        else
        {
            match = strcmp(node->sourceCode, varsList->variablesName);
        }

        if (match == 0 || hash_match == 0)
        {
            if (varsList->type) /*mdefine*/
            {
                if(node->addressingMethod == 2)
                {
                    intToBinary(varsList->variablesValue, 12, binary12);
                    changeBitsFromIndexToIndex(node, 0, 11, binary12);

                }
                else if(node->dataType == 1)
                {
                    intToBinary(varsList->variablesValue, 14, binary14);
                    changeBitsFromIndexToIndex(node, 0, 13, binary14);
                }
                else
                {
                    intToBinary(varsList->variablesValue, 12, binary12);
                    changeBitsFromIndexToIndex(node, 0, 11, binary12);
                }
                return 1;
            }
            else
            {
                intToBinary(varsList->variablesValue, 12, binary12);
                changeBitsFromIndexToIndex(node, 0, 11, binary12);
                return 1;
            }
        }
        varsList = varsList->next;
    }
    return 0;
}
/*
    Performs the second stage of the assembly process, updating the assembly code with variable values and handling extern declarations.
    @param head: Pointer to the head of the linked list containing the assembly code.
    @param commands: Array containing the command definitions.
    @param varsList: Pointer to the head of the linked list containing global variables.
    @param tempVarList: Pointer to the head of the linked list containing temporary variables.
*/
void stageTwo(Bits* head, command commands[], variables* varsList, variables* tempVarList)
{
    Bits *headlist = head;
    while (head != NULL) {
        searchAndChangeVaribles(headlist,head,varsList);
        searchForExtern(head,tempVarList);
        head = head->next;
    }
    variablesEntryExternal(varsList,tempVarList); /*check what is external and entry*/
}
