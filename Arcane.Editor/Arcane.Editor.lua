project "Arcane.Editor"
	kind "ConsoleApp"

	files {
		GSourceDirectory .. "/**.cpp",
	}

	includedirs {
		GSourceDirectory,
		GetProjectRootDirectory("Arcane.Core") .. GSourceDirectory
	}

	libdirs {
		GetProjectRootDirectory("Arcane.Core") .. GTargetDirectory
	}

	links {
		"Arcane.Core"
	}

	ApplyDefaultProjectSettings()
	ApplyConfigurationSettings()