local outputdir = "%{cfg.buildcfg}-%{cfg.system}"

project("Luma")
kind("StaticLib")
language("C++")
cppdialect("C++17")
systemversion("latest")

targetdir("../bin/" .. outputdir .. "/%{prj.name}")
objdir("../build/" .. outputdir .. "/%{prj.name}")

files({
	"source/**.h",
	"source/**.cpp",
})

includedirs({
	"source",
	"../vendor/assimp/include",
	"../vendor/glad/include",
	"../vendor/glm",
	"../vendor/imgui/include",
	"../vendor/SDL/include",
	"../vendor/stb_image/include",
})

links({
	"assimp",
	"glad",
	"imgui",
	"SDL3",
	"stb_image",
})

filter("system:linux")
libdirs({
	"../vendor/assimp/lib/linux",
	"../vendor/glad/lib/linux",
	"../vendor/imgui/lib/linux",
	"../vendor/SDL/lib/linux",
	"../vendor/stb_image/lib/linux",
})
defines({
	"PLATFORM_LINUX",
})

filter("system:windows")
defines({
	"PLATFORM_WINDOWS",
})

filter({ "configurations:Debug" })
defines("DEBUG")
symbols("on")

filter({ "configurations:Release" })
defines("RELEASE")
optimize("on")

filter({ "configurations:Dist" })
defines("DIST")
optimize("on")
