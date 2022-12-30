#include <math.h>
#include <glm/glm.hpp>

namespace Noise
{

    float interpolate(float a0, float a1, float w)
    {
        return (a1 - a0) * w + a0;
    };

    glm::vec2 random_gradient(int ix, int iy)
    {
        const unsigned w = 8 * sizeof(unsigned);
        const unsigned s = w / 2;
        unsigned a = ix, b = iy;

        a *= 3284157443; b ^= a << s | a >> w-s;
        b *= 1911520717; a ^= b << s | b >> w-s;
        a *= 2048419325;

        float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]
        glm::vec2 v;
        v.x = cos(random); v.y = sin(random);
        return v;

    };

    static float dot_grid_gradient(int ix, int iy, float x, float y)
    {
        glm::vec2 grad = random_gradient(ix, iy);

        float dx = x - (float)ix;
        float dy = y - (float)iy;

        return (dx*grad.x + dy*grad.y);
    };

    static float perlin(glm::vec3 coords)
    {
        int x0 = (int)floor(coords.x);
        int x1 = x0 + 1;
        int y0 = (int)floor(coords.z);
        int y1 = y0 + 1;

        //TODO:: Calculate interpolation weights?
        float sx = coords.x - (float)x0;
        float sy = coords.z - (float)y0;

        //Interpolate between grid point gradients
        float n0, n1, ix0, ix1, value;

        n0 = dot_grid_gradient(x0, y0, coords.x, coords.z);
        n1 = dot_grid_gradient(x1, y0, coords.x, coords.z);
        ix0 = interpolate(n0, n1, sx);

        n0 = dot_grid_gradient(x0, y1, coords.x, coords.z);
        n1 = dot_grid_gradient(x1, y1, coords.x, coords.z);
        ix1 = interpolate(n0, n1, sx);

        value = interpolate(ix0, ix1, sy);
        return value;
    };

    static void generate_noise()
    {

    };
};
