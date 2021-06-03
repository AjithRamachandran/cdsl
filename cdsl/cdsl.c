#include <Python.h>

static PyMethodDef cdsl_methods[] = {
    { NULL }
};

static struct PyModuleDef cdsl_moduledef = {
    PyModuleDef_HEAD_INIT,
    "cdsl",
    "Common Data Structures Library.",
    -1,
    cdsl_methods,
};

PyMODINIT_FUNC
PyInit_cdsl(void)
{
    PyObject* cdsl;
    cdsl = PyModule_Create(&cdsl_moduledef);

    return cdsl;
}
