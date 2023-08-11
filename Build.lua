-- premake5.lua
workspace "BO3-PT"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "PracticeTool"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "Walnut/Build-Walnut-External.lua"

group "PracticeTool"
    include "BO3 Practice Tool/Build-Practice-Tool.lua"
group ""