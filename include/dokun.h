/*
    Copyright (c) 2015-2018 Sid Lartey Doku

    This file is part of the Dokun Library.

    Licensed under the MIT Open Source License, 
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/
#ifndef _DOKUN
#define _DOKUN
#define DOKUN_VERSION_MAJOR 1
#define DOKUN_VERSION_MINOR 0
#define DOKUN_VERSION_PATCH 0
#define DOKUN_VERSION DOKUN_VERSION_MAJOR "." DOKUN_VERSION_MINOR "." DOKUN_VERSION_PATCH
// header-only files.
#include "platform.h"
#include "types.h" 
#include "math.h"
#include "vector.h"
#include "matrix_test.h"
//#include "quaternion.h" // not in using right now
#include "string.h"
#include "logger.h"
#include "utility.h"
#include "system.h"
// engine.
#include "engine.h"
// container.
#include "window.h"
// graphics.
#include "sprite.h"
#include "model.h"
#include "texture.h"
#include "image.h"
#include "vertex.h"
// input.
#include "mouse.h"
#include "keyboard.h"
#include "joystick.h"
#include "touch.h"
#include "virtual.h" // virtual reality
#include "brain.h" // eeg
// audio.
#include "sound.h"
#include "music.h"
#include "voice.h"
// font
#include "font.h"
// ui
#include "ui.h"
#include "widget.h" 
#include "button.h" 
#include "label.h"
#include "edit.h"
#include "progressbar.h"
#include "slider.h"
#include "toggle.h"
#include "scrollbar.h"
#include "list.h"
#include "spinner.h"
#include "menubar.h"
#include "grid.h"
// network.
#include "server.h"
#include "client.h"
// utilities - the most essential part of the engine
// entity and component
#include "entity.h"
#include "component.h"
// camera
#include "camera.h"
// level (scene) - also controls physics
#include "level.h"
// timer
#include "timer.h"
// lighting
#include "light.h"
// shader (glsl)
#include "shader.h"
// resources (assets)
#include "resource.h"
// animation - for keyframe animation and skeletal animation
#include "animator.h" 
// data serialization (sqlite or libxml)
#include "data.h"
// object manager - keeps track of objects
#include "factory.h" 
// script manager
#include "script.h"
// console
#include "console.h"
// video (cinema)
#include "video.h"
// plugin (extension)
#include "plugin.h"
// file system
#include "file.h"
// event system
#include "event.h"
// file format loader
#include "loader.h"
// renderer
#include "renderer.h"
// process
#include "process.h"
// browser
#include "http.h"
#endif 
// about 60-70 headers; 50-80 sources                     missing (or not included here): dokun.h, build.h,  
