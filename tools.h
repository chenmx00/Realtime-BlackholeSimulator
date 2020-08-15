#pragma once
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <Windows.h>
// #define GLUT_DISABLE_ATEXIT_HACK 

#include <vector>
#include <string>
#include <stdarg.h>
#include <memory>
#include <unordered_map>

#include "lib/glew.h"
#include "lib/glut.h"
#include "lib/glm.hpp"
#include "lib/vec3.hpp"
#include "cgl_shader.h"

using namespace std;
using namespace glm;

inline float length2(vec3 v) { return dot(v, v); }

i32vec2 resolution_2i();

vec3 resolution_3f();

float time();

void load_texture(GLuint* tex_id, char* filename);

void load_cubemap(GLuint* tex_id, const vector<char*>& files);

void create_fb(GLuint* framebuffer, GLuint* framebuffer_tex, int w, int h);


void _set_geometry(int x, int y, int z, int w);


void _set_time_start();


class Vfx {
public:
    virtual void resize() {};
};




inline string string_format(const string fmt_str, ...) {
    int final_n, n = ((int)fmt_str.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
    unique_ptr<char[]> formatted;
    va_list ap;
    while (1) {
        formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
        strcpy(&formatted[0], fmt_str.c_str());
        va_start(ap, fmt_str);
        final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
        va_end(ap);
        if (final_n < 0 || final_n >= n)
            n += abs(final_n - n + 1);
        else
            break;
    }
    return string(formatted.get());
}
//
//template<typename T>
//inline void bound(T& value, T from, T to) {
//    if (value > from) value = from;
//    if ()
//}

class Input {
    unordered_map<char, bool> keys_pressed;
public:
    Input() = default;
    void on_key_down(char ch) {
        keys_pressed[ch] = true;
    }
    void on_key_up(char ch) {
        keys_pressed[ch] = false;
    }
    bool get_key(char ch) {
        auto& k = keys_pressed.find(ch);
        if (k == keys_pressed.end()) {
            return false;
        }
        return k->second;
    }
};