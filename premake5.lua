include "Dependencies.lua"

workspace "SNTDataBase"
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
	include "imgui"
group ""

include "imgui"
include "SNTDataBase"