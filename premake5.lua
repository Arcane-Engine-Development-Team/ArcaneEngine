GBuildDirectory = "./Build"
GSourceDirectory = "./Source"
GTargetDirectory = "./Binaries/Output/%{cfg.buildcfg}"
GObjectDirectory = "./Binaries/Intermediate/%{cfg.buildcfg}"
GCppDialect = "C++17"
GWorkspaceRoot = "%{wks.location}/../"

function GetProjectRootDirectory(project_name)
	return GWorkspaceRoot .. project_name .. "/"
end

function ApplyConfigurationSettings()
	filter "configurations:Debug"
		defines { "AR_PLATFORM_BUILD_CONFIG_DEBUG=1" }
		symbols "On"

	filter "configurations:Release"
		defines { "AR_PLATFORM_BUILD_CONFIG_RELEASE=1" }
		optimize "Speed"

	filter {}
end

function ApplyDefaultProjectSettings()
	language "C++"
	cppdialect(GCppDialect)

	location(GBuildDirectory)
	
	targetdir(GTargetDirectory)
	objdir(GObjectDirectory)
end

workspace "ArcaneEngine"
    configurations { "Debug", "Release" }
    startproject "Arcane.Editor"
	location (GBuildDirectory)

	toolset "gcc"

	include "./Arcane.Core/Arcane.Core.lua"
	include "./Arcane.Editor/Arcane.Editor.lua"