![dokun logo](res/icon/Dokun_text_logo.png "Dokun-logo") <!-- res/icon/Dokun-logo_128x128.png -->


[![Build Status](https://travis-ci.com/sidtheprince/dokun.svg?branch=master)](https://travis-ci.com/sidtheprince/dokun) [![alt text](res/icon/open_source_button.png)](https://opensource.org/licenses/MIT "Open Source")

`Dokun` is a video game engine framework written in [C++](https://en.wikipedia.org/wiki/C%2B%2B) with support
for the [Lua](https://www.lua.org/) language.

`version:`         `1.0.0`

```
author           Sid Lartey Doku
development date 2015-present
license          MIT
```

## Build requirements
> `Windows` [`mingw 4.8.4+`](http://www.mingw.org/)

> `All    ` [`cmake 2.8+  `](https://cmake.org/download/)</a>

 
# Building dokun
Read <a href="https://github.com/sidtheprince/dokun/blob/master/src/build.txt">src/build.txt</a> for more information on how to build dokun.
```sh
...
```

# Definitions
```sh
 DOKUN_DEBUG DOKUN_OPENGL DOKUN_VULKAN 
``` 

# Hello World (example)
```lua
-- Lua
require ("dokun")

WINDOW_MODE  = 0
FULL_SCREEN  = 1

window = Window:new()
window:create("Hello World", 800, 600, WINDOW_MODE)
window:show()

while window:is_open() do 
    window:update()
end
```
```cpp
// C++
#include <dokun.hpp>

int main()
{
    Window window;
    window.create("Hello World", 800, 600, 0);
    window.show();  
    
	while(window.is_open())
	{
		window.update();      
	}
	return 0;
}
```

# Supported External Libraries
|      Library name      |             License                  |         Used for                                      |
|------------------------|--------------------------------------|-------------------------------------------------------|
| `lua`                  | MIT                                  | scripting                                             |
| `libpng`               | zlib/libpng                          | image                                                 |
| zlib                   | zlib/libpng                          | extension                                             |
| libjpeg                | BSD                                  | image                                                 |
| giflib                 | MIT                                  | image                                                 |
| libtiff                | BSD                                  | image                                                 |
| `libogg`               | BSD                                  | container                                             |
| `libvorbis`            | BSD                                  | audio (codec)                                         |
| `libtheora`            | BSD                                  | video (codec)                                         |
| opus                   | BSD                                  | audio (codec)                                         |
| openal (soft)          | LGPL                                 | audio (playback)                                      |
| `freetype`             | FT license (BSD)                     | font                                                  |
| utf8                   | BSD                                  | unicode                                               |
| sqlite3                | public domain                        | data_serialization                                    |
| libxml2                | MIT                                  | data_serialization, model (COLLADA), image (SVG)      |
| box2d                  | zlib/libpng                          | physics                                               |
| bullet3                | zlib/libpng                          | physics                                               |
| `opengl`               | MIT                                  | graphics                                              |
| glew                   | BSD, MIT                             | extension                                             |
| vulkan                 | MIT                                  | graphics                                              |


## Powered by ##
<a href="https://www.lua.org/" target="blank"><img src="res/icon/lua-128x128.png"></a>
<a href="https://www.opengl.org/" target="_blank"><img src="res/icon/OpenGL_100px_June16.png"></a>
<a href="https://www.khronos.org/vulkan/" target="_blank"><img src="res/icon/Vulkan_100px_Mar15.png"></a>

`operating systems`

<a href="https://www.microsoft.com/en-us/windows" target="_blank"><img src="res/icon/windows-orig.png" title="Windows" width="32"></img></a>         <a href="http://www.ubuntu.com/" target="_blank"><img src="res/icon/ubuntu.png" width="32" title="Ubuntu"></img></a>  
<!-- <a href="https://www.apple.com/osx/"><img src="res/icon/mac.png" width="32"></img></a>   <a href="https://www.android.com/"><img src="res/icon/android.png" width="32"></img></a>  <a href="https://www.apple.com/ios/"><img src="res/icon/apple.png" width="32"></img></a>
-->

## Donations ##

`BTC:`
[`14VA6zJ46fdv4pagJEg4WNMYPiFn7RX1xS`](https://www.blockchain.com/btc/address/14VA6zJ46fdv4pagJEg4WNMYPiFn7RX1xS)

<a href="https://www.blockchain.com/btc/address/14VA6zJ46fdv4pagJEg4WNMYPiFn7RX1xS" target="_blank"><img src="res/icon/btc_addr.png" title="Donate_BTC" width="128"></img></a>

