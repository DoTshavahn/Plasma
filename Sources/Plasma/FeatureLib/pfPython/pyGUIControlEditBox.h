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
#ifndef _pyGUIControlEditBox_h_
#define _pyGUIControlEditBox_h_

//////////////////////////////////////////////////////////////////////
//
// pyGUIControlEditBox   - a wrapper class to provide interface to modifier
//                   attached to a GUIControlEditBox
//
//////////////////////////////////////////////////////////////////////

#include "pyGlueHelpers.h"
#include "pyGUIControl.h"

#include <string_theory/string>

class pyColor;

class pyGUIControlEditBox : public pyGUIControl
{
protected:
    pyGUIControlEditBox(): pyGUIControl() {} // for python glue only, do NOT call
    pyGUIControlEditBox(pyKey& gckey);
    pyGUIControlEditBox(plKey objkey);

public:
    // required functions for PyObject interoperability
    PYTHON_CLASS_NEW_FRIEND(ptGUIControlEditBox);
    static PyObject *New(pyKey& gckey);
    static PyObject *New(plKey objkey);
    PYTHON_CLASS_CHECK_DEFINITION; // returns true if the PyObject is a pyGUIControlEditBox object
    PYTHON_CLASS_CONVERT_FROM_DEFINITION(pyGUIControlEditBox); // converts a PyObject to a pyGUIControlEditBox (throws error if not correct type)

    static void AddPlasmaClasses(PyObject *m);

    static bool IsGUIControlEditBox(pyKey& gckey);

    virtual void    SetBufferSize( uint32_t size );
    virtual ST::string GetBuffer();
    virtual void    ClearBuffer();
    virtual void    SetText( const ST::string& str );
    virtual void    SetCursorToHome();
    virtual void    SetCursorToEnd();
    virtual void    SetColor(pyColor& forecolor, pyColor& backcolor);
    virtual void    SetSelColor(pyColor& forecolor, pyColor& backcolor);

    virtual bool    WasEscaped();

    virtual void    SetSpecialCaptureKeyMode(bool state);
    virtual uint32_t  GetLastKeyCaptured();
    virtual uint32_t  GetLastModifiersCaptured();
    virtual void    SetLastKeyCapture(uint32_t key, uint32_t modifiers);

    virtual void    SetChatMode(bool state);

};

#endif // _pyGUIControlEditBox_h_
