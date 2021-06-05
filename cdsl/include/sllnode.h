#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <structmember.h>

static PyTypeObject SllType;
static PyTypeObject SllNodeType;

/* ****************************************************************************** */
/*                               Singly Linked List Node                          */
/* ****************************************************************************** */

typedef struct {
    PyObject_HEAD
        PyObject *value;
    struct SllNodeObject *next;
} SllNodeObject;

static void
sllnode_delete(SllNodeObject *node, SllNodeObject *prev) {
    if (prev != Py_None)
        prev->next = node->next;

    Py_XINCREF(Py_None);
    node->next = Py_None;

    Py_XDECREF(node);
}

static SllNodeObject *
sllnode_create(SllNodeObject *next, PyObject *value) {
    SllNodeObject *new_node = (SllNodeObject *)PyObject_CallObject((PyObject *)&SllNodeType, PyTuple_New(1));

    Py_XINCREF(value);
    new_node->value = value;
    new_node->next = next;

    Py_XINCREF(new_node);
    return new_node;
}

static PyObject *
SllNodeObject_Repr(SllNodeObject *self) {
    PyObject *ret = NULL;

    if (Py_ReprEnter((PyObject *)self) > 0)
        return PyUnicode_FromString("sllnode(<...>)");

    ret = PyUnicode_FromFormat("sllnode([%U])", PyObject_Str(self->value));

    Py_ReprLeave((PyObject *)self);
    return ret;
}

static PyObject *
SllNodeObject_Str(SllNodeObject *self) {
    PyObject *ret = NULL;

    if (Py_ReprEnter((PyObject *)self) > 0)
        return PyUnicode_FromString("sllnode(<...>)");

    ret = PyUnicode_FromFormat("sllnode([%U])", PyObject_Str(self->value));

    Py_ReprLeave((PyObject *)self);
    return ret;
}

SllNodeObject_Traverse(SllNodeObject *self, visitproc visit, void *arg) {
    Py_VISIT(self->value);
    return 0;
}

static int
SllNodeObject_Clear(SllNodeObject *self) {
    Py_TYPE(self)->tp_free((PyObject *)self);
    return 0;
}

