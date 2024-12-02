$input v_color0, v_texcoord0

#include "common.sh"

SAMPLER2D(s_tex_color, 0);

void main()
{
    vec4 texColor = texture2D(s_tex_color, v_texcoord0);
    gl_FragColor = texColor * v_color0;
}
