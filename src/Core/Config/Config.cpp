// Copyright(c) 2019 - 2020, #Momo
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met :
// 
// 1. Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and /or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "Config.h"

namespace MxEngine
{
    template<typename T>
    void FromJson(T& dist, const JsonFile& json, const char* name)
    {
        dist = json.value(name, dist);
    }

    const char* EnumToString(RenderProfile profile)
    {
        switch (profile)
        {
        case RenderProfile::ANY:
            return "ANY";
        case RenderProfile::COMPAT:
            return "COMPAT";
        case RenderProfile::CORE:
            return "CORE";
        default:
            return "ANY";
        }
    }

    const char* EnumToString(BuildType mode)
    {
        switch (mode)
        {
        case BuildType::UNKNOWN:
            return "UNKNOWN";
        case BuildType::DEBUG:
            return "DEBUG";
        case BuildType::RELEASE:
            return "RELEASE";
        case BuildType::SHIPPING:
            return "SHIPPING";
        default:
            return "UNKNOWN";
        }
    }

    const char* EnumToString(EditorStyle style)
    {
        switch (style)
        {
        case MxEngine::EditorStyle::CLASSIC:
            return "CLASSIC";
        case MxEngine::EditorStyle::LIGHT:
            return "LIGHT";
        case MxEngine::EditorStyle::DARK:
            return "DARK";
        case MxEngine::EditorStyle::MXENGINE:
            return "MXENGINE";
        default:
            return "MXENGINE";
        }
    }

    void Deserialize(Config& config, const JsonFile& json)
    {
        FromJson(config.WindowPosition,         json["window"],      "position"                );
        FromJson(config.WindowSize,             json["window"],      "size"                    );
        FromJson(config.WindowTitle,            json["window"],      "title"                   );
        FromJson(config.Cursor,                 json["window"],      "cursor-mode"             );
        FromJson(config.DoubleBuffering,        json["window"],      "double-buffering"        );
        FromJson(config.GraphicAPIProfile,      json["renderer"],    "profile"                 );
        FromJson(config.GraphicAPIMajorVersion, json["renderer"],    "major-version"           );
        FromJson(config.GraphicAPIMinorVersion, json["renderer"],    "minor-version"           );
        FromJson(config.AnisothropicFiltering,  json["renderer"],    "anisothropic-filtering"  );
        FromJson(config.DirLightTextureSize,    json["renderer"],    "dir-light-texture-size"  );
        FromJson(config.PointLightTextureSize,  json["renderer"],    "point-light-texture-size");
        FromJson(config.SpotLightTextureSize,   json["renderer"],    "spot-light-texture-size" );
        FromJson(config.EngineTextureSize,      json["renderer"],    "engine-texture-size"     );
        FromJson(config.IgnoredFolders,         json["filesystem" ], "ignored-folders"         );
        FromJson(config.CachePrimitiveModels,   json["filesystem" ], "cache-primitives"        );
        FromJson(config.ShaderSourceDirectory,  json["debug-build"], "shader-source-directory" );
        FromJson(config.ApplicationCloseKey,    json["debug-build"], "app-close-key"           );
        FromJson(config.Style,                  json["debug-build"], "editor-style"            );
        FromJson(config.EditorOpenKey,          json["debug-build"], "editor-key"              );
        FromJson(config.GraphicAPIDebug,        json["debug-build"], "debug-graphics"          );
        FromJson(config.RecompileFilesKey,      json["debug-build"], "recompile-files-key"     );
        FromJson(config.AutoRecompileFiles,     json["debug-build"], "auto-recompile-files"    );
    }

    void Serialize(JsonFile& json, const Config& config)
    {
        json["window"     ]["position"                ] = config.WindowPosition;
        json["window"     ]["size"                    ] = config.WindowSize;
        json["window"     ]["title"                   ] = config.WindowTitle;
        json["window"     ]["cursor-mode"             ] = config.Cursor;
        json["window"     ]["double-buffering"        ] = config.DoubleBuffering;
        json["renderer"   ]["profile"                 ] = config.GraphicAPIProfile;
        json["renderer"   ]["major-version"           ] = config.GraphicAPIMajorVersion;
        json["renderer"   ]["minor-version"           ] = config.GraphicAPIMinorVersion;
        json["renderer"   ]["anisothropic-filtering"  ] = config.AnisothropicFiltering;
        json["renderer"   ]["dir-light-texture-size"  ] = config.DirLightTextureSize;
        json["renderer"   ]["point-light-texture-size"] = config.PointLightTextureSize;
        json["renderer"   ]["spot-light-texture-size" ] = config.SpotLightTextureSize;
        json["renderer"   ]["engine-texture-size"     ] = config.EngineTextureSize;
        json["filesystem" ]["ignored-folders"         ] = config.IgnoredFolders;
        json["filesystem" ]["cache-primitives"        ] = config.CachePrimitiveModels;
        json["debug-build"]["shader-source-directory" ] = config.ShaderSourceDirectory;
        json["debug-build"]["app-close-key"           ] = config.ApplicationCloseKey;
        json["debug-build"]["editor-style"            ] = config.Style;
        json["debug-build"]["editor-key"              ] = config.EditorOpenKey;
        json["debug-build"]["debug-graphics"          ] = config.GraphicAPIDebug;
        json["debug-build"]["recompile-files-key"     ] = config.RecompileFilesKey;
        json["debug-build"]["auto-recompile-files"    ] = config.AutoRecompileFiles;
    }

    void to_json(JsonFile& j, MxEngine::CursorMode mode)
    {
        j = EnumToString(mode);
    }
    
    void to_json(JsonFile& j, MxEngine::RenderProfile profile)
    {
        j = EnumToString(profile);
    }

