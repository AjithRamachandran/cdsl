#ifndef STACK_H
#define STACK_H

#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <structmember.h>

#include "slnode.h"

/* ****************************************************************************** */
/*                                      Stack                                     */
/* ****************************************************************************** */

typedef struct {
    PyObject_HEAD
    SlNodeObject *top;
    int size;
} StackObject;

static PyObject *
stack_push(StackObject *self, PyObject *value) {

	SlNodeObject *new_node = NULL;

	new_node = slnode_create(self->top, value);
    self->top = new_node;
    self->size++;
    
    Py_XINCREF(new_node);
	Py_RETURN_NONE;
}

static PyObject *
stack_pop(StackObject *self) {
    SlNodeObject *node = NULL;
    PyObject *value;

    if (self->top == Py_None) {
        return Py_None;
    }

    value = self->top->value;
    node = self->top->next;
    slnode_delete(self->top, Py_None);
    self->top = node;

    self->size--;

    Py_XDECREF(node);
    return value;
}

static PyObject *
stack_peek(StackObject *self) {
    PyObject *ret=NULL;

    if(self->top==Py_None)
        Py_RETURN_NONE;

    ret = self->top->value;

    return ret;
}

static PyObject *
StackObject_Repr(StackObject *self) {
    SlNodeObject *current_node = self->top;
    PyObject *ret = NULL;

    ret = PyUnicode_FromString("stack([");

    if (Py_ReprEnter((PyObject *)self) > 0)
        return PyUnicode_FromString("stack(<...>)");

    if (current_node != Py_None) {
        while (current_node != Py_None) {
            ret = PyUnicode_Concat(ret, PyUnicode_FromFormat("%U", PyObject_Repr(current_node->value)));
            if (current_node->next != Py_None)
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
StackObject_Str(StackObject *self) {
    SlNodeObject *current_node = self->top;
    PyObject *ret = NULL;

    ret = PyUnicode_FromString("stack([");

    if (Py_ReprEnter((PyObject *)self) > 0)
        return PyUnicode_FromString("stack(<...>)");

    if (current_node != Py_None) {
        while (current_node != Py_None) {
            ret = PyUnicode_Concat(ret, PyUnicode_FromFormat("%U", PyObject_Str(current_node->value)));
            if (current_node->next != Py_None)
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
StackObject_Traverse(StackObject *self, visitproc visit, void *arg) {
    SlNodeObject *node = self->top;

    while (node != Py_None) {
        SlNodeObject *next_node = node->next;
        Py_VISIT(node);
        node = next_node;
    }

    Py_XDECREF(node);
    return 0;
}

static int
StackObject_Clear(StackObject *self) {
    SlNodeObject *node = self->top;

    self->top = NULL;

    while (node != Py_None) {
        SlNodeObject *next_node = node->next;
        node->next = Py_None;
        Py_DECREF(node);
        node = next_node;
    }

    return 0;
}

static void
StackObject_dealloc(StackObject *self) {
    Py_XDECREF(self->top);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyObject *
StackObject_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    StackObject *self;

    self = (StackObject *)type->tp_alloc(type, 0);
    
    if (self == NULL) return NULL;

    Py_INCREF(Py_None);

    self->top = Py_None;
    self->size = 0;

    return (PyObject *)self;
}

static int
StackObject_init(StackObject *self, PyObject *args, PyObject *kwds) {
    self->top=Py_None;
    self->size=0;

    return 0;
}

static PyMethodDef StackObjectMethods[] = {
    {"push", (PyCFunction)stack_push, METH_O, "Insert element to the top"},
    {"pop", (PyCFunction)stack_pop, METH_NOARGS, "Delete node from top"},
    {"peek", (PyCFunction)stack_peek, METH_NOARGS, "get value at top node"},
    {NULL},
};

static PyMemberDef StackObject_members[] = {
    {"size", T_INT, offsetof(StackObject, size), READONLY, "size of stack"},
    {NULL},
};

static PyTypeObject StackType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "cdsl.stack",
    .tp_doc = "Stack object",
    .tp_basicsize = sizeof(StackObject),
    .tp_new = StackObject_new,
    .tp_init = (initproc)StackObject_init,
    .tp_dealloc = (destructor)StackObject_dealloc,
    .tp_repr = (reprfunc)StackObject_Repr,
    .tp_str = (reprfunc)StackObject_Str,
    .tp_traverse = (traverseproc)StackObject_Traverse,
    .tp_clear = (inquiry)StackObject_Clear,
    .tp_methods = StackObjectMethods,
    .tp_members = StackObject_members,
};

/* ****************************************************************************** */
/*                                  Register Stack                                */
/* ****************************************************************************** */

int stack_init_type(void) {
    if (PyType_Ready(&StackType) < 0)
        return 0;
    return 1;
}

int stack_reg_type(PyObject *module) {
    Py_INCREF(&StackType);
    if (PyModule_AddObject(module, "stack", (PyObject *)&StackType) < 0) {
        Py_DECREF(&StackType);
        Py_DECREF(module);
        return 0;
    }
    return 1;
}

#endif // STACK_H
