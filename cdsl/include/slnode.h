#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <structmember.h>

static PyTypeObject SlNodeType;

/* ****************************************************************************** */
/*                               Singly Linked List Node                          */
/* ****************************************************************************** */

typedef struct {
    PyObject_HEAD
        PyObject *value;
    struct SlNodeObject *next;
} SlNodeObject;

static void
slnode_delete(SlNodeObject *node, SlNodeObject *prev) {
    if (prev != Py_None)
        prev->next = node->next;

    Py_XINCREF(Py_None);
    node->next = Py_None;

    Py_XDECREF(node);
}

static SlNodeObject *
slnode_create(SlNodeObject *next, PyObject *value) {
    SlNodeObject *new_node = (SlNodeObject *)PyObject_CallObject((PyObject *)&SlNodeType, PyTuple_New(1));

    Py_XINCREF(value);
    new_node->value = value;
    new_node->next = next;

    Py_XINCREF(new_node);
    return new_node;
}

static PyObject *
SlNodeObject_Repr(SlNodeObject *self) {
    PyObject *ret = NULL;

    if (Py_ReprEnter((PyObject *)self) > 0)
        return PyUnicode_FromString("slnode(<...>)");

    ret = PyUnicode_FromFormat("slnode([%U])", PyObject_Str(self->value));

    Py_ReprLeave((PyObject *)self);
    return ret;
}

static PyObject *
SlNodeObject_Str(SlNodeObject *self) {
    PyObject *ret = NULL;

    if (Py_ReprEnter((PyObject *)self) > 0)
        return PyUnicode_FromString("slnode(<...>)");

    ret = PyUnicode_FromFormat("slnode([%U])", PyObject_Str(self->value));

    Py_ReprLeave((PyObject *)self);
    return ret;
}

SlNodeObject_Traverse(SlNodeObject *self, visitproc visit, void *arg) {
    Py_VISIT(self->value);
    return 0;
}

static int
SlNodeObject_Clear(SlNodeObject *self) {
    Py_TYPE(self)->tp_free((PyObject *)self);
    return 0;
}

static void
slNodeObject_dealloc(SlNodeObject *self) {
    Py_XDECREF(self->value);
    Py_XDECREF(self->next);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyObject *
SlNodeObject_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    SlNodeObject *self;
    self = (SlNodeObject *)type->tp_alloc(type, 0);
    if (self != NULL) {
        Py_INCREF(Py_None);

        self->value = Py_None;
        self->next = Py_None;

        Py_INCREF(self->value);

        return (PyObject *)self;
    }

    return NULL;
}

static int
slNodeObject_init(SlNodeObject *self, PyObject *args, PyObject *kwds) {
    PyObject *value, *tmp;

    if (!PyArg_ParseTuple(args, "O", &value))
        return -1;

    if (value) {
        tmp = self->value;
        Py_INCREF(value);
        self->value = value;
        Py_XDECREF(tmp);
    }

    return 0;
}

static PyMemberDef SlNodeObject_members[] = {
    {"value", T_OBJECT_EX, offsetof(SlNodeObject, value), 0, "value"},
    {"next", T_OBJECT_EX, offsetof(SlNodeObject, next), READONLY, "next SlnodeObject"},
    {NULL}};

static PyTypeObject SlNodeType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "cdsl.slnode",
    .tp_doc = "SlNode object",
    .tp_basicsize = sizeof(SlNodeObject),
    .tp_new = SlNodeObject_new,
    .tp_init = (initproc)slNodeObject_init,
    .tp_dealloc = (destructor)slNodeObject_dealloc,
    .tp_repr = (reprfunc)SlNodeObject_Repr,
    .tp_str = (reprfunc)SlNodeObject_Str,
    .tp_traverse = (traverseproc)SlNodeObject_Traverse,
    .tp_clear = (inquiry)SlNodeObject_Clear,
    .tp_members = SlNodeObject_members,
};

/* ****************************************************************************** */
/*                              Register Singly Linked Node                       */
/* ****************************************************************************** */

int slnode_init_type(void) {
    if (PyType_Ready(&SlNodeType) < 0)
        return 0;
    return 1;
}

int slnode_reg_type(PyObject *module) {
    Py_INCREF(&SlNodeType);
    if (PyModule_AddObject(module, "slnode", (PyObject *)&SlNodeType) < 0) {
        Py_DECREF(&SlNodeType);
        Py_DECREF(module);
        return 0;
    }
    return 1;
}
