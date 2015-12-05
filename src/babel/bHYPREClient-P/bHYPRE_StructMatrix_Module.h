/*
 * File:          bHYPRE_StructMatrix_Module.h
 * Symbol:        bHYPRE.StructMatrix-v1.0.0
 * Symbol Type:   class
 * Babel Version: 1.0.0
 * Description:   expose a constructor for the Python wrapper
 * 
 * WARNING: Automatically generated; only changes within splicers preserved
 * 
 */

/*
 * THIS CODE IS AUTOMATICALLY GENERATED BY THE BABEL
 * COMPILER. DO NOT EDIT THIS!
 * 
 * External clients need an entry point to wrap a pointer
 * to an instance of bHYPRE.StructMatrix.
 * This header files defines two methods that such clients
 * will need.
 *     bHYPRE_StructMatrix__import
 *         This should be called in the client's init
 *         module method.
 *     bHYPRE_StructMatrix__wrap
 *         This will wrap an IOR in a Python object.
 */

#ifndef included_bHYPRE_StructMatrix_MODULE
#define included_bHYPRE_StructMatrix_MODULE

#include <Python.h>
#include "sidlType.h"
#ifdef HAVE_PTHREAD
#include <pthread.h>
#endif /* HAVE_PTHREAD */

#ifdef __cplusplus
extern "C" {
#endif

struct sidl__array;

/* Forward declaration of IOR structure */
struct bHYPRE_StructMatrix__object;
struct bHYPRE_StructMatrix__array;
struct sidl_BaseInterface__object;

#define bHYPRE_StructMatrix__wrap_NUM 0
#define bHYPRE_StructMatrix__wrap_RETURN PyObject *
#define bHYPRE_StructMatrix__wrap_PROTO (struct bHYPRE_StructMatrix__object *sidlobj)

#define bHYPRE_StructMatrix__convert_NUM 1
#define bHYPRE_StructMatrix__convert_RETURN int
#define bHYPRE_StructMatrix__convert_PROTO (PyObject *obj, struct bHYPRE_StructMatrix__object **sidlobj)

#define bHYPRE_StructMatrix__convert_python_array_NUM 2
#define bHYPRE_StructMatrix__convert_python_array_RETURN int
#define bHYPRE_StructMatrix__convert_python_array_PROTO (PyObject *obj, struct bHYPRE_StructMatrix__array **sidlarray)

#define bHYPRE_StructMatrix__convert_sidl_array_NUM 3
#define bHYPRE_StructMatrix__convert_sidl_array_RETURN PyObject *
#define bHYPRE_StructMatrix__convert_sidl_array_PROTO (struct sidl__array *sidlarray)

#define bHYPRE_StructMatrix__weakRef_NUM 4
#define bHYPRE_StructMatrix__weakRef_RETURN PyObject *
#define bHYPRE_StructMatrix__weakRef_PROTO (struct bHYPRE_StructMatrix__object *sidlobj)

#define bHYPRE_StructMatrix_deref_NUM 5
#define bHYPRE_StructMatrix_deref_RETURN void
#define bHYPRE_StructMatrix_deref_PROTO (struct bHYPRE_StructMatrix__object *sidlobj)

#define bHYPRE_StructMatrix__newRef_NUM 6
#define bHYPRE_StructMatrix__newRef_RETURN PyObject *
#define bHYPRE_StructMatrix__newRef_PROTO (struct bHYPRE_StructMatrix__object *sidlobj)

#define bHYPRE_StructMatrix__addRef_NUM 7
#define bHYPRE_StructMatrix__addRef_RETURN void
#define bHYPRE_StructMatrix__addRef_PROTO (struct bHYPRE_StructMatrix__object *sidlobj)

#define bHYPRE_StructMatrix_PyType_NUM 8
#define bHYPRE_StructMatrix_PyType_RETURN PyTypeObject *
#define bHYPRE_StructMatrix_PyType_PROTO (void)

#define bHYPRE_StructMatrix__connectI_NUM 9
#define bHYPRE_StructMatrix__connectI_RETURN struct bHYPRE_StructMatrix__object* 
#define bHYPRE_StructMatrix__connectI_PROTO (const char* url, sidl_bool ar, struct sidl_BaseInterface__object ** _ex)

#define bHYPRE_StructMatrix__rmicast_NUM 10
#define bHYPRE_StructMatrix__rmicast_RETURN struct bHYPRE_StructMatrix__object* 
#define bHYPRE_StructMatrix__rmicast_PROTO (void* bi, struct sidl_BaseInterface__object ** _ex)

#define bHYPRE_StructMatrix__API_NUM 11

#ifdef bHYPRE_StructMatrix_INTERNAL

#define bHYPRE_StructMatrix__import() ;


/*
 * This declaration is not for clients.
 */

static bHYPRE_StructMatrix__wrap_RETURN
bHYPRE_StructMatrix__wrap
bHYPRE_StructMatrix__wrap_PROTO;

static bHYPRE_StructMatrix__convert_RETURN
bHYPRE_StructMatrix__convert
bHYPRE_StructMatrix__convert_PROTO;

static bHYPRE_StructMatrix__convert_python_array_RETURN
bHYPRE_StructMatrix__convert_python_array
bHYPRE_StructMatrix__convert_python_array_PROTO;

static bHYPRE_StructMatrix__convert_sidl_array_RETURN
bHYPRE_StructMatrix__convert_sidl_array
bHYPRE_StructMatrix__convert_sidl_array_PROTO;

static bHYPRE_StructMatrix__weakRef_RETURN
bHYPRE_StructMatrix__weakRef
bHYPRE_StructMatrix__weakRef_PROTO;

static bHYPRE_StructMatrix_deref_RETURN
bHYPRE_StructMatrix_deref
bHYPRE_StructMatrix_deref_PROTO;

static bHYPRE_StructMatrix__newRef_RETURN
bHYPRE_StructMatrix__newRef
bHYPRE_StructMatrix__newRef_PROTO;

static bHYPRE_StructMatrix__addRef_RETURN
bHYPRE_StructMatrix__addRef
bHYPRE_StructMatrix__addRef_PROTO;

static bHYPRE_StructMatrix_PyType_RETURN
bHYPRE_StructMatrix_PyType
bHYPRE_StructMatrix_PyType_PROTO;

#else

static void **bHYPRE_StructMatrix__API = NULL;

#define bHYPRE_StructMatrix__wrap \
  (*((bHYPRE_StructMatrix__wrap_RETURN (*) \
  bHYPRE_StructMatrix__wrap_PROTO) \
  (bHYPRE_StructMatrix__API \
  [bHYPRE_StructMatrix__wrap_NUM])))

#define bHYPRE_StructMatrix__convert \
  (*((bHYPRE_StructMatrix__convert_RETURN (*) \
  bHYPRE_StructMatrix__convert_PROTO) \
  (bHYPRE_StructMatrix__API \
  [bHYPRE_StructMatrix__convert_NUM])))

#define bHYPRE_StructMatrix__convert_python_array \
  (*((bHYPRE_StructMatrix__convert_python_array_RETURN (*) \
  bHYPRE_StructMatrix__convert_python_array_PROTO) \
  (bHYPRE_StructMatrix__API \
  [bHYPRE_StructMatrix__convert_python_array_NUM])))

#define bHYPRE_StructMatrix__convert_sidl_array \
  (*((bHYPRE_StructMatrix__convert_sidl_array_RETURN (*) \
  bHYPRE_StructMatrix__convert_sidl_array_PROTO) \
  (bHYPRE_StructMatrix__API \
  [bHYPRE_StructMatrix__convert_sidl_array_NUM])))

#define bHYPRE_StructMatrix__weakRef \
  (*((bHYPRE_StructMatrix__weakRef_RETURN (*) \
  bHYPRE_StructMatrix__weakRef_PROTO) \
  (bHYPRE_StructMatrix__API \
  [bHYPRE_StructMatrix__weakRef_NUM])))

#define bHYPRE_StructMatrix_deref \
  (*((bHYPRE_StructMatrix_deref_RETURN (*) \
  bHYPRE_StructMatrix_deref_PROTO) \
  (bHYPRE_StructMatrix__API \
  [bHYPRE_StructMatrix_deref_NUM])))

#define bHYPRE_StructMatrix__newRef \
  (*((bHYPRE_StructMatrix__newRef_RETURN (*) \
  bHYPRE_StructMatrix__newRef_PROTO) \
  (bHYPRE_StructMatrix__API \
  [bHYPRE_StructMatrix__newRef_NUM])))

#define bHYPRE_StructMatrix__addRef \
  (*((bHYPRE_StructMatrix__addRef_RETURN (*) \
  bHYPRE_StructMatrix__addRef_PROTO) \
  (bHYPRE_StructMatrix__API \
  [bHYPRE_StructMatrix__addRef_NUM])))

#define bHYPRE_StructMatrix_PyType \
  (*((bHYPRE_StructMatrix_PyType_RETURN (*) \
  bHYPRE_StructMatrix_PyType_PROTO) \
  (bHYPRE_StructMatrix__API \
  [bHYPRE_StructMatrix_PyType_NUM])))

#define bHYPRE_StructMatrix__connectI \
  (*((bHYPRE_StructMatrix__connectI_RETURN (*) \
  bHYPRE_StructMatrix__connectI_PROTO) \
  (bHYPRE_StructMatrix__API \
  [bHYPRE_StructMatrix__connectI_NUM])))

#define bHYPRE_StructMatrix__rmicast \
  (*((bHYPRE_StructMatrix__rmicast_RETURN (*) \
  bHYPRE_StructMatrix__rmicast_PROTO) \
  (bHYPRE_StructMatrix__API \
  [bHYPRE_StructMatrix__rmicast_NUM])))

#ifdef HAVE_PTHREAD
#define bHYPRE_StructMatrix__import() \
{ \
  pthread_mutex_t __sidl_pyapi_mutex = PTHREAD_MUTEX_INITIALIZER; \
  pthread_mutex_lock(&__sidl_pyapi_mutex); \
  if (!bHYPRE_StructMatrix__API) { \
    PyObject *module = PyImport_ImportModule("bHYPRE.StructMatrix"); \
    if (module != NULL) { \
      PyObject *module_dict = PyModule_GetDict(module); \
      PyObject *c_api_object = \
        PyDict_GetItemString(module_dict, "_C_API"); \
      if (c_api_object && PyCObject_Check(c_api_object)) { \
        bHYPRE_StructMatrix__API = \
          (void **)PyCObject_AsVoidPtr(c_api_object); \
      } \
      else { fprintf(stderr, "babel: bHYPRE_StructMatrix__import failed to lookup _C_API (%p %p %s).\n", c_api_object, c_api_object ? c_api_object->ob_type : NULL, c_api_object ? c_api_object->ob_type->tp_name : ""); }\
      Py_DECREF(module); \
    } else { fprintf(stderr, "babel: bHYPRE_StructMatrix__import failed to import its module.\n"); }\
  }\
  pthread_mutex_unlock(&__sidl_pyapi_mutex); \
  pthread_mutex_destroy(&__sidl_pyapi_mutex); \
}
#else /* !HAVE_PTHREAD */
#define bHYPRE_StructMatrix__import() \
if (!bHYPRE_StructMatrix__API) { \
  PyObject *module = PyImport_ImportModule("bHYPRE.StructMatrix"); \
  if (module != NULL) { \
    PyObject *module_dict = PyModule_GetDict(module); \
    PyObject *c_api_object = \
      PyDict_GetItemString(module_dict, "_C_API"); \
    if (c_api_object && PyCObject_Check(c_api_object)) { \
      bHYPRE_StructMatrix__API = \
        (void **)PyCObject_AsVoidPtr(c_api_object); \
    } \
    else { fprintf(stderr, "babel: bHYPRE_StructMatrix__import failed to lookup _C_API (%p %p %s).\n", c_api_object, c_api_object ? c_api_object->ob_type : NULL, c_api_object ? c_api_object->ob_type->tp_name : ""); }\
    Py_DECREF(module); \
  } else { fprintf(stderr, "babel: bHYPRE_StructMatrix__import failed to import its module.\n"); }\
}
#endif /* HAVE_PTHREAD */

#endif

#ifdef __cplusplus
}
#endif

#endif
