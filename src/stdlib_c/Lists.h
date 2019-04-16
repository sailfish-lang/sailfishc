/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#pragma once
#include <string>

const static std::string APPEND_LIST_INT =
    "\nint*"
    "\nappendListInt(int* a, int* b, int a_size, int b_size)"
    "\n{"
    "\n    int* combined = malloc((a_size + b_size) * sizeof(int));"
    "\n    memcpy(combined,     a, a_size * sizeof(int));"
    "\n    memcpy(combined + a_size, b, b_size * sizeof(int));"
    "\n    free(a);"
    "\n    free(b);"
    "\n    return combined;"
    "\n}\n";

const static std::string APPEND_LIST_STR =
    "\nchar**"
    "\nappendListStr(char** a, char** b, int a_size, int b_size)"
    "\n{"
    "\n    char** combined = malloc((a_size + b_size) * sizeof(char*));"
    "\n    memcpy(combined,     a, a_size * sizeof(char*));"
    "\n    memcpy(combined + a_size, b, b_size * sizeof(char*));"
    "\n    free(a);"
    "\n    free(b);"
    "\n    return combined;"
    "\n}\n";

const static std::string APPEND_LIST_BOOL =
    "\nint*"
    "\nappendListBool(int* a, int* b, int a_size, int b_size)"
    "\n{"
    "\n    int* combined = malloc((a_size + b_size) * sizeof(int));"
    "\n    memcpy(combined,     a, a_size * sizeof(int));"
    "\n    memcpy(combined + a_size, b, b_size * sizeof(int));"
    "\n    free(a);"
    "\n    free(b);"
    "\n    return combined;"
    "\n}\n";

const static std::string APPEND_LIST_FLT =
    "\nfloat*"
    "\nappendListFlt(float* a, float* b, int a_size, int b_size)"
    "\n{"
    "\n    float* combined = malloc((a_size + b_size) * sizeof(float));"
    "\n    memcpy(combined,     a, a_size * sizeof(float));"
    "\n    memcpy(combined + a_size, b, b_size * sizeof(float));"
    "\n    free(a);"
    "\n    free(b);"
    "\n    return combined;"
    "\n}\n";

const static std::string DELETE_AT_INDEX_INT =
    "\nint*"
    "\ndeleteAtIndexInt(int* a, int a_size, int index)"
    "\n{"
    "\n    if (a_size == 0 || a_size - 1 < index || index < 0)"
    "\n        return a;"
    "\n"
    "\n    int* downsize = malloc((a_size - 1) * sizeof(int));"
    "\n    for (int i = 0; i < index; i ++)"
    "\n        downsize[i] = a[i];"
    "\n    for (int i = index + 1; i < a_size; i ++)"
    "\n        downsize[i-1] = a[i];"
    "\n    return downsize;"
    "\n}\n";

const static std::string DELETE_AT_INDEX_STR =
    "\nchar**"
    "\ndeleteAtIndexStr(char** a, int a_size, int index)"
    "\n{"
    "\n    if (a_size == 0 || a_size - 1 < index || index < 0)"
    "\n        return a;"
    "\n"
    "\n    char** downsize = malloc((a_size - 1) * sizeof(char*));"
    "\n    for (int i = 0; i < index; i ++)"
    "\n        downsize[i] = a[i];"
    "\n    for (int i = index + 1; i < a_size; i ++)"
    "\n        downsize[i-1] = a[i];"
    "\n    return downsize;"
    "\n}\n";

const static std::string DELETE_AT_INDEX_Bool =
    "\nint*"
    "\ndeleteAtIndexBool(int* a, int a_size, int index)"
    "\n{"
    "\n    if (a_size == 0 || a_size - 1 < index || index < 0)"
    "\n        return a;"
    "\n"
    "\n    int* downsize = malloc((a_size - 1) * sizeof(int));"
    "\n    for (int i = 0; i < index; i ++)"
    "\n        downsize[i] = a[i];"
    "\n    for (int i = index + 1; i < a_size; i ++)"
    "\n        downsize[i-1] = a[i];"
    "\n    return downsize;"
    "\n}\n";

const static std::string DELETE_AT_INDEX_Flt =
    "\nfloat*"
    "\ndeleteAtIndexFlt(float* a, int a_size, int index)"
    "\n{"
    "\n    if (a_size == 0 || a_size - 1 < index || index < 0)"
    "\n        return a;"
    "\n"
    "\n    float* downsize = malloc((a_size - 1) * sizeof(float));"
    "\n    for (int i = 0; i < index; i ++)"
    "\n        downsize[i] = a[i];"
    "\n    for (int i = index + 1; i < a_size; i ++)"
    "\n        downsize[i-1] = a[i];"
    "\n    return downsize;"
    "\n}\n";

const static std::string GET_AT_INDEX_INT = "\nint"
                                            "\ngetAtIndexInt(int* a, int index)"
                                            "\n{"
                                            "\n    return a[index];"
                                            "\n}\n";

const static std::string GET_AT_INDEX_BOOL =
    "\nint"
    "\ngetAtIndexBool(int* a, int index)"
    "\n{"
    "\n    return a[index];"
    "\n}\n";

const static std::string GET_AT_INDEX_STR =
    "\nchar*"
    "\ngetAtIndexStr(char** a, int index)"
    "\n{"
    "\n    return a[index];"
    "\n}\n";

const static std::string GET_AT_INDEX_FLT =
    "\nfloat"
    "\ngetAtIndexFlt(float* a, int index)"
    "\n{"
    "\n    return a[index];"
    "\n}\n";

const static std::string SET_AT_INDEX_INT =
    "\nint*"
    "\nsetAtIndexInt(int* a, int index, int value)"
    "\n{"
    "\n    a[index] = value;"
    "\n    return a;"
    "\n}\n";

const static std::string SET_AT_INDEX_BOOL =
    "\nint*"
    "\nsetAtIndexBool(int* a, int index, int value)"
    "\n{"
    "\n    a[index] = value;"
    "\n    return a;"
    "\n}\n";

const static std::string SET_AT_INDEX_FLT =
    "\nfloat*"
    "\nsetAtIndexFlt(float* a, int index, float value)"
    "\n{"
    "\n    a[index] = value;"
    "\n    return a;"
    "\n}\n";

const static std::string SET_AT_INDEX_STR =
    "\nchar**"
    "\nsetAtIndexStr(char** a, int index, char* value)"
    "\n{"
    "\n    a[index] = value;"
    "\n    return a;"
    "\n}\n";

const static std::string PRINT_INT = "\nvoid"
                                     "\nprintInt(int i)"
                                     "\n{"
                                     "\n    printf(\"%d\\n\", i);"
                                     "\n}\n";

const static std::string PRINT_STR = "\nvoid"
                                     "\nprintStr(char* s)"
                                     "\n{"
                                     "\n    printf(\"%s\\n\", s);"
                                     "\n}\n";

const static std::string PRINT_BOOL = "\nvoid"
                                      "\nprintBool(int i)"
                                      "\n{"
                                      "\n    if (i == 0)"
                                      "\n       printf(\"false\\n\");"
                                      "\n    else"
                                      "\n       printf(\"true\\n\");"
                                      "\n}\n";

const static std::string PRINT_FLT = "\nvoid"
                                     "\nprintFlt(float f)"
                                     "\n{"
                                     "\n    printf(\"%f\\n\", f);"
                                     "\n}\n";

std::string getListsStdLibC();