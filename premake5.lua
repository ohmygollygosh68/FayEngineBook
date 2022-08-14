workspace "FayEngineBook"
	architecture "x64"
	configurations { "Debug", "Release" }
	-- location "build"
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "FayEngineBook"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	systemversion "latest"
	-- targetdir "bin/%{cfg.buildcfg}"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.hpp",
		"**.cpp",
		"**.h",
		"/imgui/**.h",
		"/imgui/**.cpp"
	}



	filter "configurations:*"
		defines { "SFML_DYNAMIC" }
		includedirs { "libraries/include" }
		includedirs {"imgui"}
		libdirs { "libraries/lib" }
		links
		{
			"opengl32",
			"freetype",
			"winmm",
			"gdi32",
			"flac",
			"vorbisenc",
			"vorbisfile",
			"vorbis",
			"ogg",
			"ws2_32"
		}

	filter "configurations:Debug"
		defines { "DEBUG" }
		runtime "Debug"
		symbols "On"
		links
		{	
			"sfml-graphics-d",
			"sfml-window-d",
			"sfml-system-d",
			"sfml-audio-d",
			"sfml-network-d"
		}

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		links
		{	
			"sfml-graphics",
			"sfml-window",
			"sfml-system",
			"sfml-audio",
			"sfml-network"
		}