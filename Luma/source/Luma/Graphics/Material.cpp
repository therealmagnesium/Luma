#include "Luma/Graphics/Material.h"
#include "Luma/Graphics/Renderer.h"
#include <glm/glm.hpp>

namespace Luma
{
    namespace Graphics
    {
        Material LoadMaterialDefault()
        {
            Material material;
            material.shader = &GetDefaultShader();
            return material;
        }
    }
}
