project "Arcane.Core"
	kind "StaticLib"

	files {
		GSourceDirectory .. "/**.cpp",
	}

	includedirs {
		GSourceDirectory
	}

	ApplyDefaultProjectSettings()
	ApplyConfigurationSettings()