    void from_json(const JsonFile& j, MxEngine::CursorMode& mode)
    {
        auto val = j.get<MxString>();
        if (val == "DISABLED")
            mode = CursorMode::DISABLED;
        else if (val == "NORMAL")
            mode = CursorMode::NORMAL;
        else
            mode = CursorMode::HIDDEN;
    }

    void from_json(const JsonFile& j, MxEngine::RenderProfile& profile)
    {
        auto val = j.get<MxString>();
        if (val == "COMPAT")
            profile = RenderProfile::COMPAT;
        else if (val == "CORE")
            profile = RenderProfile::CORE;
        else
            profile = RenderProfile::ANY;
    }

    void to_json(JsonFile& j, KeyCode key)
    {
        j = EnumToString(key);
    }

    void from_json(const JsonFile& j, KeyCode& key)
    {
        auto str = j.get<MxString>();
        #define JSONKEY(val) if(str == #val) { key = MxEngine::KeyCode::val; return; }
    
        JSONKEY(UNKNOWN);
        JSONKEY(SPACE);
        JSONKEY(APOSTROPHE);
        JSONKEY(COMMA);
        JSONKEY(MINUS);
        JSONKEY(PERIOD);
        JSONKEY(SLASH);
        JSONKEY(D0);
        JSONKEY(D1);
        JSONKEY(D2);
        JSONKEY(D3);
        JSONKEY(D4);
        JSONKEY(D5);
        JSONKEY(D6);
        JSONKEY(D7);
        JSONKEY(D8);
        JSONKEY(D9);
        JSONKEY(SEMICOLON);
        JSONKEY(EQUAL);
        JSONKEY(A);
        JSONKEY(B);
        JSONKEY(C);
        JSONKEY(D);
        JSONKEY(E);
        JSONKEY(F);
        JSONKEY(G);
        JSONKEY(H);
        JSONKEY(I);
        JSONKEY(J);
        JSONKEY(K);
        JSONKEY(L);
        JSONKEY(M);
        JSONKEY(N);
        JSONKEY(O);
        JSONKEY(P);
        JSONKEY(Q);
        JSONKEY(R);
        JSONKEY(S);
        JSONKEY(T);
        JSONKEY(U);
        JSONKEY(V);
        JSONKEY(W);
        JSONKEY(X);
        JSONKEY(Y);
        JSONKEY(Z);
        JSONKEY(LEFT_BRACKET);
        JSONKEY(BACKSLASH);
        JSONKEY(RIGHT_BRACKET);
        JSONKEY(GRAVE_ACCENT);
        JSONKEY(WORLD_1);
        JSONKEY(ESCAPE);
        JSONKEY(ENTER);
        JSONKEY(TAB);
        JSONKEY(BACKSPACE);
        JSONKEY(INSERT);
        JSONKEY(DELETE);
        JSONKEY(RIGHT);
        JSONKEY(LEFT);
        JSONKEY(DOWN);
        JSONKEY(UP);
        JSONKEY(PAGE_UP);
        JSONKEY(PAGE_DOWN);
        JSONKEY(HOME);
        JSONKEY(END);
        JSONKEY(CAPS_LOCK);
        JSONKEY(SCROLL_LOCK);
        JSONKEY(NUM_LOCK);
        JSONKEY(PRINT_SCREEN);
        JSONKEY(PAUSE);
        JSONKEY(F1);
        JSONKEY(F2);
        JSONKEY(F3);
        JSONKEY(F4);
        JSONKEY(F5);
        JSONKEY(F6);
        JSONKEY(F7);
        JSONKEY(F8);
        JSONKEY(F9);
        JSONKEY(F10);
        JSONKEY(F11);
        JSONKEY(F12);
        JSONKEY(F13);
        JSONKEY(F14);
        JSONKEY(F15);
        JSONKEY(F16);
        JSONKEY(F17);
        JSONKEY(F18);
        JSONKEY(F19);
        JSONKEY(F20);
        JSONKEY(F21);
        JSONKEY(F22);
        JSONKEY(F23);
        JSONKEY(F24);
        JSONKEY(F25);
        JSONKEY(KP_0);
        JSONKEY(KP_1);
        JSONKEY(KP_2);
        JSONKEY(KP_3);
        JSONKEY(KP_4);
        JSONKEY(KP_5);
        JSONKEY(KP_6);
        JSONKEY(KP_7);
        JSONKEY(KP_8);
        JSONKEY(KP_9);
        JSONKEY(KP_DECIMAL);
        JSONKEY(KP_DIVIDE);
        JSONKEY(KP_MULTIPLY);
        JSONKEY(KP_SUBTRACT);
        JSONKEY(KP_ADD);
        JSONKEY(KP_ENTER);
        JSONKEY(KP_EQUAL);
        JSONKEY(LEFT_SHIFT);
        JSONKEY(LEFT_CONTROL);
        JSONKEY(LEFT_ALT);
        JSONKEY(LEFT_SUPER);
        JSONKEY(RIGHT_SHIFT);
        JSONKEY(RIGHT_CONTROL);
        JSONKEY(RIGHT_ALT);
        JSONKEY(RIGHT_SUPER);
        JSONKEY(MENU);

        key = KeyCode::UNKNOWN;
    }

    void to_json(JsonFile& j, EditorStyle style)
    {
        j = EnumToString(style);
    }

    void from_json(const JsonFile& j, EditorStyle& style)
    {
        auto val = j.get<MxString>();
        if (val == "CLASSIC")
            style = EditorStyle::CLASSIC;
        else if (val == "DARK")
            style = EditorStyle::DARK;
        else if (val == "LIGHT")
            style = EditorStyle::LIGHT;
        else
            style = EditorStyle::MXENGINE;
    }
}