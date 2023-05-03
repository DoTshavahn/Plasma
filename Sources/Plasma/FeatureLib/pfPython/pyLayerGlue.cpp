/*==LICENSE==*

CyanWorlds.com Engine - MMOG client, server and tools
Copyright (C) 2011  Cyan Worlds, Inc.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Additional permissions under GNU GPL version 3 section 7

If you modify this Program, or any covered work, by linking or
combining it with any of RAD Game Tools Bink SDK, Autodesk 3ds Max SDK,
NVIDIA PhysX SDK, Microsoft DirectX SDK, OpenSSL library, Independent
JPEG Group JPEG library, Microsoft Windows Media SDK, or Apple QuickTime SDK
(or a modified version of those libraries),
containing parts covered by the terms of the Bink SDK EULA, 3ds Max EULA,
PhysX SDK EULA, DirectX SDK EULA, OpenSSL and SSLeay licenses, IJG
JPEG Library README, Windows Media SDK EULA, or QuickTime SDK EULA, the
licensors of this Program grant you additional
permission to convey the resulting work. Corresponding Source for a
non-source form of such a combination shall include the source code for
the parts of OpenSSL and IJG JPEG Library used as well as that of the covered
work.

You can contact Cyan Worlds, Inc. by email legal@cyan.com
 or by snail mail at:
      Cyan Worlds, Inc.
      14617 N Newport Hwy
      Mead, WA   99021

*==LICENSE==*/

#include <Python.h>
#include "pyKey.h"

#include "pyLayer.h"
#include "pyImage.h"

// glue functions
PYTHON_CLASS_DEFINITION(ptLayer, pyLayer);

PYTHON_DEFAULT_NEW_DEFINITION(ptLayer, pyLayer)
PYTHON_DEFAULT_DEALLOC_DEFINITION(ptLayer)

PYTHON_INIT_DEFINITION(ptLayer, args, keywords)
{
    PyObject* keyObj = nullptr;
    if (!PyArg_ParseTuple(args, "O", &keyObj))
    {
        PyErr_SetString(PyExc_TypeError, "__init__ expects a ptKey");
        PYTHON_RETURN_INIT_ERROR;
    }
    if (!pyKey::Check(keyObj))
    {
        PyErr_SetString(PyExc_TypeError, "__init__ expects a ptKey");
        PYTHON_RETURN_INIT_ERROR;
    }
    pyKey* key = pyKey::ConvertFrom(keyObj);
    self->fThis->setKey(*key);
    PYTHON_RETURN_INIT_OK;
}

PYTHON_RICH_COMPARE_DEFINITION(ptLayer, obj1, obj2, compareType)
{
    if ((obj1 == Py_None) || (obj2 == Py_None) || !pyLayer::Check(obj1) || !pyLayer::Check(obj2))
    {
        // if they aren't the same type, they don't match, obviously (we also never equal none)
        if (compareType == Py_EQ)
            PYTHON_RCOMPARE_FALSE;
        else if (compareType == Py_NE)
            PYTHON_RCOMPARE_TRUE;
        else
        {
            PyErr_SetString(PyExc_NotImplementedError, "invalid comparison for a ptLayer object");
            PYTHON_RCOMPARE_ERROR;
        }
    }
    pyLayer* layer1 = pyLayer::ConvertFrom(obj1);
    pyLayer* layer2 = pyLayer::ConvertFrom(obj2);
    if (compareType == Py_EQ)
    {
        if ((*layer1) == (*layer2))
            PYTHON_RCOMPARE_TRUE;
        PYTHON_RCOMPARE_FALSE;
    }
    else if (compareType == Py_NE)
    {
        if ((*layer1) != (*layer2))
            PYTHON_RCOMPARE_TRUE;
        PYTHON_RCOMPARE_FALSE;
    }
    PyErr_SetString(PyExc_NotImplementedError, "invalid comparison for a ptLayer object");
    PYTHON_RCOMPARE_ERROR;
}

PYTHON_GET_DEFINITION(ptLayer, texture)
{
    return self->fThis->GetTexture();
}

PYTHON_SET_DEFINITION(ptLayer, texture, value)
{
    if (pyImage::Check(value)) {
        self->fThis->SetTexture(pyImage::ConvertFrom(value)->GetImage());
        PYTHON_RETURN_SET_OK;
    }
    PyErr_SetString(PyExc_TypeError, "setTexture expects a ptImage");
    PYTHON_RETURN_SET_ERROR;
}

PYTHON_START_GETSET_TABLE(ptLayer)
PYTHON_GETSET(ptLayer, texture, "The image texture of the layer"),
PYTHON_END_GETSET_TABLE;

PYTHON_START_METHODS_TABLE(ptLayer)
// no methods
PYTHON_END_METHODS_TABLE;

// Type structure definition
#define ptLayer_AS_NUMBER       PYTHON_NO_AS_NUMBER
#define ptLayer_AS_SEQUENCE     PYTHON_NO_AS_SEQUENCE
#define ptLayer_AS_MAPPING      PYTHON_NO_AS_MAPPING
#define ptLayer_STR             PYTHON_NO_STR
#define ptLayer_GETATTRO        PYTHON_NO_GETATTRO
#define ptLayer_SETATTRO        PYTHON_NO_SETATTRO
#define ptLayer_RICH_COMPARE    PYTHON_DEFAULT_RICH_COMPARE(ptLayer)
#define ptLayer_GETSET          PYTHON_DEFAULT_GETSET(ptLayer)
#define ptLayer_BASE            PYTHON_NO_BASE
PLASMA_CUSTOM_TYPE(ptLayer, "Params: layerKey\nPlasma layer class");

// required functions for PyObject interoperability
PyObject* pyLayer::New(plLayer* layer)
{
    ptLayer* newObj = (ptLayer*)ptLayer_type.tp_new(&ptLayer_type, NULL, NULL);
    newObj->fThis->fLayer = layer;
    newObj->fThis->fLayerKey = layer->GetKey();
    if (layer->GetKey())
        newObj->fThis->fLayerKey->RefObject();
    return (PyObject*)newObj;
}

PyObject* pyLayer::New(plKey layerKey)
{
    ptLayer* newObj = (ptLayer*)ptLayer_type.tp_new(&ptLayer_type, NULL, NULL);
    newObj->fThis->fLayerKey = layerKey;
    return (PyObject*)newObj;
}

PyObject* pyLayer::New(pyKey& layerKey)
{
    ptLayer* newObj = (ptLayer*)ptLayer_type.tp_new(&ptLayer_type, NULL, NULL);
    newObj->fThis->fLayerKey = layerKey.getKey();
    return (PyObject*)newObj;
}

PYTHON_CLASS_CHECK_IMPL(ptLayer, pyLayer)
PYTHON_CLASS_CONVERT_FROM_IMPL(ptLayer, pyLayer)

///////////////////////////////////////////////////////////////////////////
//
// AddPlasmaClasses - the python module definitions
//
void pyLayer::AddPlasmaClasses(PyObject* m)
{
    PYTHON_CLASS_IMPORT_START(m);
    PYTHON_CLASS_IMPORT(m, ptLayer);
    PYTHON_CLASS_IMPORT_END(m);
}
