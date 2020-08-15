#pragma once
#include "tools.h"
#include "lib/gtc/type_ptr.hpp"

// hide the shader names
namespace impl {
    class BlackholeShader : public CGLShader {
        GLint _resolution, _time, _tex_disc, _tex_previous, _tex_sun, _tex_bg, _cam_mat, _samples;
    public:
        BlackholeShader(const char* frag_shader) : CGLShader(nullptr, frag_shader) {
            _resolution = glGetUniformLocation(programObject, "resolution");
            _time = glGetUniformLocation(programObject, "time_total");
            _tex_disc = glGetUniformLocation(programObject, "tex_disc");
            _tex_previous = glGetUniformLocation(programObject, "tex_previous");
            _tex_sun = glGetUniformLocation(programObject, "tex_sun");
            _tex_bg = glGetUniformLocation(programObject, "tex_bg");
            _cam_mat = glGetUniformLocation(programObject, "cam_mat");
            _samples = glGetUniformLocation(programObject, "samples");
        };
        void update(vec3 resolution, float time, unsigned int tex_disc, unsigned int tex_previous, unsigned int tex_bg, const mat4& cam_mat) {
            glUniform3f(_resolution, resolution.x, resolution.y, resolution.z);
            glUniform1f(_time, time);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, tex_disc);
            glUniform1i(_tex_disc, 2);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, tex_previous);
            glUniform1i(_tex_previous, 1);


            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, tex_bg);
            glUniform1i(_tex_bg, 3);

            glUniformMatrix4fv(_cam_mat, 1, GL_FALSE, glm::value_ptr(cam_mat));

            glUniform1i(_samples, 1);
        }
    };
}

class Vfx_bh : public Vfx {
    unsigned int fb = 0, fb_tex = 0;
    unsigned int tex_bh = 0, tex_background = 0;
    impl::BlackholeShader* bh_shader = nullptr;
    void fb_init() {
        create_fb(&fb, &fb_tex, resolution_2i().x, resolution_2i().y);
    }
public:
    Vfx_bh() {
        fb_init();
        load_texture(&tex_background, "res/milky_way_nasa.bmp");
        bh_shader = new impl::BlackholeShader("res/blackhole_adisc.glslf");
    }

    void resize() {
        fb_init();
    }

    GLuint render_texture(const mat4& cam_mat) {
        glBindFramebuffer(GL_FRAMEBUFFER, fb);
        auto shader = use_shader(bh_shader);
        shader->update(resolution_3f(), time(), tex_bh, fb_tex, tex_background, cam_mat);
        glRectf(-1.0, -1.0, 1.0, 1.0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return fb_tex;
    }
};