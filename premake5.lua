include "Dependencies.lua"

workspace "SNTProject"
	architecture "x86_64"
	startproject "SNTDataBase"

	configurations
	{
		"Debug",
		"Release"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/glfw"
	include "vendor/imgui"
	include "vendor/OpenXLSX"
group ""

include "SNTDataBase"