static void
sllNodeObject_dealloc(SllNodeObject *self) {
    Py_XDECREF(self->value);
    Py_XDECREF(self->next);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyObject *
SllNodeObject_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    SllNodeObject *self;
    self = (SllNodeObject *)type->tp_alloc(type, 0);
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
sllNodeObject_init(SllNodeObject *self, PyObject *args, PyObject *kwds) {
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

static PyMemberDef SllNodeObject_members[] = {
    {"value", T_OBJECT_EX, offsetof(SllNodeObject, value), 0, "value"},
    {"next", T_OBJECT_EX, offsetof(SllNodeObject, next), READONLY, "next SllnodeObject"},
    {NULL}};

static PyTypeObject SllNodeType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "cdsl.sllnode",
    .tp_doc = "SllNode object",
    .tp_basicsize = sizeof(SllNodeObject),
    .tp_new = SllNodeObject_new,
    .tp_init = (initproc)sllNodeObject_init,
    .tp_dealloc = (destructor)sllNodeObject_dealloc,
    .tp_repr = (reprfunc)SllNodeObject_Repr,
    .tp_str = (reprfunc)SllNodeObject_Str,
    .tp_traverse = (traverseproc)SllNodeObject_Traverse,
    .tp_clear = (inquiry)SllNodeObject_Clear,
    .tp_members = SllNodeObject_members,
};

/* ****************************************************************************** */
/*                               Singly Linked List                               */
/* ****************************************************************************** */

typedef struct {
    PyObject_HEAD
        SllNodeObject *head;
    SllNodeObject *tail;
    int length;
} SllObject;

static PyObject *
SllObject_Repr(SllObject *self) {
    SllNodeObject *current_node = self->head;
    PyObject *ret = NULL;

    ret = PyUnicode_FromString("sll([");

    if (Py_ReprEnter((PyObject *)self) > 0)
        return PyUnicode_FromString("sll(<...>)");

    if (current_node != Py_None) {
        while (current_node != Py_None) {
            ret = PyUnicode_Concat(ret, PyUnicode_FromFormat("%U", PyObject_Repr(current_node->value)));
            if (current_node != self->tail)
                ret = PyUnicode_Concat(ret, PyUnicode_FromString(", "));
            current_node = current_node->next;
        }
    }
    ret = PyUnicode_Concat(ret, PyUnicode_FromString("])"));

    Py_XDECREF(current_node);
    Py_ReprLeave((PyObject *)self);
    return ret;
}

static PyObject *
SllObject_Str(SllObject *self) {
    SllNodeObject *current_node = self->head;
    PyObject *ret = NULL;

    ret = PyUnicode_FromString("sll([");

    if (Py_ReprEnter((PyObject *)self) > 0)
        return PyUnicode_FromString("sll(<...>)");

    if (current_node != Py_None) {
        while (current_node != Py_None) {
            ret = PyUnicode_Concat(ret, PyUnicode_FromFormat("%U", PyObject_Str(current_node->value)));
            if (current_node != self->tail)
                ret = PyUnicode_Concat(ret, PyUnicode_FromString(", "));
            current_node = current_node->next;
        }
    }
    ret = PyUnicode_Concat(ret, PyUnicode_FromString("])"));

    Py_XDECREF(current_node);
    Py_ReprLeave((PyObject *)self);
    return ret;
}

static int
SllObject_Traverse(SllObject *self, visitproc visit, void *arg) {
    SllNodeObject *node = self->head;

    while (node != Py_None) {
        SllNodeObject *next_node = node->next;
        Py_VISIT(node);
        node = next_node;
    }

    Py_XDECREF(node);
    return 0;
}

static int
SllObject_Clear(SllObject *self) {
    SllNodeObject *node = self->head;

    self->head = NULL;
    self->tail = NULL;

    while (node != Py_None) {
        SllNodeObject *next_node = node->next;
        node->next = Py_None;
        Py_DECREF(node);
        node = next_node;
    }

    return 0;
}

static SllNodeObject *
sll_search(SllObject *self, PyObject *value) {
    SllNodeObject *current_node = self->head;

    while(current_node != Py_None) {
        if(current_node->value == value){
            Py_DECREF(current_node);
            return current_node;
        }
        current_node = current_node->next;
    }

    Py_XDECREF(current_node);
    Py_RETURN_NONE;
}

static SllNodeObject *
sll_node_at(SllObject *self, PyObject *args) {
    SllNodeObject *current_node = self->head;
    int index;
    
    if (!PyArg_ParseTuple(args, "i", &index))
        return NULL;

    while(index--) {
        current_node = current_node->next;
    }

    Py_XDECREF(current_node);
    return current_node;
}

static void
sll_extend(SllObject *self, SllObject *arg) {
    SllNodeObject *current_node = arg->head, *node = NULL;

    /* Both are same ll */
    if (self->head == arg->head) {
        int pos = self->length;
        while (pos--) {
            node = sllnode_create(Py_None, current_node->value);
            self->tail->next = node;
            self->tail = node;
            current_node = current_node->next;
            self->length;
        }
    } else {
        while (current_node != Py_None) {
            node = sllnode_create(Py_None, current_node->value);
            if (self->head == Py_None) {
                self->head = node;
                self->tail = node;
            } else {
                self->tail->next = node;
                self->tail = node;
            }
            current_node = current_node->next;
            self->length++;
        }
    }

    Py_XDECREF(current_node);
    Py_RETURN_NONE;
}

static SllNodeObject *
get_prev_node(SllObject *self, SllNodeObject *node) {
    SllNodeObject *current_node = self->head;

    if (current_node != node) {
        while (current_node->next != Py_None) {
            if (current_node->next == node) {
                Py_XINCREF(current_node);
                return current_node;
            }
            current_node = current_node->next;
        }
    }

    Py_XINCREF(Py_None);
    return Py_None;
}

static PyObject *
sll_delete(SllObject *self, PyObject *args) {
    SllNodeObject *del_node = NULL, *prev = NULL;
    PyObject *value;
    int pos = self->length - 1, i;

    if (!PyArg_ParseTuple(args, "|i", &pos))
        return NULL;

    if (pos < 0 || pos >= self->length) {
        PyErr_SetString(PyExc_ValueError, "index out of range.");
        return NULL;
    }

    if (self->head == Py_None) {
        PyErr_SetString(PyExc_ValueError, "Empty List.");
        return NULL;
    }

    if (pos == 0) {
        value = self->head->value;
        prev = get_prev_node(self, self->head);
        del_node = self->head->next;
        sllnode_delete(self->head, prev);
        self->head = del_node;
    } else if (pos == self->length - 1 || pos == -1) {
        value = self->tail->value;
        prev = get_prev_node(self, self->tail);
        sllnode_delete(self->tail, prev);
        self->tail = prev;
    } else {
        del_node = self->head;
        for (i = 0; i < pos; i++) {
            del_node = del_node->next;
        }
        value = del_node->value;
        prev = get_prev_node(self, del_node);
        sllnode_delete(del_node, prev);
    }
    self->length--;

    Py_XDECREF(prev);
    Py_XDECREF(del_node);
    Py_XINCREF(value);
    return value;
}

static PyObject *
sll_delete_node(SllObject *self, SllNodeObject *node) {
    SllNodeObject *prev = NULL;
    PyObject *value;
    int pos = self->length - 1, i;

    if (self->head == Py_None) {
        PyErr_SetString(PyExc_ValueError, "Empty List.");
        return NULL;
    }

    value = node->value;
    prev = get_prev_node(self, node);
    sllnode_delete(node, prev);
    self->length--;

    Py_XDECREF(prev);
    Py_XINCREF(value);
    return value;
}

static void
sll_insert(SllObject *self, PyObject *args) {
    SllNodeObject *new_node = NULL, *current_node = NULL;
    PyObject *value;
    int pos = -1, i;

    if (!PyArg_ParseTuple(args, "O|i", &value, &pos))
        return NULL;

    if (pos != 0) {
        if ((pos < -1 || pos >= self->length)) {
            PyErr_SetString(PyExc_ValueError, "index out of range.");
            return NULL;
        }
    }

    if ((self->head == Py_None) && (pos != 0) && (pos != -1)) {
        PyErr_SetString(PyExc_ValueError, "Empty List. can only be added to start");
        return NULL;
    }

    if (self->head == Py_None) {
        new_node = sllnode_create(Py_None, value);
        self->head = new_node;
        self->tail = new_node;
        self->length++;
        Py_RETURN_NONE;
    }

    if (pos == 0) {
        new_node = sllnode_create(self->head, value);
        self->head = new_node;
    } else if (pos == self->length - 1 || pos == -1) {
        new_node = sllnode_create(Py_None, value);
        self->tail->next = new_node;
        self->tail = new_node;
    } else {
        current_node = self->head;
        for (i = 1; i < pos; i++) {
            current_node = current_node->next;
        }
        new_node = sllnode_create(current_node->next, value);
        current_node->next = new_node;
    }
    self->length++;

    Py_XDECREF(current_node);
    Py_XDECREF(value);

    Py_RETURN_NONE;
}

static void
sll_insert_node(SllObject *self, PyObject *args) {
    SllNodeObject *current_node = NULL, *node = NULL;
    PyObject *arg = NULL;
    int pos = -1, i;

    if (!PyArg_ParseTuple(args, "O|i", &arg, &pos))
        return NULL;

    node = (SllNodeObject *)arg;

    if (pos != 0) {
        if ((pos < -1 || pos >= self->length)) {
            PyErr_SetString(PyExc_ValueError, "index out of range.");
            return NULL;
        }
    }

    if ((self->head == Py_None) && (pos != 0) && (pos != -1)) {
        PyErr_SetString(PyExc_ValueError, "Empty List. can only be added to start");
        return NULL;
    }

    if (self->head == Py_None) {
        node->next = Py_None;
        self->head = node;
        self->tail = node;
        self->length++;
        Py_RETURN_NONE;
    }

    if (pos == 0) {
        node->next = self->head;
        self->head = node;
    } else if (pos == self->length - 1 || pos == -1) {
        node->next = Py_None;
        self->tail->next = node;
        self->tail = node;
    } else {
        current_node = self->head;
        for (i = 1; i < pos; i++) {
            current_node = current_node->next;
        }
        node->next = current_node->next;
        current_node->next = node;
    }
    self->length++;

    Py_XDECREF(current_node);
    Py_RETURN_NONE;
}

static void
sllObject_dealloc(SllObject *self) {
    Py_XDECREF(self->head);
    Py_XDECREF(self->tail);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyObject *
SllObject_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    SllObject *self;

    self = (SllObject *)type->tp_alloc(type, 0);

    if (self == NULL) return NULL;

    Py_INCREF(Py_None);

    self->head = Py_None;
    self->tail = Py_None;
    self->length = 0;

    return (PyObject *)self;
}

static int
sllObject_init(SllObject *self, PyObject *args, PyObject *kwds) {
    self->head = Py_None;
    self->tail = Py_None;
    self->length = 0;

    return 0;
}

static PyMethodDef SllObjectMethods[] = {
    {"insert", (PyCFunction)sll_insert, METH_VARARGS, "Insert element to the list"},
    {"insert_node", (PyCFunction)sll_insert_node, METH_VARARGS, "Insert node to the list"},
    {"delete", (PyCFunction)sll_delete, METH_VARARGS, "Delete element to the list"},
    {"delete_node", (PyCFunction)sll_delete_node, METH_O, "Delete node from the list"},
    {"extend", (PyCFunction)sll_extend, METH_O, "extend sll with another sll"},
    {"node_at", (PyCFunction)sll_node_at, METH_VARARGS, "get sll at given index"},
    {"search", (PyCFunction)sll_search, METH_O, "return first node with given value"},
    {NULL},
};

static PyMemberDef SllObject_members[] = {
    {"head", T_OBJECT_EX, offsetof(SllObject, head), READONLY, "First node"},
    {"tail", T_OBJECT_EX, offsetof(SllObject, tail), READONLY, "Next node"},
    {"length", T_INT, offsetof(SllObject, length), READONLY, "length"},
    {NULL},
};

static PyTypeObject SllType = {
    PyVarObject_HEAD_INIT(NULL, 0)
        .tp_name = "cdsl.sll",
    .tp_doc = "Sll object",
    .tp_basicsize = sizeof(SllObject),
    .tp_new = SllObject_new,
    .tp_init = (initproc)sllObject_init,
    .tp_dealloc = (destructor)sllObject_dealloc,
    .tp_repr = (reprfunc)SllObject_Repr,
    .tp_str = (reprfunc)SllObject_Str,
    .tp_traverse = (traverseproc)SllObject_Traverse,
    .tp_clear = (inquiry)SllObject_Clear,
    .tp_methods = SllObjectMethods,
    .tp_members = SllObject_members,
};

/* ****************************************************************************** */
/*                              Register Singly Linked List                       */
/* ****************************************************************************** */

int sll_init_type(void) {
    if (PyType_Ready(&SllType) < 0 || PyType_Ready(&SllNodeType) < 0)
        return 0;
    return 1;
}

int sll_reg_type(PyObject *module) {
    Py_INCREF(&SllType);
    Py_INCREF(&SllNodeType);
    if (PyModule_AddObject(module, "sllnode", (PyObject *)&SllNodeType) < 0) {
        Py_DECREF(&SllNodeType);
        Py_DECREF(module);
        return 0;
    }
    if (PyModule_AddObject(module, "sll", (PyObject *)&SllType) < 0) {
        Py_DECREF(&SllType);
        Py_DECREF(module);
        return 0;
    }
    return 1;
}
