#include <Python.h>

#include "include/sllnode.h"

static PyMethodDef cdsl_methods[] = {
    { NULL }, /* sentinel */
};

static struct PyModuleDef cdsl_moduledef = {
    PyModuleDef_HEAD_INIT,
    .m_name = "cdsl",
    .m_doc = "Common Data Structures Library.",
    .m_size = -1,
    cdsl_methods,
};

PyMODINIT_FUNC
PyInit_cdsl(void)
{
    PyObject* cdsl;

    if(!sll_init_type())
        return NULL;

    cdsl = PyModule_Create(&cdsl_moduledef);

    if (cdsl==NULL)
        return NULL;

    if(!sll_reg_type(cdsl))
        return NULL;

    return cdsl;
}
