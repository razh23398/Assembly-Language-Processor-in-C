#include "main.h"

/*
    Inserts a Bits node at the end of a linked list.
    @param head: Pointer to the head of the linked list.
    @param newNode: Pointer to the node to be inserted.
*/
int insertBitsNode(Bits** head, Bits* newNode) {
    Bits* temp;

    if (newNode == NULL) {
        return 0; 
    }

    if (*head == NULL) {
        *head = newNode;
        newNode->next = NULL;
        return 1; 
    }

    temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }

    temp->next = newNode;
    newNode->next = NULL;

    return 1; 
}
/*
    Creates a new variables node and initializes its values.
    @param DC: Pointer to the data counter.
    @param word: The name of the variable.
    @param variablesValue: The value of the variable.
    @param type: The data type of the variable.
    @param typeOfUpdate: The update type of the variable.
    @return: Returns a pointer to the newly created variables node.
             Returns NULL if memory allocation fails or if there is a memory overflow.
*/
 variables* createVariable(int *DC, const char* word, int variablesValue, DataType type, updateType typeOfUpdate) {
    int word_len = strlen(word);
     variables* newVariable = ( variables*)malloc(sizeof( variables));

    if (newVariable == NULL) {
        messageIssued("Memory allocation failed!\n");
        return NULL; 
    }

    if (*DC >= MAX_VARIABLES || word_len >= MAX_VARIABLES_NAMES) {
        messageIssued("Memory overflow in variables list\n");
        return NULL;
    }

    if (word[word_len - 1] == ':') {
        word_len--;
    }

    strncpy(newVariable->variablesName, word, word_len);
    newVariable->variablesName[word_len] = '\0';
    if(typeOfUpdate == 2)
    {
        newVariable->variablesValue = 0;
    }
    else
    {
        if (!variablesValue) {
            newVariable->variablesValue = *DC;
        }
        else {
            newVariable->variablesValue = variablesValue;
        }
    }
    newVariable->variablesNumber = *DC;
    newVariable->type = type;
    newVariable->typeOfUpdate = typeOfUpdate;
    newVariable->next = NULL;  
    (*DC)++;
    return newVariable;
}
/*
    Creates a new temporary variables node and initializes its values.
    @param word: The name of the temporary variable.
    @param variablesValue: The value of the temporary variable.
    @param type: The data type of the temporary variable.
    @param typeOfUpdate: The update type of the temporary variable.
    @return: Returns a pointer to the newly created temporary variables node.
             Returns NULL if memory allocation fails or if there is a memory overflow.
*/
 variables* createTempVariable(const char* word, int variablesValue, DataType type, updateType typeOfUpdate) {
    int word_len = strlen(word);
     variables* newVariable = ( variables*)malloc(sizeof( variables));

    if (newVariable == NULL) {
        messageIssued("Memory allocation failed!\n");
        return NULL; 
    }

    if (word_len >= MAX_VARIABLES_NAMES) {
        messageIssued("Memory overflow\n");
        return NULL; 
    }

    strncpy(newVariable->variablesName, word, word_len);
    newVariable->variablesName[word_len] = '\0';


    newVariable->variablesValue = variablesValue;

    newVariable->variablesNumber = 0;
    newVariable->type = type;
    newVariable->typeOfUpdate = typeOfUpdate;
    newVariable->next = NULL;  
    return newVariable;
}
/*
    Inserts a variables node at the end of a linked list.
    @param head: Pointer to the head of the linked list.
    @param newVariable: Pointer to the node to be inserted.
*/
int insertVariable(variables** head, variables* newVariable) {
    variables* temp;

    if (newVariable == NULL) {
        return 0; 
    }

    if (*head == NULL) {
        *head = newVariable;
        newVariable->next = NULL;
        return 1; 
    }

    temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }

    temp->next = newVariable;
    newVariable->next = NULL;

    return 1;
}
/*
    Creates a new Bits node and initializes its values.
    @param IC: Pointer to the instruction counter.
    @param dataType: The data type of the node.
    @param addressingMethod: The addressing method of the node.
    @param sourceCode: The source code associated with the node.
    @param instruction: The instruction flag indicating if the node represents an instruction.
    @return: Returns a pointer to the newly created Bits node.
             Returns NULL if memory allocation fails or if there is a memory overflow.
*/
Bits* createNode(int *IC, DataType dataType, AddressingMethod addressingMethod, char sourceCode[],int instruction)
{
    Bits* newNode = (Bits*)malloc(sizeof(Bits));
    if (newNode == NULL) {
        messageIssued("Memory allocation failed!\n");
        return NULL; 
    }
    if (*IC >= MEMORY_SIZE) {
        messageIssued("Memory overflow in main bits list The memory size is %d suitable and you over it\n\n",MEMORY_SIZE);
        return NULL; 
    }
    if (strlen(sourceCode) >= SOURCE_CODE_NUMBER) {
        messageIssued("Source code too long\n");
        return NULL; 
    }

    newNode->number = *IC;
    strncpy(newNode->bits, "00000000000000", BITS_NUMBER - 1);
    newNode->bits[BITS_NUMBER - 1] = '\0';
    newNode->dataType = dataType;
    newNode->addressingMethod = addressingMethod;
    strncpy(newNode->sourceCode, sourceCode, SOURCE_CODE_NUMBER - 1);
    newNode->sourceCode[SOURCE_CODE_NUMBER - 1] = '\0';
    newNode->IC = *IC;
    newNode->instruction = instruction;
    newNode->next = NULL;
    (*IC)++;
    return newNode;
}
/*
    Changes a substring in the given string from the specified start index to the end index with a new substring.
    @param str: The string to be modified.
    @param start: The starting index of the substring to be replaced.
    @param end: The ending index of the substring to be replaced.
    @param newSubstring: The new substring to replace the old substring.
*/
int changeBitsFromIndexToIndex(Bits* node, int start, int end, const char *newSubstring) {
    int newLength = strlen(newSubstring);
    int oldLength = end - start + 1;
    int shift = newLength - oldLength;
    if (node == NULL) {
        return 0; 
    }
    if (shift != 0) {
        int tailLength = strlen(node->bits + end + 1);
        
        memmove(node->bits + end + 1 + shift, node->bits + end + 1, tailLength + 1);
    }
    
    memcpy(node->bits + start, newSubstring, newLength);
    return 1;
}
/*
    Modifies the Bits node according to the command found in its source code.
    @param node: Pointer to the Bits node to be modified.
    @param commands: Array of command structures.
*/
int commandChanger(Bits *node, command commands[])
{
    char binary[5];
    int opcode = 0;
    if (node == NULL) {
        return 0; 
    }
    opcode = isCommand(node->sourceCode, commands, 16);
    if(opcode != -1)
    {
        intToBinary(opcode,4,binary);
        changeBitsFromIndexToIndex(node, 4, 7, binary);
        node->dataType = 3;
    }
    return 1;
}
/*
    Modifies the Bits node based on its addressing method and data type.
    @param node: Pointer to the Bits node to be modified.
*/
int bitChanger(Bits *node)
{
    int number;
    int i;
    char binary14[15];
    char binary12[13];
    char name[MAX_LINE_LENGTH];
    int size = 14;
    if (node == NULL) {
        return 0; 
    }
    if(node->addressingMethod == 0)
    {
        strcpy(name,node->sourceCode+1);
        number = atoi(name);
        intToBinary(number, 12, binary12);
        changeBitsFromIndexToIndex(node, 0, 11, binary12);
    }
    else if(node->addressingMethod == 1)
    {

        strcpy(name,node->sourceCode);
        number = atoi(name);
        intToBinary(number, 12, binary12);
        changeBitsFromIndexToIndex(node, 0, 11, binary12);
    }
    else if(node->addressingMethod == 2)
    {
        strcpy(name,node->sourceCode);
        number = atoi(name);
        intToBinary(number, 12, binary12);
        changeBitsFromIndexToIndex(node, 0, 11, binary12);
    }
    else if(node->dataType == 1)
    {
        number = atoi(node->sourceCode);
        intToBinary(number, size, binary14);
        changeBitsFromIndexToIndex(node, 0, 13, binary14);
    }
    else if(node->dataType == 2)
    {
        for (i = 0; i < strlen(node->sourceCode); i++) {
            intToBinary(node->sourceCode[i], size, binary14); 
            changeBitsFromIndexToIndex(node, 0, 13, binary14);
        }
    }
    return 1;
}
/*
    Converts a 4-bit binary string to its equivalent decimal value.
    @param binary: The binary string to be converted.
    @return: Returns the decimal value represented by the 4-bit binary string.
*/
int binary4ToDecimal(char *binary) {
    int decimal = 0;
    int i;
    for (i = 4; i <= 7; i++) {
        decimal = (decimal << 1) | (binary[i] - '0');
    }
    return decimal;
}
/*
    Changes the data type of variables in a linked list.
    @param head: Pointer to the head of the linked list.
    @param type: The new data type to be assigned.
*/
int changeVariablesType(variables *head, int type)
{
    if (head == NULL) {
        return 0; 
    }
    head->type = type;
    return 1;
}
/*
    Deletes a variables node at the specified position from a linked list.
    @param head: Pointer to the head of the linked list.
    @param position: The position of the node to be deleted.
*/
void deleteVar(variables** head, int position) {
     variables* temp;
     variables* nextNode;
    int i;
    if (*head == NULL) {
        messageIssued("List is empty. Cannot delete.\n");
        return;
    }

    if (position < 0) {
        messageIssued("Invalid position. Position cannot be negative.\n");
        return;
    }

    temp = *head;

    if (position == 0) {
        *head = temp->next;
        free(temp);
        return;
    }

    for (i = 0; temp != NULL && i < position - 1; i++)
        temp = temp->next;

    if (temp == NULL || temp->next == NULL) {
        messageIssued("Invalid position. Position out of bounds.\n");
        return;
    }

    nextNode = temp->next->next;

    free(temp->next);

    temp->next = nextNode;
}
/*
    Encodes the Bits node into a symbol-based string.
    @param node: Pointer to the Bits node to be encoded.
    @param coded_str: The string to store the encoded symbols.
    @return: Returns a pointer to the encoded string.
*/
char* encoding(Bits *node, char *coded_str) {
    int i;
    for (i = 0; i < 14; i += 2) {
        int value = (node->bits[i] - '0') * 2 + (node->bits[i + 1] - '0');
        coded_str[i / 2] = printSymbol(value);
    }
    coded_str[7] = '\0';
    return coded_str;
}
/*
    Prints the encoding of Bits nodes in the linked list.
    @param head: Pointer to the head of the linked list.
*/
void printEncoding(Bits* head) {
    int result = 0;
    printf("Printing the list:\n");
    while (head != NULL)
    {
        result = strcmp(head->sourceCode, "");
        if(result == 0)
        {
            head = head->next;
        }
        else
        {
            char coded_str[8];
            encoding(head,coded_str);
            head = head->next;
        }
    }
}
/*
    Prints the details of Bits nodes in the linked list.
    @param head: Pointer to the head of the linked list.
*/
void printList(Bits* head) {
    int result = 0;
    printf("Printing the list:\n");
    while (head != NULL)
    {
        result = strcmp(head->sourceCode, "");
        if(result == 0)
        {
            head = head->next;
        }
        else
        {
            char coded_str[8];
            printf("(%04d)    - encode %-10s - bits %-20s name - %-10s dataType - %-10d adress_type - %-10d\n", head->number,encoding(head, coded_str), head->bits,head->sourceCode,head->dataType, head->addressingMethod);
            head = head->next;
        }
    }
}

