#ifndef QUEUE_H
#define QUEUE_H

#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <structmember.h>

#include "slnode.h"

/* ****************************************************************************** */
/*                                      queue                                     */
/* ****************************************************************************** */

typedef struct {
    PyObject_HEAD
    SlNodeObject *front;
    SlNodeObject *rear;
    int size;
} QueueObject;

static PyObject *
queue_enqueue(QueueObject *self, PyObject *value) {

	SlNodeObject *new_node = NULL;

    if (self->rear == Py_None) {
        new_node = slnode_create(Py_None, value);
        self->front = new_node;
        self->rear = new_node;
        self->size++;
        Py_RETURN_NONE;
    }

	new_node = slnode_create(Py_None, value);
    self->rear->next = new_node;
    self->rear = new_node;
    self->size++;
    
    Py_XINCREF(new_node);
	Py_RETURN_NONE;
}

static PyObject *
queue_dequeue(QueueObject *self) {
    SlNodeObject *node = NULL;
    PyObject *value;

    if (self->front == Py_None) {
        return Py_None;
    }

    value = self->front->value;
    node = self->front->next;
    slnode_delete(self->front, Py_None);
    self->front = node;

    self->size--;

    Py_XDECREF(node);
    return value;
}

static PyObject *
queue_peek(QueueObject *self) {
    PyObject *ret=NULL;

    if(self->front==Py_None)
        Py_RETURN_NONE;

    ret = self->front->value;

    return ret;
}

static PyObject *
QueueObject_Repr(QueueObject *self) {
    SlNodeObject *current_node = self->front;
    PyObject *ret = NULL;

    ret = PyUnicode_FromString("queue([");

    if (Py_ReprEnter((PyObject *)self) > 0)
        return PyUnicode_FromString("queue(<...>)");

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
QueueObject_Str(QueueObject *self) {
    SlNodeObject *current_node = self->front;
    PyObject *ret = NULL;

    ret = PyUnicode_FromString("queue([");

    if (Py_ReprEnter((PyObject *)self) > 0)
        return PyUnicode_FromString("queue(<...>)");

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
QueueObject_Traverse(QueueObject *self, visitproc visit, void *arg) {
    SlNodeObject *node = self->front;

    while (node != Py_None) {
        SlNodeObject *next_node = node->next;
        Py_VISIT(node);
        node = next_node;
    }

    Py_XDECREF(node);
    return 0;
}

static int
QueueObject_Clear(QueueObject *self) {
    SlNodeObject *node = self->front;

    self->front = NULL;
    self->rear = NULL;

    while (node != Py_None) {
        SlNodeObject *next_node = node->next;
        node->next = Py_None;
        Py_DECREF(node);
        node = next_node;
    }

    return 0;
}

static void
QueueObject_dealloc(QueueObject *self) {
    Py_XDECREF(self->front);
    Py_XDECREF(self->rear);
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyObject *
QueueObject_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    QueueObject *self;

    self = (QueueObject *)type->tp_alloc(type, 0);
    
    if (self == NULL) return NULL;

    Py_INCREF(Py_None);

    self->front = Py_None;
    self->rear = Py_None;
    self->size = 0;

    return (PyObject *)self;
}

static int
QueueObject_init(QueueObject *self, PyObject *args, PyObject *kwds) {
    self->front=Py_None;
    self->rear=Py_None;
    self->size=0;

    return 0;
}

static PyMethodDef QueueObjectMethods[] = {
    {"enqueue", (PyCFunction)queue_enqueue, METH_O, "Insert element to the rear"},
    {"dequeue", (PyCFunction)queue_dequeue, METH_NOARGS, "Delete node from front"},
    {"peek", (PyCFunction)queue_peek, METH_NOARGS, "get value at front node"},
    {NULL},
};

static PyMemberDef QueueObject_members[] = {
    {"size", T_INT, offsetof(QueueObject, size), READONLY, "size of queue"},
    {NULL},
};

static PyTypeObject QueueType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "cdsl.queue",
    .tp_doc = "queue object",
    .tp_basicsize = sizeof(QueueObject),
    .tp_new = QueueObject_new,
    .tp_init = (initproc)QueueObject_init,
    .tp_dealloc = (destructor)QueueObject_dealloc,
    .tp_repr = (reprfunc)QueueObject_Repr,
    .tp_str = (reprfunc)QueueObject_Str,
    .tp_traverse = (traverseproc)QueueObject_Traverse,
    .tp_clear = (inquiry)QueueObject_Clear,
    .tp_methods = QueueObjectMethods,
    .tp_members = QueueObject_members,
};

/* ****************************************************************************** */
/*                                  Register queue                                */
/* ****************************************************************************** */

int queue_init_type(void) {
    if (PyType_Ready(&QueueType) < 0)
        return 0;
    return 1;
}

int queue_reg_type(PyObject *module) {
    Py_INCREF(&QueueType);
    if (PyModule_AddObject(module, "queue", (PyObject *)&QueueType) < 0) {
        Py_DECREF(&QueueType);
        Py_DECREF(module);
        return 0;
    }
    return 1;
}

#endif // QUEUE_H
