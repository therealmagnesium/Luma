workspace("Luma3D")
architecture("x64")

configurations({
	"Debug",
	"Release",
	"Dist",
})

include("Luma")
include("Sandbox")
