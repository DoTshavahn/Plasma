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

#include "plLocalization.h"

#include "HeadSpin.h"
#include "plFileSystem.h"

#include <string_theory/string>
#include <string_theory/string_stream>

plLocalization::Language plLocalization::fLanguage = plLocalization::kEnglish;

const ST::string plLocalization::fLangTags[] =
{
    ST_LITERAL("_eng"), // kEnglish
    ST_LITERAL("_fre"), // kFrench
    ST_LITERAL("_ger"), // kGerman
    ST_LITERAL("_spa"), // kSpanish
    ST_LITERAL("_ita"), // kItalian
    ST_LITERAL("_jpn")  // kJapanese
};
const int kLangTagLen = 4;

// ISO 639, e.g. used in video tracks
std::set<ST::string> plLocalization::fLangCodes[] =
{
    {"eng", "en"},
    {"fre", "fra", "fr"},
    {"ger", "deu", "de"},
    {"spa", "es"},
    {"ita", "it"},
    {"jpn", "ja"}
};

const ST::string plLocalization::fLangNames[] =
{
    ST_LITERAL("English"), // kEnglish
    ST_LITERAL("French"),  // kFrench
    ST_LITERAL("German"),  // kGerman
    ST_LITERAL("Spanish"), // kSpanish
    ST_LITERAL("Italian"), // kItalian
    ST_LITERAL("Japanese") // kJapanese
};

plFileName plLocalization::IGetLocalized(const plFileName& name, Language lang)
{
    ST_ssize_t underscore = name.AsString().find_last('_');

    if (underscore >= 0)
    {
        ST::string langTag = name.AsString().substr(underscore, kLangTagLen);

        if (langTag == fLangTags[kEnglish])
            return name.AsString().replace(fLangTags[kEnglish], fLangTags[lang]);
    }

    return plFileName();
}

ST::string plLocalization::GetLanguageName(plLocalization::Language lang)
{
    return fLangNames[lang];
}

std::set<ST::string> plLocalization::GetLanguageCodes(plLocalization::Language lang)
{
    return fLangCodes[lang];
}

plFileName plLocalization::GetLocalized(plFileName const &name)
{
    return IGetLocalized(name, fLanguage);
}

plFileName plLocalization::ExportGetLocalized(const plFileName& name, int lang)
{
    plFileName localizedName = IGetLocalized(name, Language(lang+1));
    if (plFileInfo(localizedName).Exists())
        return localizedName;

    return "";
}

ST::string plLocalization::LocalToString(const std::vector<ST::string>& localizedText)
{
    ST::string_stream ss;
    for (size_t i = 0; i < localizedText.size(); i++)
    {
        if (i > kNumLanguages - 1)
            break;
        ST::string langName = GetLanguageName((Language)i);
        ss << '$' << langName.substr(0, 2) << '$' << localizedText[i];
    }
    return ss.to_string();
}

std::vector<ST::string> plLocalization::StringToLocal(const ST::string& localizedText)
{
    std::vector<ST::string> tags;
    std::vector<int> tagLocs;
    std::vector<int> sortedTagLocs;
    std::vector<ST::string> retVal;
    int i;
    for (i=0; i<kNumLanguages; i++)
    {
        ST::string langName = GetLanguageName((Language)i);
        ST::string tag = "$" + langName.substr(0, 2) + "$";
        tags.push_back(tag);
        tagLocs.push_back(localizedText.find(tag));
        sortedTagLocs.push_back(i);
        retVal.emplace_back();
    }
    for (i=0; i<kNumLanguages-1; i++)
    {
        for (int j=i; j<kNumLanguages; j++)
        {
            if (tagLocs[sortedTagLocs[i]] > tagLocs[sortedTagLocs[j]])
                sortedTagLocs[i]^=sortedTagLocs[j]^=sortedTagLocs[i]^=sortedTagLocs[j]; // swap the contents (yes, it works)
        }
    }
    // now sortedTagLocs has the indexes of tagLocs sorted from smallest loc to highest loc
    bool noTags = true;
    for (i=0; i<kNumLanguages; i++)
    {
        int lang = sortedTagLocs[i]; // the language we are extracting
        if (tagLocs[lang] != -1)
        {
            noTags = false; // at least one tag was found in the text
            int startLoc = tagLocs[lang] + tags[lang].size();
            int endLoc;
            if (i+1 == kNumLanguages)
                endLoc = localizedText.size();
            else
                endLoc = tagLocs[sortedTagLocs[i+1]];
            retVal[lang] = localizedText.substr(startLoc,endLoc-startLoc);
        }
    }
    if (noTags)
        retVal[0] = localizedText; // if no tags were in the text, we assume it to be English
    return retVal;
}
