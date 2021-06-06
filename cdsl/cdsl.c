#define PY_SSIZE_T_CLEAN

#include <Python.h>

#include "include/slnode.h"
#include "include/stack.h"
#include "include/queue.h"
#include "include/sll.h"

static PyMethodDef cdsl_methods[] = {
    { NULL },
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

    if(!slnode_init_type())
        return NULL;

    if(!sll_init_type())
        return NULL;

    if(!stack_init_type())
        return NULL;
    
    if(!queue_init_type())
        return NULL;

    cdsl = PyModule_Create(&cdsl_moduledef);

    if (cdsl==NULL)
        return NULL;

    if(!slnode_reg_type(cdsl))
        return NULL;

    if(!sll_reg_type(cdsl))
        return NULL;

    if(!stack_reg_type(cdsl))
        return NULL;

    if(!queue_reg_type(cdsl))
        return NULL;

    return cdsl;
}
