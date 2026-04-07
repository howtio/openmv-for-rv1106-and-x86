#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "imlib.h"
#include "collections.h"

typedef struct {
    PyObject_HEAD
    image_t img;
    int owns_data;
} PyImageObject;

static void PyImage_dealloc(PyImageObject *self)
{
    if (self->owns_data && self->img.data) {
        free(self->img.data);
        self->img.data = NULL;
    }
    Py_TYPE(self)->tp_free((PyObject *) self);
}

static int PyImage_init(PyImageObject *self, PyObject *args, PyObject *kwds)
{
    const char *path = NULL;
    static char *kwlist[] = {"path", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "s", kwlist, &path)) {
        return -1;
    }

    int w = 0, h = 0, ch = 0;
    unsigned char *img_data = stbi_load(path, &w, &h, &ch, 1); // 强制灰度
    if (!img_data) {
        PyErr_Format(PyExc_RuntimeError, "load jpg failed: %s", path);
        return -1;
    }

    memset(&self->img, 0, sizeof(self->img));
    self->img.w = w;
    self->img.h = h;
    self->img.bpp = 1;
    self->img.pixfmt = PIXFORMAT_GRAYSCALE;
    self->img.data = img_data;
    self->owns_data = 1;
    return 0;
}

static PyObject *PyImage_width(PyImageObject *self, PyObject *Py_UNUSED(ignored))
{
    return PyLong_FromLong(self->img.w);
}

static PyObject *PyImage_height(PyImageObject *self, PyObject *Py_UNUSED(ignored))
{
    return PyLong_FromLong(self->img.h);
}

static int parse_roi(PyObject *obj, rectangle_t *roi, int default_w, int default_h)
{
    if (obj == Py_None) {
        roi->x = 0;
        roi->y = 0;
        roi->w = default_w;
        roi->h = default_h;
        return 0;
    }

    if (!PyTuple_Check(obj) || PyTuple_Size(obj) != 4) {
        PyErr_SetString(PyExc_TypeError, "roi must be (x, y, w, h)");
        return -1;
    }

    roi->x = (int) PyLong_AsLong(PyTuple_GetItem(obj, 0));
    roi->y = (int) PyLong_AsLong(PyTuple_GetItem(obj, 1));
    roi->w = (int) PyLong_AsLong(PyTuple_GetItem(obj, 2));
    roi->h = (int) PyLong_AsLong(PyTuple_GetItem(obj, 3));

    if (PyErr_Occurred()) return -1;
    return 0;
}

static PyObject *PyImage_find_circles(PyImageObject *self, PyObject *args, PyObject *kwds)
{
    PyObject *roi_obj = Py_None;
    int x_stride = 2;
    int y_stride = 1;
    int threshold = 2000;
    int x_margin = 10;
    int y_margin = 10;
    int r_margin = 10;
    int r_min = 2;
    int r_max = -1;
    int r_step = 2;

    static char *kwlist[] = {
        "roi",
        "x_stride",
        "y_stride",
        "threshold",
        "x_margin",
        "y_margin",
        "r_margin",
        "r_min",
        "r_max",
        "r_step",
        NULL
    };

    if (!PyArg_ParseTupleAndKeywords(
            args, kwds, "|Oiiiiiiiii", kwlist,
            &roi_obj,
            &x_stride, &y_stride, &threshold,
            &x_margin, &y_margin, &r_margin,
            &r_min, &r_max, &r_step)) {
        return NULL;
    }

    rectangle_t roi;
    if (parse_roi(roi_obj, &roi, self->img.w, self->img.h) != 0) {
        return NULL;
    }

    if (r_max < 0) {
        r_max = (self->img.w < self->img.h ? self->img.w : self->img.h) / 2;
    }

    list_t out_list;
    list_init(&out_list, sizeof(find_circles_list_lnk_data_t));

    imlib_find_circles(&out_list, &self->img, &roi,
                       x_stride, y_stride,
                       threshold,
                       x_margin, y_margin, r_margin,
                       r_min, r_max, r_step);

    PyObject *ret = PyList_New(0);
    if (!ret) return NULL;

    while (list_size(&out_list) > 0) {
        find_circles_list_lnk_data_t c;
        list_pop_front(&out_list, &c);

        // 先返回最小结果：(x, y, r)
        PyObject *item = Py_BuildValue("(iii)", c.p.x, c.p.y, c.r);
        if (!item) {
            Py_DECREF(ret);
            return NULL;
        }
        PyList_Append(ret, item);
        Py_DECREF(item);
    }

    return ret;
}

static PyMethodDef PyImage_methods[] = {
    {"width",  (PyCFunction) PyImage_width,  METH_NOARGS, "width()"},
    {"height", (PyCFunction) PyImage_height, METH_NOARGS, "height()"},
    {
        "find_circles",
        (PyCFunction) PyImage_find_circles,
        METH_VARARGS | METH_KEYWORDS,
        "find_circles(roi=None, x_stride=2, y_stride=1, threshold=2000, "
        "x_margin=10, y_margin=10, r_margin=10, r_min=2, r_max=None, r_step=2)"
    },
    {NULL, NULL, 0, NULL}
};

static PyTypeObject PyImageType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "image.Image",
    .tp_basicsize = sizeof(PyImageObject),
    .tp_itemsize = 0,
    .tp_dealloc = (destructor) PyImage_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Minimal OpenMV-like Image object",
    .tp_methods = PyImage_methods,
    .tp_init = (initproc) PyImage_init,
    .tp_new = PyType_GenericNew,
};

static PyMethodDef image_module_methods[] = {
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef image_module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "image",
    .m_doc = "Minimal OpenMV-like image module",
    .m_size = -1,
    .m_methods = image_module_methods,
};

PyMODINIT_FUNC PyInit_image(void)
{
    if (PyType_Ready(&PyImageType) < 0) {
        return NULL;
    }

    PyObject *m = PyModule_Create(&image_module);
    if (!m) {
        return NULL;
    }

    Py_INCREF(&PyImageType);
    if (PyModule_AddObject(m, "Image", (PyObject *) &PyImageType) < 0) {
        Py_DECREF(&PyImageType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}