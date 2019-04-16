/*
 * Robert Durst 2019
 * Sailfish Programming Language
 */
#include "Lists.h"
std::string
getListsStdLibC()
{
    return APPEND_LIST_INT + APPEND_LIST_STR + APPEND_LIST_BOOL +
           APPEND_LIST_FLT + DELETE_AT_INDEX_INT + DELETE_AT_INDEX_Flt +
           DELETE_AT_INDEX_Bool + DELETE_AT_INDEX_STR + GET_AT_INDEX_INT +
           GET_AT_INDEX_FLT + GET_AT_INDEX_STR + GET_AT_INDEX_BOOL +
           SET_AT_INDEX_INT + SET_AT_INDEX_BOOL + SET_AT_INDEX_FLT +
           SET_AT_INDEX_STR + PRINT_BOOL + PRINT_FLT + PRINT_INT + PRINT_STR;
}