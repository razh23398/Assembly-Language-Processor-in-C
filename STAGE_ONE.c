#include "main.h"

/*
    Handles the definition of a constant value.
    @param line_number: The line number in the source file.
    @param str: The string containing the definition.
    @param define_stage: Pointer to the stage of the define process.
    @param saved_var_name: Array to store the name of the defined variable.
    @param instruction_counter: Pointer to the instruction counter.
    @param vars_list: Pointer to the list of variables.
    @param dc: Pointer to the data counter.
    @return: Returns 1 if an error is found, otherwise returns 0.
*/
int handleDefine(int line_number,char *str , int* define_stage, char saved_var_name[MAX_LINE_LENGTH], int* instruction_counter, variables** vars_list, int** dc, char* macrosNames) {
    int error_found = 0;
    char before_equal[MAX_LINE_LENGTH];
    char after_equal[MAX_LINE_LENGTH];
    (*define_stage) = 1;
    removeDefine(str);
    if(splitString(str, before_equal, after_equal) == 0)
    {
        messageIssued("Problem in line %d - Missing \"=\" in define definition.\n", line_number);
        error_found = 1;
    }
    if (variablesAlreadySaved(before_equal, *vars_list, -1) != -1) {
        messageIssued("Problem in line %d - You already have this name in the symbol type.\n", line_number);
        error_found = 1;
    }
    if (isASavedWord(before_equal, macrosNames)) {
        messageIssued("Problem in line %d - \"%s\" is a saved word you can't use it as a define\n", line_number,before_equal);
        error_found = 1;
    }
    strcpy(saved_var_name, before_equal);
    if(hasSpaceInMiddle(after_equal))
    {
        messageIssued("Problem in line %d - It is not possible to write more code words after the end of a prompt.\n", line_number);
        error_found = 1;
    }
    else
    {
        removeSpaces(after_equal);
        if (isAsmbliNumber(after_equal) == 0) {
            messageIssued("Problem in line %d - The value of define must be a legal number.\n", line_number);
            error_found = 1;
        }
        *instruction_counter = atoi(after_equal);
        insertVariable(vars_list, createVariable(*dc, saved_var_name, *instruction_counter, MDEFINE, NONE));
        (*define_stage) = 0;
    }
    return error_found;
}
/*
    Handles the data directive, processing the elements and inserting them into the memory.
    @param line_number: The line number in the source file.
    @param words: Array containing the words in the current line.
    @param head: Pointer to the head of the linked list containing the bits.
    @param vars_list: Pointer to the list of variables.
    @param ic: Pointer to the instruction counter.
    @param dc: Pointer to the data counter.
    @param i: Pointer to the index of the current line.
    @param temp_var_node: Pointer to the temporary variables node.
    @param num_elements_counter: Pointer to the counter for the number of elements processed.
    @param count_commas: Pointer to the counter for the number of commas encountered.
    @return: Returns 1 if an error is found, otherwise returns 0.
*/
int handleData(int line_number, char words[MAX_LINE_LENGTH][MAX_LINE_LENGTH], Bits** head, variables** vars_list, int** ic, int** dc, int* i,variables **temp_var_node,int* num_elements_counter,int* count_commas) {
    int error_found = 0;
    int num_elements;
    char** broken_strings;
    Bits* new_node;
    int j;
    const char delimiter[] = " \t,";
    *count_commas += countCommas(words[*i]);
    broken_strings = breakString(words[*i], delimiter, &num_elements);
    for (j = 0; j < num_elements; j++) {
        if (isAsmbliNumber(broken_strings[j]) == 1) {
            new_node = createNode(*ic, INTEGER, -1, broken_strings[j],NONE);
            insertBitsNode(head, new_node);
            bitChanger(new_node);
        } else {
            if (variablesAlreadySaved(broken_strings[j], *vars_list, 2) == -1) {
                messageIssued("Problem in line %d - found problem in \"%s\" not in symbol table or can't mdefine.\n", line_number, broken_strings[j]);
                error_found = 1;
            } else {
                insertBitsNode(head, createNode(*ic, INTEGER, -1, broken_strings[j],NONE));
            }
        }
        free(broken_strings[j]);
    }

    changeVariablesType(*temp_var_node, 0);
    *num_elements_counter += num_elements;
    free(broken_strings);

    return error_found;
}
/*
    Handles the string directive, processing the characters and inserting them into the memory.
    @param line_number: The line number in the source file.
    @param words: Array containing the words in the current line.
    @param head: Pointer to the head of the linked list containing the bits.
    @param ic: Pointer to the instruction counter.
    @param in_quotation_marks: Pointer to the flag indicating whether we are inside quotation marks.
    @param i: Pointer to the index of the current line.
    @return: Returns 1 if an error is found, otherwise returns 0.
*/
int handleString(int line_number, char words[MAX_LINE_LENGTH][MAX_LINE_LENGTH], Bits** head, int** ic, int* in_quotation_marks, int* i) {
    int error_found = 0;
    int str_count = 0;
    char str_char[3];
    Bits* new_node;

    if (!((words[*i][0]) == '"') && *in_quotation_marks == 0) {
        messageIssued("Problem in line %d - The string doesn't have a \" at start\n", line_number);
        error_found = 1;
    }

    if (*in_quotation_marks == 0) {
        *in_quotation_marks = 1;
        str_count++;
    }

    for (; str_count < strlen(words[*i]); str_count++) {
        if (*in_quotation_marks == 2) {
            messageIssued("Problem in line %d - You trying to add data after end of string \n", line_number);
            error_found = 1;
        } else if (*in_quotation_marks == 1 && words[*i][str_count] == '"') {
            *in_quotation_marks = 2;
        }

        if (*in_quotation_marks != 2) {
            if (!isprint(words[*i][str_count])) {
                messageIssued("Problem in line %d - You trying to add the char \"%c\" that not allowed in string \n", line_number, words[*i][str_count]);
                error_found = 1;
            }
            str_char[0] = words[*i][str_count];
            str_char[1] = '\0';
            new_node = createNode(*ic, CHAR, -1, str_char,NONE);
            insertBitsNode(head, new_node);
            bitChanger(new_node);
        }
    }

    if (*in_quotation_marks == 1) {
        char binary14[15];
        new_node = createNode(*ic, 0, -2, " ",NONE);
        insertBitsNode(head, new_node);
        intToBinary(32, 14, binary14);
        changeBitsFromIndexToIndex(new_node, 0, 13, binary14);
    } else {
        new_node = createNode(*ic, 0, -1, "\\0",NONE);
        insertBitsNode(head, new_node);
        bitChanger(new_node);
    }

    return error_found;
}
/*
    Handles the command directive, parsing the command and its operands and inserting them into memory.
    @param line_number: The line number in the source file.
    @param words: Array containing the words in the current line.
    @param vars_list: Pointer to the head of the variables list.
    @param commands: Array containing the command structs.
    @param head: Pointer to the head of the linked list containing the bits.
    @param ic: Pointer to the instruction counter.
    @param saved_cmd_node: Pointer to the saved command node.
    @param saved_reg_node: Pointer to the saved register node.
    @param was_reg: Pointer to the flag indicating if a register was used.
    @param i: Pointer to the index of the current line.
    @param iscmd: Pointer to the index of the current command in the commands array.
    @param num_elements_counter: Pointer to the counter of elements in the current line.
    @param count_commas: Pointer to the counter of commas in the current line.
    @return: Returns 1 if an error is found, otherwise returns 0.
*/
int handleCommand(int line_number, char words[MAX_LINE_LENGTH][MAX_LINE_LENGTH], variables** vars_list, command commands[], Bits** head, int** ic, Bits** saved_cmd_node, Bits** saved_reg_node, int* was_reg, int* i,int* iscmd,int* num_elements_counter,int* count_commas) {
    int error_found = 0;
    int num_elements;
    char** broken_strings;
    Bits* new_node;
    int j;
    char part1[50], part2[50];
    const char delimiter[] = " \t,";
    *count_commas += countCommas(words[*i]);
    if (strcmp(words[*i], ",") == 0) {
        return error_found;
    }
    broken_strings = breakString(words[*i], delimiter, &num_elements);
    for (j = 0; j < num_elements; j++) {
        if (parseString(broken_strings[j], part1, part2)) {
            if (j + *num_elements_counter == 0 && hasOneInCmdSrc(&commands[*iscmd])) {
                changeBitsFromIndexToIndex(*saved_cmd_node, 8, 9, "10");
            } else if (hasOneInCmdDes(&commands[*iscmd])) {
                changeBitsFromIndexToIndex(*saved_cmd_node, 10, 11, "10");
            } else {
                messageIssued("Problem in line %d - You can't use this cmd with these type of type\n", line_number);
                error_found = 1;
            }

            new_node = createNode(*ic, STRING, REGISTER, part1,1);
            insertBitsNode(head, new_node);
            changeBitsFromIndexToIndex(new_node, 12, 13, "10");
            if (isAsmbliNumber(part2) == 1) {
                new_node = createNode(*ic, INTEGER, INDIRECT, part2,1);
                insertBitsNode(head, new_node);
                bitChanger(new_node);
            } else {
                if (variablesAlreadySaved(part2, *vars_list, 2) == -1) {
                    messageIssued("Problem in line %d - found problem in \"%s\" not in symbol table or can't mdefine\n", line_number, part2);
                    error_found = 1;
                } else {
                    insertBitsNode(head, createNode(*ic, INTEGER, REGISTER, part2,1));
                }
            }
        } else {
            if (getRegisterIndexByString(broken_strings[j]) != -1)
            {
                if(*was_reg == 1 && *saved_reg_node == NULL)
                {
                    return 1;
                }
                if (j + *num_elements_counter == 0 && hasOneInCmdSrc(&commands[*iscmd])) {
                    int number;
                    char binary3[4];
                    *was_reg = 1;
                    changeBitsFromIndexToIndex(*saved_cmd_node, 8, 9, "11");
                    new_node = createNode(*ic, STRING, 3, broken_strings[j],1);
                    insertBitsNode(head, new_node);
                    *saved_reg_node = new_node;
                    number = getRegisterIndexByNode(*saved_reg_node);
                    intToBinary(number, 3, binary3);
                    changeBitsFromIndexToIndex(new_node, 6, 8, binary3);
                } else if (hasOneInCmdDes(&commands[*iscmd])) {
                    changeBitsFromIndexToIndex(*saved_cmd_node, 10, 11, "11");
                    if (*was_reg == 1) {
                        int number;
                        char binary3[4];
                        number = getRegisterIndexByString(broken_strings[j]);
                        intToBinary(number, 3, binary3);
                        strcat((*saved_reg_node)->sourceCode, " ");
                        strcat((*saved_reg_node)->sourceCode, broken_strings[j]);
                        changeBitsFromIndexToIndex(*saved_reg_node, 9, 11, binary3);
                        *was_reg = 0;
                    } else {
                        int number;
                        char binary3[4];
                        new_node = createNode(*ic, STRING, 3, broken_strings[j],1);
                        insertBitsNode(head, new_node);
                        *saved_reg_node = new_node;
                        number = getRegisterIndexByNode(*saved_reg_node);
                        intToBinary(number, 3, binary3);
                        changeBitsFromIndexToIndex(new_node, 9, 11, binary3);
                    }
                } else {
                    messageIssued("Problem in line %d - You can't use this cmd with these type of type\n", line_number);
                    error_found = 1;
                }
            } else if ((broken_strings[j][0]) == '#') {
                char new_string[MAX_LINE_LENGTH];
                if (j + *num_elements_counter == 0 && hasOneInCmdSrc(&commands[*iscmd])) {
                    changeBitsFromIndexToIndex(*saved_cmd_node, 8, 9, "00");
                } else if (hasOneInCmdDes(&commands[*iscmd])) {
                    changeBitsFromIndexToIndex(*saved_cmd_node, 10, 11, "00");
                } else {
                    messageIssued("Problem in line %d - You can't use this cmd with these type of type\n", line_number);
                    error_found = 1;
                }
                strcpy(new_string, broken_strings[j]);
                removeHash(new_string);
                if (isAsmbliNumber(new_string) == 1) {
                    new_node = createNode(*ic, STRING, DIRECT, broken_strings[j],1);
                    insertBitsNode(head, new_node);
                    bitChanger(new_node);
                } else {
                    if (variablesAlreadySaved(new_string, *vars_list, 2) == -1) {
                        messageIssued("Problem in line %d - found problem in \"%s\" not in symbol table or can't mdefine.\n", line_number, broken_strings[j]);
                        error_found = 1;
                    } else {
                        insertBitsNode(head, createNode(*ic, STRING, REGISTER, broken_strings[j],1));
                    }
                }
            } else {
                if (isNumber(broken_strings[j]) == 1) {
                    if (j + *num_elements_counter == 0 && hasOneInCmdSrc(&commands[*iscmd])) {
                        changeBitsFromIndexToIndex(*saved_cmd_node, 8, 9, "00");
                    } else if (hasOneInCmdDes(&commands[*iscmd])) {
                        changeBitsFromIndexToIndex(*saved_cmd_node, 10, 11, "00");
                    } else {
                        messageIssued("Problem in line %d - You can't use this cmd with these type of type\n", line_number);
                        error_found = 1;
                    }
                    new_node = createNode(*ic, STRING, -2, broken_strings[j],1);
                    insertBitsNode(head, new_node);
                    changeBitsFromIndexToIndex(new_node, 12, 13, "00");
                } else {
                    if (j + *num_elements_counter == 0 && hasOneInCmdSrc(&commands[*iscmd])) {
                        changeBitsFromIndexToIndex(*saved_cmd_node, 8, 9, "01");
                    } else if (hasOneInCmdDes(&commands[*iscmd])) {
                        changeBitsFromIndexToIndex(*saved_cmd_node, 10, 11, "01");
                    } else {
                        messageIssued("Problem in line %d - You can't use this cmd with these type of type\n", line_number);
                        error_found = 1;
                    }
                    new_node = createNode(*ic, STRING, INDIRECT, broken_strings[j],1);
                    insertBitsNode(head, new_node);
                    changeBitsFromIndexToIndex(new_node, 12, 13, "10");
                }
            }
        }
        free(broken_strings[j]);
    }
    *num_elements_counter += num_elements;
    free(broken_strings);

    return error_found;
}
/*
    Handles the extern directive, inserting the external variable into the variables list.
    @param line_number: The line number in the source file.
    @param words: Array containing the words in the current line.
    @param vars_list: Pointer to the head of the variables list.
    @param temp_vars_list: Pointer to the head of the temporary variables list.
    @param dc: Pointer to the data counter.
    @param ic: Pointer to the instruction counter.
    @param i: Pointer to the index of the current word in the line.
    @return: Returns 1 if an error is found, otherwise returns 0.
*/
int handleExtern(int line_number, char words[MAX_LINE_LENGTH][MAX_LINE_LENGTH], variables** vars_list, variables** temp_vars_list, int** dc, int** ic, int* i) {
    int error_found = 0;

    if (variablesAlreadySaved(words[*i], *temp_vars_list, -1) != -1) {
        messageIssued("Problem in line %d - You already have this label\n", line_number);
        error_found = 1;
    }
    insertVariable(vars_list, createVariable(*dc, words[*i], 0, DATA, EXTERNAL));
    insertVariable(temp_vars_list, createTempVariable(words[*i], **ic, DATA, EXTERNAL));

    return error_found;
}
/*
    Handles the entry directive, inserting the entry variable into the temporary variables list.
    @param line_number: The line number in the source file.
    @param words: Array containing the words in the current line.
    @param temp_vars_list: Pointer to the head of the temporary variables list.
    @param ic: Pointer to the instruction counter.
    @param i: Pointer to the index of the current word in the line.
    @return: Returns 1 if an error is found, otherwise returns 0.
*/
int handleEntry(int line_number, char words[MAX_LINE_LENGTH][MAX_LINE_LENGTH], variables** temp_vars_list, int** ic, int* i) {
    int error_found = 0;

    if (variablesAlreadySaved(words[*i], *temp_vars_list, -1) != -1) {
        messageIssued("Problem in line %d - You already have this label\n", line_number);
        error_found = 1;
    }
    insertVariable(temp_vars_list, createTempVariable(words[*i], **ic, DATA, ENTRY));

    return error_found;
}
/*
    Handles various directives and commands encountered in the source file.
    @param line_number: The line number in the source file.
    @param words: Array containing the words in the current line.
    @param commands: Array of structs representing assembly commands.
    @param head: Pointer to the head of the linked list containing the assembly code.
    @param vars_list: Pointer to the head of the variables list.
    @param temp_vars_list: Pointer to the head of the temporary variables list.
    @param dc: Pointer to the data counter.
    @param ic: Pointer to the instruction counter.
    @param situation: Pointer to the current situation in the assembly process.
    @param define_stage: Pointer to the stage of the define directive processing.
    @param has_label: Pointer to flag indicating whether a label has been encountered.
    @param labelFlag: Pointer to flag indicating whether the current word is a label.
    @param temp_var_node: Pointer to the temporary variable node being processed.
    @param i: Pointer to the index of the current word in the line.
    @param iscmd: Pointer to the index of the current command in the commands array.
    @param saved_cmd_node: Pointer to the node storing the current command information.
    @return: Returns 1 if an error is found, otherwise returns 0.
*/
int handleDefult(int line_number, char words[MAX_LINE_LENGTH][MAX_LINE_LENGTH], command commands[], Bits** head, variables** vars_list, variables** temp_vars_list, int** dc, int** ic, int* situation, int* define_stage, int* has_label,int* labelFlag, variables** temp_var_node, int* i,int* iscmd,Bits** saved_cmd_node, char* macrosNames) {
    int error_found = 0;
    Bits* newNode = NULL;
    if ((words[*i][strlen(words[*i]) - 1]) == ':') {
        if (!isAValidLabel(words[*i],line_number)) {
            error_found = 1;
        }
        if (variablesAlreadySaved(words[*i], *vars_list, -1) != -1) {
            messageIssued("Problem in line %d - You already have this label\n", line_number);
            error_found = 1;
        }
        if (isASavedWord(words[*i], macrosNames)) {
            messageIssued("Problem in line %d - \"%s\" is a saved word you can't use it as a define\n", line_number,words[*i]);
            error_found = 1;
        }
        if (*has_label == 0)
            *has_label = 1;
        else{
            messageIssued("Problem in line %d - You can't declare more then one label in same line.\n", line_number);
            error_found = 1;
        }
        *labelFlag = 1;
        *temp_var_node = createVariable(*dc, words[*i], **ic, DATA, LABEL);
        insertVariable(vars_list, *temp_var_node);
    } else if (strcmp(words[*i], ".define") == 0) {
        if (*has_label != 0) {
            messageIssued("Problem in line %d - Label can't be declared before define\n", line_number);
            error_found = 1;
        }
        *situation = SITUATION_DEFINE;
    } else if (strcmp(words[*i], ".data") == 0) {
        if (*has_label == 1) {
            changeVariablesType(*temp_var_node, 0);
            *has_label = 2;
        }
        *situation = SITUATION_DATA;
    } else if (strcmp(words[*i], ".string") == 0) {
        if (*has_label == 1) {
            changeVariablesType(*temp_var_node, 0);
            *has_label = 2;
        }
        *situation = SITUATION_STRING;
    } else if (strcmp(words[*i], ".extern") == 0) {
        if (*has_label != 0) {
            messageIssued("Problem in line %d - Label can't be declared before extern\n", line_number);
            deleteVar(vars_list, **dc-1);
            (**dc)--;
        }
        *situation = SITUATION_EXTERN;
    } else if (strcmp(words[*i], ".entry") == 0) {
        if (*has_label != 0) {
            messageIssued("Problem in line %d - Label can't be declared before entry\n", line_number);
            deleteVar(vars_list, **dc-1);
            (**dc)--;
        }
        *situation = SITUATION_ENTRY;
    } else if ((*iscmd = isCommand(words[*i], commands, 16)) != -1) {
        if (*has_label == 1) {
            changeVariablesType(*temp_var_node, 1);
            *has_label = 2;
        }
        *situation = SITUATION_COMMAND;
        newNode = createNode(*ic, STRING, 3, words[*i],1);
        *saved_cmd_node = newNode;
        insertBitsNode(head, newNode);
        commandChanger(newNode, commands);

        
    } else {
        messageIssued("Problem in line %d - Error in instruction name %s\n", line_number, words[*i]);
        error_found = 1;
    }
    return error_found;
}
/*
    Handles the remaining cases where errors might occur during assembly processing.
    @param line_number: The line number in the source file.
    @param define_stage: Pointer to the stage of the define directive processing.
    @param data_counter: Pointer to the data counter.
    @param commands: Array of structs representing assembly commands.
    @param situation: Pointer to the current situation in the assembly process.
    @param word_count: Number of words in the current line.
    @param cmdIndex: Pointer to the index of the current command in the commands array.
    @return: Returns 1 if an error is found, otherwise returns 0.
*/
int handle_remaining_cases(int line_number, int* define_stage, int* data_counter, command commands[], int* situation, int word_count,int* cmdIndex) {
    int error_found = 0;

    if (*define_stage != 0) {
        messageIssued("Problem in line %d - Missing define value or name, You must define in this structure .define (name of define) = (value) \n", line_number);
        error_found = 1;
    }

    if (*data_counter + 1 != word_count && *situation == SITUATION_DATA) {
        messageIssued("Problem in line %d - Mismatch in element count for data! Expected words: %d, Found: %d\n", line_number, *data_counter +1, word_count);
        error_found = 1;
    }

    if (*situation == SITUATION_COMMAND && commands[*cmdIndex].numberOfOperand != word_count) {
        messageIssued("Problem in line %d - Mismatch in element count for command '%s'! Expected number of ops: %d, Found: %d\n", line_number, commands[*cmdIndex].name, commands[*cmdIndex].numberOfOperand, word_count);
        error_found = 1;
    }

    if (*data_counter + 1 != word_count && *situation == SITUATION_COMMAND && word_count > 0) {
        messageIssued("Problem in line %d - Mismatch in comma count! Expected commas: %d, Found words: %d\n", line_number, *data_counter + 1, word_count);
        error_found = 1;
    }

    return error_found;
}
/*
    Tokenizes a line of content into individual words.
    @param content: The line of content to tokenize.
    @param words: A 2D array to store the tokenized words.
    @param count_words: Pointer to an integer storing the count of words.
    @return: Returns 0 if successful, -1 if the word count exceeds MAX_LINE_LENGTH.
*/
int tokenizeLine(char content[], char words[][MAX_LINE_LENGTH], int *count_words) {
    char *token = strtok(content, " \t\n");
    while (token != NULL && *count_words < MAX_LINE_LENGTH) {
        strcpy(words[*count_words], token);
        (*count_words)++;
        token = strtok(NULL, " \t\n");
    }
    return (*count_words >= MAX_LINE_LENGTH) ? -1 : 0;
}
/*
    Executes the first pass of the assembly process for a given line.
    @param Line: The line number being processed.
    @param lines: Information about the line being processed.
    @param commands: Array of command structures representing assembly commands.
    @param head: Pointer to the head of the linked list containing the assembly code.
    @param VarsList: Pointer to the head of the linked list containing variables.
    @param TempVarsList: Pointer to the head of the linked list containing temporary variables.
    @param IC: Pointer to the instruction counter.
    @param DC: Pointer to the data counter.
    @return: Returns the number of errors encountered during the first pass.
*/
int stageOne(int Line, lineInfo lines, command commands[], Bits **head, variables **VarsList, variables **TempVarsList, int **IC, int **DC, char* macrosNames) {
    int count_words = 0;
    char words[MAX_LINE_LENGTH][MAX_LINE_LENGTH];
    int wordIndexCounter;
    int situation = 0;
    int define_stage = 0;
    variables *tempVarNode = NULL;
    char saved_var_name[MAX_LINE_LENGTH];
    int instruction_counter = 0;
    int count_Commas = 0;
    int num_elements_counter = 0;
    int iscmd = 0;
    int was_reg = 0;
    int situationEnd = 0;
    int has_label = 0;
    int labelFlag = 0;
    int in_quotation_marks = 0;
    Bits *saved_cmd_node = NULL;
    Bits *saved_node = NULL;
    int error_found = 0;
    char tempLine[81];
    int main_loop_flag = 1;
    
    /* Tokenize the line */
    strcpy(tempLine, lines.content);
    if (tokenizeLine(lines.content, words, &count_words) == -1) {
        messageIssued("Error: Maximum line length exceeded\n");
        error_found++; 
    }
    for (wordIndexCounter = 0; wordIndexCounter < count_words && main_loop_flag; wordIndexCounter++) {
        if (**IC >= MEMORY_SIZE) {
            messageIssued("Problem in line %d - Memory overflow in main bits list The memory size is %d suitable and you over it\n",Line,MEMORY_SIZE);
            error_found++;
            break;
        }
        if (situationEnd == 1) {
            messageIssued("Problem in line %d - It is not possible to write more code words after the end of a prompt.\n", Line);
            error_found++;
        }
        if(labelFlag == 1)
        {
            labelFlag = 2;
        }
        printf("line - %d - situation - %d WORD - %s\n", Line,situation, words[wordIndexCounter]);
        
        /* Determine the situation based on the current word */
        switch (situation) {
            case SITUATION_DEFINE:
                error_found |= handleDefine(Line, tempLine, &define_stage, saved_var_name, &instruction_counter, VarsList, DC, macrosNames);
                main_loop_flag = 0;
                break;
            case SITUATION_DATA: /*DATA*/
                error_found |= handleData(Line, words, head, VarsList, IC, DC, &wordIndexCounter, &tempVarNode, &num_elements_counter, &count_Commas);
                break;
            case SITUATION_STRING: /*STRING*/
                error_found |= handleString(Line, words, head, IC, &in_quotation_marks, &wordIndexCounter);
                break;
            case SITUATION_COMMAND: /*COMMAND*/
                error_found |= handleCommand(Line, words, VarsList, commands, head, IC, &saved_cmd_node, &saved_node, &was_reg, &wordIndexCounter, &iscmd, &num_elements_counter, &count_Commas);
                break;
            case SITUATION_EXTERN: /*extern*/
                error_found |= handleExtern(Line, words, VarsList, TempVarsList, DC, IC, &wordIndexCounter);
                break;
            case SITUATION_ENTRY: /*entry*/
                error_found |= handleEntry(Line, words, TempVarsList, IC, &wordIndexCounter);
                break;
            case SITUATION_DEFULT:
                error_found |= handleDefult(Line, words, commands, head, VarsList, TempVarsList, DC, IC, &situation, &define_stage, &has_label,&labelFlag, &tempVarNode, &wordIndexCounter, &iscmd, &saved_cmd_node, macrosNames);
                break;
        }
    }
    if(labelFlag == 1)
    {
        messageIssued("Problem in line %d - Missing instruction/action after label.\n", Line);
        error_found++;
    }
    /* Exception handling */
    error_found |= handle_remaining_cases(Line, &define_stage, &count_Commas, commands, &situation, num_elements_counter, &iscmd);
    return error_found;
}
/*
    Executes the preprocessing stage before the first pass of the assembly process.
    @param IC: Pointer to the instruction counter.
    @param DC: Pointer to the data counter.
    @param filename: Name of the input file.
    @param commands: Array of command structures representing assembly commands.
    @param head: Pointer to the head of the linked list containing the assembly code.
    @param VarsList: Pointer to the head of the linked list containing variables.
    @param TempVarsList: Pointer to the head of the linked list containing temporary variables.
    @return: Returns the number of errors encountered during preprocessing.
*/
int preStageOne(int *IC, int *DC, const char *filename, command commands[], Bits **head, variables **VarsList, variables **TempVarsList, char* macrosNames) {
    char temp_line[MAX_LINE_LENGTH];
    int line_count = 0;
    int index;
    int passedRun = 0;
    int line_capacity = 50;
    int longLineFlag = 0;
    lineInfo *lines = malloc(line_capacity * sizeof(lineInfo)); 


    FILE *file = fopen(filename, "r");
    if (!file) {
        messageIssued("Failed to open file\n");
        free(lines); 
        return 1;
    }

    while (fgets(temp_line, MAX_LINE_LENGTH, file) != NULL) {
        if (strchr(temp_line, '\n') == NULL && !feof(file)) {
            messageIssued("ignore line %d becouse it was too long. Maximum chars at line is %d.\n", line_count, MAX_LINE_LENGTH - 1);
            longLineFlag = 1;
            continue;
        }
        if(longLineFlag == 1) 
        {
            longLineFlag = 0;
            continue;
        }
        temp_line[strcspn(temp_line, "\r\n")] = '\0';
        temp_line[strlen(temp_line)] = '\0';  
        if (temp_line[0] == '\0') {
            continue;
        }

        if (temp_line[0] == ';') {
            continue;
        }
        memset(&lines[line_count], 0, sizeof(lineInfo));
        strcpy(lines[line_count].content, temp_line);


        line_count++;

        if (line_count >= line_capacity) {
            line_capacity += 50;
            lines = realloc(lines, line_capacity * sizeof(lineInfo));
            if (!lines) {
                messageIssued("Memory allocation failed.\n");
                free(lines);
                fclose(file);
                return 1;
            }
        }
    }

    if (line_count == 0) {
        messageIssued("The file is empty.\n");
    } else {
        printf("\nAll lines in the file:\n");
        for (index = 0; index < line_count; index++) {
            printf("Line %d: %s\n", index + 1, lines[index].content);
        }
    }
    for (index = 0; index < line_count; index++) {
        passedRun += stageOne(index, lines[index], commands, head, VarsList, TempVarsList, &IC, &DC, macrosNames);
    }
    free(lines); 
    fclose(file);
    return passedRun;
}

