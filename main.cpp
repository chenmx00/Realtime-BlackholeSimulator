#include "tools.h"
#include "draw_texture.h"
#include "blackhole.h"
#include <Windows.h>
#include <vector>
#include <unordered_map>


vector<Vfx*> effects;

Vfx_bh* ef_blackhole;
DrawTextureEffect* ef_draw_tex;

Input input;

mat4 cam_mat;
vec3 cam_pos = vec3(5.6, 4.7, -9);
vec3 cam_rot = vec3(-.25, 0.128, 0.17); 
vec3 cam_pos_v;
vec3 cam_rot_v;
float cam_pos_v_init = 2.f;
float cam_rot_v_init = 0.35f;
float cam_v_decay = 0.2;

void set_cam_pos_velocity(vec3 dir) {
    cam_pos_v += vec3(cam_mat * vec4(dir, 0) * cam_pos_v_init);
}

void set_cam_rot_velocity(vec3 rot) {
    cam_rot_v += rot * cam_rot_v_init;
}



void update();
void render();

float time_previous = -1;
float fps = 0;
float time_elapsed;

void compute_time() {
    static int frames, last_time;
    auto ticks = clock();
    if (frames == 0) last_time = ticks;
    frames++;
    if (ticks - last_time >= 500)
    {
        fps = 1000.0 * frames / (ticks - last_time);
        frames = 0;
    }

    if (time_previous < 0) time_previous = time();
    time_elapsed = time() - time_previous;
    time_previous = time();
}

static void display() {
    compute_time();
    update();
    render();

    glFinish();
    glutPostRedisplay();
}


static void reshape(int w, int h) {
    _set_geometry(w, h, 0, 0);
    glViewport(0, 0, w, h);
    for (Vfx* ef : effects) {
        ef->resize();
    }
}

static void key_up(unsigned char k, int x, int y)
{
    input.on_key_up(k);
}

static void key_down(unsigned char k, int x, int y)
{
    input.on_key_down(k);
}

void update() {
    cam_pos += cam_pos_v * time_elapsed;
    cam_rot += cam_rot_v * time_elapsed;

    cam_pos_v *= 1 - cam_v_decay * std::max(1.f, time_elapsed);
    cam_rot_v *= 1 - cam_v_decay * std::max(1.f, time_elapsed);

    float factor = input.get_key(' ') ? .05 : 1;

    if (input.get_key('a')) {
        set_cam_pos_velocity(vec3(-1, 0, 0) * factor);
    }
    if (input.get_key('d')) {
        set_cam_pos_velocity(vec3(1, 0, 0) * factor);
    }
    if (input.get_key('w')) {
        set_cam_pos_velocity(vec3(0, 1, 0) * factor);
    }
    if (input.get_key('s')) {
        set_cam_pos_velocity(vec3(0, -1, 0) * factor);
    }
    if (input.get_key('z')) {
        set_cam_pos_velocity(vec3(0, 0, -1) * factor);
    }
    if (input.get_key('x')) {
        set_cam_pos_velocity(vec3(0, 0, 1) * factor);
    }
    if (input.get_key('j')) {
        set_cam_rot_velocity(vec3(-1, 0, 0) * factor);
    }
    if (input.get_key('l')) {
        set_cam_rot_velocity(vec3(1, 0, 0) * factor);
    }
    if (input.get_key('i')) {
        set_cam_rot_velocity(vec3(0, -1, 0) * factor);
    }
    if (input.get_key('k')) {
        set_cam_rot_velocity(vec3(0, 1, 0) * factor);
    }
    if (input.get_key('u')) {
        set_cam_rot_velocity(vec3(0, 0, -1) * factor);
    }
    if (input.get_key('o')) {
        set_cam_rot_velocity(vec3(0, 0, 1) * factor);
    }

    cam_mat = glm::identity<mat4>();
    cam_mat = glm::translate(cam_mat, cam_pos);
    cam_mat = glm::rotate(cam_mat, cam_rot[0], vec3(0, 1, 0));
    cam_mat = glm::rotate(cam_mat, cam_rot[1], vec3(1, 0, 0));
    cam_mat = glm::rotate(cam_mat, cam_rot[2], vec3(0, 0, 1));
}

void render() {
    ef_draw_tex->draw(ef_blackhole->render_texture(cam_mat));

    int pos_y = 0;

}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_SINGLE);
    glutInitWindowSize(1400, 1000);

    int windowHandle = glutCreateWindow("Blackhole Simulator");
    _set_time_start();
 
    printf("Starting...\n");
    glewInit();
 

   
    effects.push_back(ef_blackhole = new Vfx_bh());
    effects.push_back(ef_draw_tex = new DrawTextureEffect());
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(key_down);
    glutKeyboardUpFunc(key_up);



    glutMainLoop();
}