/*
    Checks if a variable name is already saved in the linked list of variables.
    If found, returns the variable number if the type matches; otherwise, returns -1.
    If type is set to -1, returns the variable number regardless of the type.
    @param str: The variable name to be checked.
    @param head: Pointer to the head of the linked list of variables.
    @param type: The type of the variable to match (-1 for any type).
    @return: Returns the variable number if found and type matches; otherwise, returns -1.
*/
int variablesAlreadySaved(char *str,  variables* head, int type) {
    int match = -1;
    if (str[strlen(str) - 1] == ':') {
        str[strlen(str) - 1] = '\0';
    }
    while (head != NULL) {
        if (type != -1) {
            match = strcmp(str, head->variablesName);
            if (match == 0) {
                if (head->type == type) {
                    return head->variablesNumber;
                }
            }
        } else {
            match = strcmp(str, head->variablesName);
            if (match == 0) {
                return head->variablesNumber;
            }
        }
        head = head->next;
    }
    return -1;
}
/*
    Prints the details of variables in the linked list.
    @param head: Pointer to the head of the linked list.
*/
void printVariableList( variables* head) {
    printf("Printing the list:\n");
    while (head != NULL) {
        printf("(%d) [vname] %-10s [vvalue] %-10d [vtype] %-10d [vtypeOfUpdate] %-10d\n", head->variablesNumber,head->variablesName, head->variablesValue,head->type,head->typeOfUpdate);
        head = head->next;
    }
}



