/* Created By: Virgil Dupras
 * Created On: 2010-01-30
 * Copyright 2010 Hardcoded Software (http://www.hardcoded.net)
 *
 * This software is licensed under the "HS" License as described in the "LICENSE" file, 
 * which should be included with this package. The terms are also available at 
 * http://www.hardcoded.net/licenses/hs_license
 */

#include "common.h"

/* I know that there strtol out there, but it requires a pointer to
 * a char, which would in turn require me to buffer my chars around,
 * making the whole process slower.
 */
static long
xchar_to_long(char c)
{
    if ((c >= 48) && (c <= 57)) { /* 0-9 */
        return c - 48;
    }
    else if ((c >= 65) && (c <= 70)) { /* A-F */
        return c - 55;
    }
    else if ((c >= 97) && (c <= 102)) { /* a-f */
        return c - 87;
    }
    return 0;
}

static PyObject*
cache_string_to_colors(PyObject *self, PyObject *args)
{
    char *s;
    Py_ssize_t char_count, color_count, i;
    PyObject *result;
    
    if (!PyArg_ParseTuple(args, "s#", &s, &char_count)) {
        return NULL;
    }
    
    color_count = (char_count / 6);
    result = PyList_New(color_count);
    if (result == NULL) {
        return NULL;
    }
    
    for (i=0; i<color_count; i++) {
        long r, g, b;
        Py_ssize_t ci;
        PyObject *color_tuple;
        
        ci = i * 6;
        r = (xchar_to_long(s[ci]) << 4) + xchar_to_long(s[ci+1]);
        g = (xchar_to_long(s[ci+2]) << 4) + xchar_to_long(s[ci+3]);
        b = (xchar_to_long(s[ci+4]) << 4) + xchar_to_long(s[ci+5]);
        
        color_tuple = inttuple(3, r, g, b);
        if (color_tuple == NULL) {
            Py_DECREF(result);
            return NULL;
        }
        PyList_SET_ITEM(result, i, color_tuple);
    }
    
    return result;
}

static PyMethodDef CacheMethods[] = {
    {"string_to_colors",  cache_string_to_colors, METH_VARARGS,
     "Transform the string 's' in a list of 3 sized tuples."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
init_cache(void)
{
    (void)Py_InitModule("_cache", CacheMethods);
}