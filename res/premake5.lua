-- premake5.lua
workspace "dokun"
    configurations { "Debug", "Release" }
    symbols 'On'
    cppdialect 'C++11'
    
    filter 'system:linux'
        platforms { 'x86_64' }
    filter 'system:macosx'
        platforms { 'x86_64' }
    filter 'system:windows'
        platforms { 'x86', 'x86_64' }
        defaultplatform 'x86_64'
		defines { '_CRT_SECURE_NO_WARNINGS' }
	filter {}

   filter "configurations:Debug"
       defines { "DEBUG" }
       symbols "On"
   filter "configurations:Release"
       defines { "NDEBUG" }
       optimize "Off"
   filter {}

    filter { "language:C++", "toolset:gcc" }
	    buildoptions { "-std=gnu++11", "-no-pie" }
	filter {}  	
--------------------------
project "dokun"
    kind "ConsoleApp"
    language "C++"
    defines { "DOKUN_OPENGL", "DOKUN_DEBUG", "DOKUN_BUILD_CONSOLE", "LUA_USE_POSIX", "FT2_BUILD_LIBRARY", } -- custom definitions
    targetdir ( "bin/%{cfg.buildcfg}" )
    files {
        -- dokun
        "src/engine.cpp", "src/window.cpp", "src/sprite.cpp", "src/model.cpp", "src/texture.cpp", "src/image.cpp", "src/vertex.cpp", "src/mouse.cpp", "src/keyboard.cpp", "src/joystick.cpp", "src/touch.cpp", "src/virtual.cpp", "src/brain.cpp", "src/audio.cpp", "src/sound.cpp", "src/music.cpp", "src/voice.cpp", "src/font.cpp", "src/ui.cpp", "src/widget.cpp", "src/button.cpp", "src/label.cpp", "src/edit.cpp", "src/progressbar.cpp", "src/slider.cpp", "src/toggle.cpp", "src/tooltip.cpp", "src/scrollbar.cpp", "src/list.cpp", "src/spinner.cpp", "src/menubar.cpp", "src/grid.cpp", "src/combo.cpp", "src/tab.cpp", "src/server.cpp", "src/client.cpp", "src/entity.cpp", "src/component.cpp", "src/camera.cpp", "src/level.cpp", "src/timer.cpp", "src/light.cpp", "src/shader.cpp", "src/resource.cpp", "src/animator.cpp", "src/data.cpp", "src/factory.cpp", "src/script.cpp", "src/console.cpp", "src/video.cpp", "src/plugin.cpp", "src/file.cpp", "src/event.cpp", "src/loader.cpp", "src/renderer.cpp", "src/process.cpp", "src/http.cpp",
        -- lua5.x
        "dependencies/lua/lua-5.3.5/src/lua.c", "dependencies/lua/lua-5.3.5/src/luac.c", "dependencies/lua/lua-5.3.5/src/lapi.c", "dependencies/lua/lua-5.3.5/src/lcode.c", "dependencies/lua/lua-5.3.5/src/lctype.c", "dependencies/lua/lua-5.3.5/src/ldebug.c", "dependencies/lua/lua-5.3.5/src/ldo.c", "dependencies/lua/lua-5.3.5/src/ldump.c", "dependencies/lua/lua-5.3.5/src/lfunc.c", "dependencies/lua/lua-5.3.5/src/lgc.c", "dependencies/lua/lua-5.3.5/src/llex.c", "dependencies/lua/lua-5.3.5/src/lmem.c", "dependencies/lua/lua-5.3.5/src/lobject.c", "dependencies/lua/lua-5.3.5/src/lopcodes.c", "dependencies/lua/lua-5.3.5/src/lparser.c", "dependencies/lua/lua-5.3.5/src/lstate.c", "dependencies/lua/lua-5.3.5/src/lstring.c", "dependencies/lua/lua-5.3.5/src/ltable.c", "dependencies/lua/lua-5.3.5/src/ltm.c", "dependencies/lua/lua-5.3.5/src/lundump.c", "dependencies/lua/lua-5.3.5/src/lvm.c", "dependencies/lua/lua-5.3.5/src/lzio.c", "dependencies/lua/lua-5.3.5/src/lauxlib.c", "dependencies/lua/lua-5.3.5/src/lbaselib.c", "dependencies/lua/lua-5.3.5/src/lbitlib.c", "dependencies/lua/lua-5.3.5/src/lcorolib.c", "dependencies/lua/lua-5.3.5/src/ldblib.c", "dependencies/lua/lua-5.3.5/src/liolib.c", "dependencies/lua/lua-5.3.5/src/lmathlib.c", "dependencies/lua/lua-5.3.5/src/loslib.c", "dependencies/lua/lua-5.3.5/src/lstrlib.c", "dependencies/lua/lua-5.3.5/src/ltablib.c", "dependencies/lua/lua-5.3.5/src/lutf8lib.c", "dependencies/lua/lua-5.3.5/src/loadlib.c", "dependencies/lua/lua-5.3.5/src/linit.c",
        -- glew
        "dependencies/glew/src/glew.c",
        -- zlib
        "dependencies/zlib/adler32.c", "dependencies/zlib/crc32.c", "dependencies/zlib/deflate.c", "dependencies/zlib/inflate.c", "dependencies/zlib/compress.c", "dependencies/zlib/gzclose.c", "dependencies/zlib/gzlib.c", "dependencies/zlib/gzread.c", "dependencies/zlib/gzwrite.c", "dependencies/zlib/infback.c", "dependencies/zlib/inffast.c", "dependencies/zlib/inftrees.c", "dependencies/zlib/trees.c", "dependencies/zlib/uncompr.c", "dependencies/zlib/zutil.c",
        -- png
        "dependencies/png/png.c", "dependencies/png/pngerror.c", "dependencies/png/pngget.c", "dependencies/png/pngmem.c", "dependencies/png/pngpread.c", "dependencies/png/pngread.c", "dependencies/png/pngrio.c", "dependencies/png/pngrtran.c", "dependencies/png/pngrutil.c", "dependencies/png/pngset.c", "dependencies/png/pngtrans", "dependencies/png/pngwio.c", "dependencies/png/pngwrite.c", "dependencies/png/pngwtran.c", "dependencies/png/pngwutil.c",
        -- jpeg
        "dependencies/jpeg/jaricom.c", "dependencies/jpeg/jcapimin.c", "dependencies/jpeg/jcapistd.c", "dependencies/jpeg/jcarith.c", "dependencies/jpeg/jccoefct.c", "dependencies/jpeg/jccolor.c", "dependencies/jpeg/jcdctmgr.c", "dependencies/jpeg/jchuff.c", "dependencies/jpeg/jcinit.c", "dependencies/jpeg/jcmainct.c", "dependencies/jpeg/jcmarker.c", "dependencies/jpeg/jcmaster.c", "dependencies/jpeg/jcomapi.c", "dependencies/jpeg/jcparam.c", "dependencies/jpeg/jcprepct.c", "dependencies/jpeg/jcsample.c", "dependencies/jpeg/jctrans.c", "dependencies/jpeg/jdapimin.c", "dependencies/jpeg/jdapistd.c", "dependencies/jpeg/jdarith.c", "dependencies/jpeg/jdatadst.c", "dependencies/jpeg/jdatasrc.c", "dependencies/jpeg/jdcoefct.c", "dependencies/jpeg/jdcolor.c", "dependencies/jpeg/jddctmgr.c", "dependencies/jpeg/jdhuff.c", "dependencies/jpeg/jdinput.c", "dependencies/jpeg/jdmainct.c", "dependencies/jpeg/jdmarker.c", "dependencies/jpeg/jdmaster.c", "dependencies/jpeg/jdmerge.c", "dependencies/jpeg/jdpostct.c", "dependencies/jpeg/jdsample.c", "dependencies/jpeg/jdtrans.c", "dependencies/jpeg/jerror.c", "dependencies/jpeg/jfdctflt.c", "dependencies/jpeg/jfdctfst.c", "dependencies/jpeg/jfdctint.c", "dependencies/jpeg/jidctflt.c", "dependencies/jpeg/jidctfst.c", "dependencies/jpeg/jidctint.c", "dependencies/jpeg/jquant1.c", "dependencies/jpeg/jquant2.c", "dependencies/jpeg/jutils.c", "dependencies/jpeg/jmemmgr.c",
        -- gif
        "dependencies/gif/lib/dgif_lib.c", "dependencies/gif/lib/egif_lib.c", "dependencies/gif/lib/gif_err.c", "dependencies/gif/lib/gif_font.c", "dependencies/gif/lib/gif_hash.c", "dependencies/gif/lib/gifalloc.c", "dependencies/gif/lib/openbsd-reallocarray.c", "dependencies/gif/lib/quantize.c",
        -- tiff
        "dependencies/tiff/libtiff/tif_aux.c", "dependencies/tiff/libtiff/tif_close.c", "dependencies/tiff/libtiff/tif_codec.c", "dependencies/tiff/libtiff/tif_color.c", "dependencies/tiff/libtiff/tif_compress.c", "dependencies/tiff/libtiff/tif_dir.c", "dependencies/tiff/libtiff/tif_dirinfo.c", "dependencies/tiff/libtiff/tif_dirread.c", "dependencies/tiff/libtiff/tif_dirwrite.c", "dependencies/tiff/libtiff/tif_dumpmode.c", "dependencies/tiff/libtiff/tif_error.c", "dependencies/tiff/libtiff/tif_extension.c", "dependencies/tiff/libtiff/tif_fax3.c", "dependencies/tiff/libtiff/tif_fax3sm.c", "dependencies/tiff/libtiff/tif_flush.c", "dependencies/tiff/libtiff/tif_getimage.c", "dependencies/tiff/libtiff/tif_jbig.c", "dependencies/tiff/libtiff/tif_jpeg_12.c", "dependencies/tiff/libtiff/tif_jpeg.c", "dependencies/tiff/libtiff/tif_luv.c", "dependencies/tiff/libtiff/tif_lzma.c", "dependencies/tiff/libtiff/tif_lzw.c", "dependencies/tiff/libtiff/tif_next.c", "dependencies/tiff/libtiff/tif_ojpeg.c", "dependencies/tiff/libtiff/tif_open.c", "dependencies/tiff/libtiff/tif_packbits.c", "dependencies/tiff/libtiff/tif_pixarlog.c", "dependencies/tiff/libtiff/tif_predict.c", "dependencies/tiff/libtiff/tif_print.c", "dependencies/tiff/libtiff/tif_read.c", "dependencies/tiff/libtiff/tif_stream.cxx", "dependencies/tiff/libtiff/tif_strip.c", "dependencies/tiff/libtiff/tif_swab.c", "dependencies/tiff/libtiff/tif_thunder.c", "dependencies/tiff/libtiff/tif_tile.c", "dependencies/tiff/libtiff/tif_version.c", "dependencies/tiff/libtiff/tif_warning.c", "dependencies/tiff/libtiff/tif_write.c", "dependencies/tiff/libtiff/tif_zip.c",       
        -- ogg
        "dependencies/ogg/src/bitwise.c", "dependencies/ogg/src/framing.c",
        -- vorbis
        "dependencies/vorbis/lib/analysis.c", "dependencies/vorbis/lib/bitrate.c", "dependencies/vorbis/lib/block.c", "dependencies/vorbis/lib/codebook.c", "dependencies/vorbis/lib/envelope.c", "dependencies/vorbis/lib/floor0.c", "dependencies/vorbis/lib/floor1.c", "dependencies/vorbis/lib/info.c", "dependencies/vorbis/lib/lookup.c", "dependencies/vorbis/lib/lpc.c", "dependencies/vorbis/lib/lsp.c", "dependencies/vorbis/lib/mapping0.c", "dependencies/vorbis/lib/mdct.c", "dependencies/vorbis/lib/psy.c", "dependencies/vorbis/lib/registry.c", "dependencies/vorbis/lib/res0.c", "dependencies/vorbis/lib/sharedbook.c", "dependencies/vorbis/lib/smallft.c", "dependencies/vorbis/lib/synthesis.c", "dependencies/vorbis/lib/vorbisenc.c", "dependencies/vorbis/lib/vorbisfile.c", "dependencies/vorbis/lib/window.c",
        -- theora
        "dependencies/theora/lib/analyze.c", "dependencies/theora/lib/apiwrapper.c", "dependencies/theora/lib/bitpack.c", "dependencies/theora/lib/cpu.c", "dependencies/theora/lib/decapiwrapper.c", "dependencies/theora/lib/decinfo.c", "dependencies/theora/lib/decode.c", "dependencies/theora/lib/dequant.c", "dependencies/theora/lib/encapiwrapper.c", "dependencies/theora/lib/encfrag.c", "dependencies/theora/lib/encinfo.c", "dependencies/theora/lib/encode.c", "dependencies/theora/lib/enquant.c", "dependencies/theora/lib/fdct.c", "dependencies/theora/lib/fragment.c", "dependencies/theora/lib/huffdec.c", "dependencies/theora/lib/huffenc.c", "dependencies/theora/lib/idct.c", "dependencies/theora/lib/info.c", "dependencies/theora/lib/internal.c", "dependencies/theora/lib/mathops.c", "dependencies/theora/lib/mcenc.c", "dependencies/theora/lib/quant.c", "dependencies/theora/lib/rate.c", "dependencies/theora/lib/state.c", "dependencies/theora/lib/tokenize.c",
        -- freetype
        "dependencies/freetype/src/autofit/autofit.c", "dependencies/freetype/src/base/ftbase.c", "dependencies/freetype/src/base/ftbbox.c", "dependencies/freetype/src/base/ftbdf.c", "dependencies/freetype/src/base/ftbitmap.c", "dependencies/freetype/src/base/ftcid.c", "dependencies/freetype/src/base/ftfstype.c", "dependencies/freetype/src/base/ftgasp.c", "dependencies/freetype/src/base/ftglyph.c", "dependencies/freetype/src/base/ftgxval.c", "dependencies/freetype/src/base/ftinit.c", "dependencies/freetype/src/base/ftmm.c", "dependencies/freetype/src/base/ftotval.c", "dependencies/freetype/src/base/ftpatent.c", "dependencies/freetype/src/base/ftpfr.c", "dependencies/freetype/src/base/ftstroke.c", "dependencies/freetype/src/base/ftsynth.c", "dependencies/freetype/src/base/ftsystem.c", "dependencies/freetype/src/base/fttype1.c", "dependencies/freetype/src/base/ftwinfnt.c", "dependencies/freetype/src/bdf/bdf.c", "dependencies/freetype/src/bzip2/ftbzip2.c", "dependencies/freetype/src/cache/ftcache.c", "dependencies/freetype/src/cff/cff.c", "dependencies/freetype/src/cid/type1cid.c", "dependencies/freetype/src/gzip/ftgzip.c", "dependencies/freetype/src/lzw/ftlzw.c", "dependencies/freetype/src/pcf/pcf.c", "dependencies/freetype/src/pfr/pfr.c", "dependencies/freetype/src/psaux/psaux.c", "dependencies/freetype/src/pshinter/pshinter.c", "dependencies/freetype/src/psnames/psnames.c", "dependencies/freetype/src/raster/raster.c", "dependencies/freetype/src/sfnt/sfnt.c", "dependencies/freetype/src/smooth/smooth.c", "dependencies/freetype/src/truetype/truetype.c", "dependencies/freetype/src/type1/type1.c", "dependencies/freetype/src/type42/type42.c", "dependencies/freetype/src/winfonts/winfnt.c",
        -- box2d
        "dependencies/box2d/Box2D/Box2D/Collision/b2BroadPhase.cpp", "dependencies/box2d/Box2D/Box2D/Collision/b2CollideCircle.cpp", "dependencies/box2d/Box2D/Box2D/Collision/b2CollideEdge.cpp", "dependencies/box2d/Box2D/Box2D/Collision/b2CollidePolygon.cpp", "dependencies/box2d/Box2D/Box2D/Collision/b2Collision.cpp", "dependencies/box2d/Box2D/Box2D/Collision/b2Distance.cpp", "dependencies/box2d/Box2D/Box2D/Collision/b2DynamicTree.cpp", "dependencies/box2d/Box2D/Box2D/Collision/b2TimeOfImpact.cpp", "dependencies/box2d/Box2D/Box2D/Collision/Shapes/b2CircleShape.cpp", "dependencies/box2d/Box2D/Box2D/Collision/Shapes/b2EdgeShape.cpp", "dependencies/box2d/Box2D/Box2D/Collision/Shapes/b2ChainShape.cpp", "dependencies/box2d/Box2D/Box2D/Collision/Shapes/b2PolygonShape.cpp", "dependencies/box2d/Box2D/Box2D/Common/b2BlockAllocator.cpp", "dependencies/box2d/Box2D/Box2D/Common/b2Draw.cpp", "dependencies/box2d/Box2D/Box2D/Common/b2Math.cpp", "dependencies/box2d/Box2D/Box2D/Common/b2Settings.cpp", "dependencies/box2d/Box2D/Box2D/Common/b2StackAllocator.cpp", "dependencies/box2d/Box2D/Box2D/Common/b2Timer.cpp", "dependencies/box2d/Box2D/Box2D/Dynamics/b2Body.cpp", "dependencies/box2d/Box2D/Box2D/Dynamics/b2ContactManager.cpp", "dependencies/box2d/Box2D/Box2D/Dynamics/b2Fixture.cpp", "dependencies/box2d/Box2D/Box2D/Dynamics/b2Island.cpp", "dependencies/box2d/Box2D/Box2D/Dynamics/b2World.cpp", "dependencies/box2d/Box2D/Box2D/Dynamics/b2WorldCallbacks.cpp", "dependencies/box2d/Box2D/Box2D/Dynamics/Contacts/b2CircleContact.cpp", "dependencies/box2d/Box2D/Box2D/Dynamics/Contacts/b2Contact.cpp", "dependencies/box2d/Box2D/Box2D/Dynamics/Contacts/b2ContactSolver.cpp", "dependencies/box2d/Box2D/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp", "dependencies/box2d/Box2D/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp", "dependencies/box2d/Box2D/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp", "dependencies/box2d/Box2D/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp", "dependencies/box2d/Box2D/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp", "dependencies/box2d/Box2D/Box2D/Dynamics/Contacts/b2PolygonContact.cpp", "dependencies/box2d/Box2D/Box2D/Dynamics/Joints/b2DistanceJoint.cpp", "dependencies/box2d/Box2D/Box2D/Dynamics/Joints/b2FrictionJoint.cpp", "dependencies/box2d/Box2D/Box2D/Dynamics/Joints/b2GearJoint.cpp", "dependencies/box2d/Box2D/Box2D/Dynamics/Joints/b2Joint.cpp", "dependencies/box2d/Box2D/Box2D/Dynamics/Joints/b2MotorJoint.cpp", "dependencies/box2d/Box2D/Box2D/Dynamics/Joints/b2MouseJoint.cpp", "dependencies/box2d/Box2D/Box2D/Dynamics/Joints/b2PrismaticJoint.cpp", "dependencies/box2d/Box2D/Box2D/Dynamics/Joints/b2PulleyJoint.cpp", "dependencies/box2d/Box2D/Box2D/Dynamics/Joints/b2RevoluteJoint.cpp", "dependencies/box2d/Box2D/Box2D/Dynamics/Joints/b2RopeJoint.cpp", "dependencies/box2d/Box2D/Box2D/Dynamics/Joints/b2WeldJoint.cpp", "dependencies/box2d/Box2D/Box2D/Dynamics/Joints/b2WheelJoint.cpp", "dependencies/box2d/Box2D/Box2D/Rope/b2Rope.cpp",
        -- sqlite
        "dependencies/sqlite/sqlite3.c",
        --
        --
        --
    }
    -- exclude these files
    removefiles { "src/build.txt", "src/OpenAL32.lib", "src/vulkan-1.lib", "libdokun.a", "src/matrix.cpp",  "src/game_old.cpp" } -- exclude specific files
    includedirs { "include",
       "dependencies/lua/lua-5.3.5/src", 
       "dependencies/glew/include", 
       "dependencies/zlib",
       "dependencies/png",
       "dependencies/freetype/include",
       "dependencies/freetype/include/freetype",
       "dependencies/freetype/include/freetype/config",
       "dependencies/freetype/include/freetype/internal",
       "dependencies/ogg/include",
       "dependencies/vorbis/include",
       "dependencies/vorbis/lib",
       "dependencies/theora/include",
       "dependencies/sqlite",
       --"dependencies/utf8/source",
       --"dependencies/openal/include",
       "dependencies/openal-soft/include",
       "dependencies/jpeg",
       "dependencies/jpeg/jpeg-6b",
       "dependencies/tiff/libtiff",
       "dependencies/gif/lib",
       --"dependencies/theoraplay",
       --"dependencies/theoraplayer/include",
       "dependencies/box2d/Box2D",
       --"dependencies/bullet3/src",
       --"dependencies/bullet3/Bullet3Common",
       --"dependencies/json/src",
       "dependencies/glm",
    } -- include dirs (where to look for includes)
    libdirs { "src/*" } -- library dirs (where to look for libraries)
    
    configuration { "windows" } -- Windows
    	files { "include/win32_header.h", "dependencies/tiff/libtiff/tif_win32.c", "dependencies/jpeg/jmemansi.c", "dependencies/freetype/builds/windows/ftdebug.c", "dependencies/freetype/src/base/ftver.rc", } -- source files and headers for specific platforms
        links { "glu32", "opengl32", "ws2_32" } -- libraries to link to;   to find location of a lib use: libdirs { os.findlib("X11") }

	configuration { "macosx" } -- MacOSX
		files { "include/macos_header.h", "dependencies/jpeg/jmemmac.c", }
        links { "Cocoa.framework" }

    configuration { "linux", "gmake" } -- Linux
		files { "include/linux_header.h", "dependencies/tiff/libtiff/tif_unix.c", "dependencies/jpeg/jmemansi.c", "dependencies/freetype/src/base/ftdebug.c", }  -- source files and headers for specific platforms
        links { "GL", "GLU", "openal", "X11", "xcb", "Xau", "Xdmcp", "pthread", "dl", "Box2D", } -- link to external libraries  to find location of a lib use: libdirs { os.findlib("X11") }
        includedirs { "/usr/include", "/usr/local/include" } -- added /usr/local/include (2019-08-05)
        libdirs { "/usr/lib/x86_64-linux-gnu", "/usr/lib/gcc/x86_64-linux-gnu" } -- added /usr/lib/gcc/x86_64-linux-gnu   (2019-08-05)
    
    
--------------------------
--project "libdokun"
--    kind "StaticLib"
--    language "C++"
--    defines { "DOKUN_OPENGL", "DOKUN_DEBUG" "DOKUN_BUILD_LIBRARY"}
--------------------------
--project "editor"
--    kind "ConsoleApp"
--    language "C++"
--    defines { "DOKUN_OPENGL", "DOKUN_DEBUG" "DOKUN_BUILD_LIBRARY"}
--    files { src/editor.cpp }
-- https://github.com/premake/premake-core/wiki
-- later on make doxyfile for documentation: http://www.doxygen.nl/
-- premake5 --cc=gcc --file='premake5.lua' --os=linux gmake
-- gmake or gmake2 # gmake recommended


