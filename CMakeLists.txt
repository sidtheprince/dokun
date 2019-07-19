cmake_minimum_required(VERSION 3.0) # Tested on 2.8 and higher (3.7 comes with vulkan support)

project(dokun)

set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")
set(PROJECT_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
#############
# platform
if(WIN32) # tested on Windows 10 with MinGW (GNU 6.3.0)   8MB
    message("WIN32 device detected")
    set(WINDOWS TRUE)
endif()
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux") # tested on Ubuntu 14.04(with GCC/G++4.8), 16.04(GCC/G++5.4.0)
    message("Linux device detected")
    set(LINUX TRUE)
    set(CMAKE_C_COMPILER   /usr/bin/gcc)
    set(CMAKE_CXX_COMPILER /usr/bin/g++)
endif()
#if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" ) message("MacOSX device detected" ) set(MACOSX  TRUE) endif()
#if(${CMAKE_SYSTEM_NAME} MATCHES "Android") message("Android device detected") set(ANDROID TRUE) endif()
#############
# use C++ 11 (REQUIRED)
if(CMAKE_VERSION VERSION_LESS "3.1")
if(CMAKE_COMPILER_IS_GNUCXX)
    message("cmake version less than 3.1. Using CMAKE_CXX_FLAGS")
    set(CMAKE_CXX_FLAGS "-std=gnu++11") # 98, 11, 14, 17, 20
endif()
else()
    message("cmake version 3.1 or greater. Using CMAKE_CXX_STANDARD")
    set(CMAKE_CXX_STANDARD 11) # supported in cmake version 3.1 and higher
endif() #
#############
# tell g++ not to make a position independent executable (PIE)
if(CMAKE_COMPILER_IS_GNUCXX)
    list(APPEND CMAKE_CXX_FLAGS "-no-pie")
endif()
#############
# engine (dokun)
set(dokun_res  dokun.rc      )
set(dokun_src  src/engine.cpp src/window.cpp src/sprite.cpp src/model.cpp src/texture.cpp src/image.cpp src/vertex.cpp src/mouse.cpp src/keyboard.cpp src/joystick.cpp src/touch.cpp src/virtual.cpp src/brain.cpp src/audio.cpp src/sound.cpp src/music.cpp src/voice.cpp src/font.cpp src/ui.cpp src/widget.cpp src/button.cpp src/label.cpp src/edit.cpp src/progressbar.cpp src/slider.cpp src/toggle.cpp src/tooltip.cpp src/scrollbar.cpp src/list.cpp src/spinner.cpp src/menubar.cpp src/grid.cpp src/combo.cpp src/tab.cpp src/server.cpp src/client.cpp src/entity.cpp src/component.cpp src/camera.cpp src/level.cpp src/timer.cpp src/light.cpp src/shader.cpp src/resource.cpp src/animator.cpp src/data.cpp src/factory.cpp src/script.cpp src/console.cpp src/video.cpp src/plugin.cpp src/file.cpp src/event.cpp src/loader.cpp src/renderer.cpp src/process.cpp src/http.cpp)
set(editor_src src/editor.cpp)
set(game_src   src/game.cpp  ) # game demo
#############
# scripting
set(lua51_src dependencies/lua/lua-5.1.5/src/lua.c dependencies/lua/lua-5.1.5/src/luac.c dependencies/lua/lua-5.1.5/src/lapi.c dependencies/lua/lua-5.1.5/src/lcode.c dependencies/lua/lua-5.1.5/src/ldebug.c dependencies/lua/lua-5.1.5/src/ldo.c dependencies/lua/lua-5.1.5/src/ldump.c dependencies/lua/lua-5.1.5/src/lfunc.c dependencies/lua/lua-5.1.5/src/lgc.c dependencies/lua/lua-5.1.5/src/llex.c dependencies/lua/lua-5.1.5/src/lmem.c dependencies/lua/lua-5.1.5/src/lobject.c dependencies/lua/lua-5.1.5/src/lopcodes.c dependencies/lua/lua-5.1.5/src/lparser.c dependencies/lua/lua-5.1.5/src/lstate.c dependencies/lua/lua-5.1.5/src/lstring.c dependencies/lua/lua-5.1.5/src/ltable.c dependencies/lua/lua-5.1.5/src/ltm.c dependencies/lua/lua-5.1.5/src/lundump.c dependencies/lua/lua-5.1.5/src/lvm.c dependencies/lua/lua-5.1.5/src/lzio.c dependencies/lua/lua-5.1.5/src/lauxlib.c dependencies/lua/lua-5.1.5/src/lbaselib.c dependencies/lua/lua-5.1.5/src/ldblib.c dependencies/lua/lua-5.1.5/src/liolib.c dependencies/lua/lua-5.1.5/src/lmathlib.c dependencies/lua/lua-5.1.5/src/loslib.c dependencies/lua/lua-5.1.5/src/lstrlib.c dependencies/lua/lua-5.1.5/src/ltablib.c dependencies/lua/lua-5.1.5/src/loadlib.c dependencies/lua/lua-5.1.5/src/linit.c)
set(lua52_src dependencies/lua/lua-5.2.4/src/lua.c dependencies/lua/lua-5.2.4/src/luac.c dependencies/lua/lua-5.2.4/src/lapi.c dependencies/lua/lua-5.2.4/src/lcode.c dependencies/lua/lua-5.2.4/src/lctype.c dependencies/lua/lua-5.2.4/src/ldebug.c dependencies/lua/lua-5.2.4/src/ldo.c dependencies/lua/lua-5.2.4/src/ldump.c dependencies/lua/lua-5.2.4/src/lfunc.c dependencies/lua/lua-5.2.4/src/lgc.c dependencies/lua/lua-5.2.4/src/llex.c dependencies/lua/lua-5.2.4/src/lmem.c dependencies/lua/lua-5.2.4/src/lobject.c dependencies/lua/lua-5.2.4/src/lopcodes.c dependencies/lua/lua-5.2.4/src/lparser.c dependencies/lua/lua-5.2.4/src/lstate.c dependencies/lua/lua-5.2.4/src/lstring.c dependencies/lua/lua-5.2.4/src/ltable.c dependencies/lua/lua-5.2.4/src/ltm.c dependencies/lua/lua-5.2.4/src/lundump.c dependencies/lua/lua-5.2.4/src/lvm.c dependencies/lua/lua-5.2.4/src/lzio.c dependencies/lua/lua-5.2.4/src/lauxlib.c dependencies/lua/lua-5.2.4/src/lbaselib.c dependencies/lua/lua-5.2.4/src/lbitlib.c dependencies/lua/lua-5.2.4/src/lcorolib.c dependencies/lua/lua-5.2.4/src/ldblib.c dependencies/lua/lua-5.2.4/src/liolib.c dependencies/lua/lua-5.2.4/src/lmathlib.c dependencies/lua/lua-5.2.4/src/loslib.c dependencies/lua/lua-5.2.4/src/lstrlib.c dependencies/lua/lua-5.2.4/src/ltablib.c dependencies/lua/lua-5.2.4/src/loadlib.c dependencies/lua/lua-5.2.4/src/linit.c)                                           # 5.2 added lctype.c, lbitlib.c, and lcorolib.c
set(lua53_src dependencies/lua/lua-5.3.5/src/lua.c dependencies/lua/lua-5.3.5/src/luac.c dependencies/lua/lua-5.3.5/src/lapi.c dependencies/lua/lua-5.3.5/src/lcode.c dependencies/lua/lua-5.3.5/src/lctype.c dependencies/lua/lua-5.3.5/src/ldebug.c dependencies/lua/lua-5.3.5/src/ldo.c dependencies/lua/lua-5.3.5/src/ldump.c dependencies/lua/lua-5.3.5/src/lfunc.c dependencies/lua/lua-5.3.5/src/lgc.c dependencies/lua/lua-5.3.5/src/llex.c dependencies/lua/lua-5.3.5/src/lmem.c dependencies/lua/lua-5.3.5/src/lobject.c dependencies/lua/lua-5.3.5/src/lopcodes.c dependencies/lua/lua-5.3.5/src/lparser.c dependencies/lua/lua-5.3.5/src/lstate.c dependencies/lua/lua-5.3.5/src/lstring.c dependencies/lua/lua-5.3.5/src/ltable.c dependencies/lua/lua-5.3.5/src/ltm.c dependencies/lua/lua-5.3.5/src/lundump.c dependencies/lua/lua-5.3.5/src/lvm.c dependencies/lua/lua-5.3.5/src/lzio.c dependencies/lua/lua-5.3.5/src/lauxlib.c dependencies/lua/lua-5.3.5/src/lbaselib.c dependencies/lua/lua-5.3.5/src/lbitlib.c dependencies/lua/lua-5.3.5/src/lcorolib.c dependencies/lua/lua-5.3.5/src/ldblib.c dependencies/lua/lua-5.3.5/src/liolib.c dependencies/lua/lua-5.3.5/src/lmathlib.c dependencies/lua/lua-5.3.5/src/loslib.c dependencies/lua/lua-5.3.5/src/lstrlib.c dependencies/lua/lua-5.3.5/src/ltablib.c dependencies/lua/lua-5.3.5/src/lutf8lib.c dependencies/lua/lua-5.3.5/src/loadlib.c dependencies/lua/lua-5.3.5/src/linit.c) # 5.3 added lutf8lib
set(DOKUN_LUA51 OFF CACHE BOOL "Lua 5.1.5")
set(DOKUN_LUA52 OFF CACHE BOOL "Lua 5.2.4")
set(DOKUN_LUA53 OFF CACHE BOOL "Lua 5.3.5")
if(DOKUN_LUA51) # cmake -G"..Makefiles" -DDOKUN_LUA51=ON
    set(lua_src ${lua51_src})
	add_definitions(-DDOKUN_LUA51)
endif()
if(DOKUN_LUA52) # cmake -G"..Makefiles" -DDOKUN_LUA52=ON
    set(lua_src ${lua52_src})
endif()
if(DOKUN_LUA53) # cmake -G"..Makefiles" -DDOKUN_LUA53=ON
    set(lua_src ${lua53_src})
    add_definitions(-DLUA_LIBRARY) # required if using lua 5.3
endif()
if(NOT lua_src)
    message("-- Lua version not specified. Using Lua 5.3 by default")
	set(DOKUN_LUA53 ON)
	set(lua_src ${lua53_src})
	add_definitions(-DLUA_LIBRARY) # lua 5.3
endif()
if(LINUX)
add_definitions(-DLUA_USE_MKSTEMP) 
endif()
#############
# GPU
if(WIN32)
    set(opengl_src opengl32.lib glu32.lib)
    set(vulkan_src vulkan-1.lib)
    set(Vulkan_LIBRARY C:/Windows/SysWOW64/vulkan-1.dll) # 64-bit Windows
    set(Vulkan_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/vulkan)
endif()
if(LINUX)
    set(opengl_src libGL.so libGLU.so)
    set(OpenGL_GL_PREFERENCE "LEGACY") # LEGACY(libGL.so) OR GLVND(libOpenGL.so and libGLX.so)
    set(vulkan_src libvulkan.so      )
endif()
#############
# window + screen
set(win32_src)
set(x11_src libX11.a libxcb.a libXau.a libXdmcp.a)
set(cocoa_src)
#############
# input
set(freevr_src)
set(biosig_src)
#############
# extension
set(glew_src  dependencies/glew/src/glew.c) # glew-2.1.0 (this version adds support for OpenGL 4.6) (http://glew.sourceforge.net/)
set(posix_src libpthread.so libdl.so)
set(zlib_src  dependencies/zlib/adler32.c dependencies/zlib/crc32.c dependencies/zlib/deflate.c dependencies/zlib/inflate.c dependencies/zlib/compress.c dependencies/zlib/gzclose.c dependencies/zlib/gzlib.c dependencies/zlib/gzread.c dependencies/zlib/gzwrite.c dependencies/zlib/infback.c dependencies/zlib/inffast.c dependencies/zlib/inftrees.c dependencies/zlib/trees.c dependencies/zlib/uncompr.c dependencies/zlib/zutil.c) # required for libpng
#############
# image
set(png_src  dependencies/png/png.c dependencies/png/pngerror.c dependencies/png/pngget.c dependencies/png/pngmem.c dependencies/png/pngpread.c dependencies/png/pngread.c dependencies/png/pngrio.c dependencies/png/pngrtran.c dependencies/png/pngrutil.c dependencies/png/pngset.c dependencies/png/pngtrans dependencies/png/pngwio.c dependencies/png/pngwrite.c dependencies/png/pngwtran.c dependencies/png/pngwutil.c) # libpng-1.6.35 (http://www.libpng.org/pub/png/libpng.html)(https://libpng.sourceforge.io/)
set(jpeg_src dependencies/jpeg/jaricom.c dependencies/jpeg/jcapimin.c dependencies/jpeg/jcapistd.c dependencies/jpeg/jcarith.c dependencies/jpeg/jccoefct.c dependencies/jpeg/jccolor.c dependencies/jpeg/jcdctmgr.c dependencies/jpeg/jchuff.c dependencies/jpeg/jcinit.c dependencies/jpeg/jcmainct.c dependencies/jpeg/jcmarker.c dependencies/jpeg/jcmaster.c dependencies/jpeg/jcomapi.c dependencies/jpeg/jcparam.c dependencies/jpeg/jcprepct.c dependencies/jpeg/jcsample.c dependencies/jpeg/jctrans.c dependencies/jpeg/jdapimin.c dependencies/jpeg/jdapistd.c dependencies/jpeg/jdarith.c dependencies/jpeg/jdatadst.c dependencies/jpeg/jdatasrc.c dependencies/jpeg/jdcoefct.c dependencies/jpeg/jdcolor.c dependencies/jpeg/jddctmgr.c dependencies/jpeg/jdhuff.c dependencies/jpeg/jdinput.c dependencies/jpeg/jdmainct.c dependencies/jpeg/jdmarker.c dependencies/jpeg/jdmaster.c dependencies/jpeg/jdmerge.c dependencies/jpeg/jdpostct.c dependencies/jpeg/jdsample.c dependencies/jpeg/jdtrans.c dependencies/jpeg/jerror.c dependencies/jpeg/jfdctflt.c dependencies/jpeg/jfdctfst.c dependencies/jpeg/jfdctint.c dependencies/jpeg/jidctflt.c dependencies/jpeg/jidctfst.c dependencies/jpeg/jidctint.c dependencies/jpeg/jquant1.c dependencies/jpeg/jquant2.c dependencies/jpeg/jutils.c dependencies/jpeg/jmemmgr.c) # jpeg-9b (http://ijg.org/) (consider https://libjpeg-turbo.org/ , a "2-6x times" faster fork of libjpeg)
set(gif_src  dependencies/gif/lib/dgif_lib.c dependencies/gif/lib/egif_lib.c dependencies/gif/lib/gif_err.c dependencies/gif/lib/gif_font.c dependencies/gif/lib/gif_hash.c dependencies/gif/lib/gifalloc.c dependencies/gif/lib/openbsd-reallocarray.c dependencies/gif/lib/quantize.c) # giflib-5.1.4 (http://giflib.sourceforge.net/)
set(tiff_src dependencies/tiff/libtiff/tif_aux.c dependencies/tiff/libtiff/tif_close.c dependencies/tiff/libtiff/tif_codec.c dependencies/tiff/libtiff/tif_color.c dependencies/tiff/libtiff/tif_compress.c dependencies/tiff/libtiff/tif_dir.c dependencies/tiff/libtiff/tif_dirinfo.c dependencies/tiff/libtiff/tif_dirread.c dependencies/tiff/libtiff/tif_dirwrite.c dependencies/tiff/libtiff/tif_dumpmode.c dependencies/tiff/libtiff/tif_error.c dependencies/tiff/libtiff/tif_extension.c dependencies/tiff/libtiff/tif_fax3.c dependencies/tiff/libtiff/tif_fax3sm.c dependencies/tiff/libtiff/tif_flush.c dependencies/tiff/libtiff/tif_getimage.c dependencies/tiff/libtiff/tif_jbig.c dependencies/tiff/libtiff/tif_jpeg_12.c dependencies/tiff/libtiff/tif_jpeg.c dependencies/tiff/libtiff/tif_luv.c dependencies/tiff/libtiff/tif_lzma.c dependencies/tiff/libtiff/tif_lzw.c dependencies/tiff/libtiff/tif_next.c dependencies/tiff/libtiff/tif_ojpeg.c dependencies/tiff/libtiff/tif_open.c dependencies/tiff/libtiff/tif_packbits.c dependencies/tiff/libtiff/tif_pixarlog.c dependencies/tiff/libtiff/tif_predict.c dependencies/tiff/libtiff/tif_print.c dependencies/tiff/libtiff/tif_read.c dependencies/tiff/libtiff/tif_stream.cxx dependencies/tiff/libtiff/tif_strip.c dependencies/tiff/libtiff/tif_swab.c dependencies/tiff/libtiff/tif_thunder.c dependencies/tiff/libtiff/tif_tile.c dependencies/tiff/libtiff/tif_version.c dependencies/tiff/libtiff/tif_warning.c dependencies/tiff/libtiff/tif_write.c dependencies/tiff/libtiff/tif_zip.c) #tiff-4.0.7  tiffconf.h is generated with ./configure (http://simplesystems.org/libtiff/)
if(WINDOWS)
    list(APPEND tiff_src dependencies/tiff/libtiff/tif_win32.c)
	list(APPEND jpeg_src dependencies/jpeg/jmemansi.c         )
endif()
if(LINUX)
    list(APPEND tiff_src dependencies/tiff/libtiff/tif_unix.c)
	list(APPEND jpeg_src dependencies/jpeg/jmemansi.c        )
endif()
if(MACOSX)
    list(APPEND jpeg_src dependencies/jpeg/jmemmac.c)
endif()
#############
# container - for vorbis and theora
set(ogg_src dependencies/ogg/src/bitwise.c dependencies/ogg/src/framing.c) # libogg-1.3.3  generate config_types.h using ./configure # (https://xiph.org/downloads/)
#############
# audio (codec)
set(vorbis_src dependencies/vorbis/lib/analysis.c dependencies/vorbis/lib/bitrate.c dependencies/vorbis/lib/block.c dependencies/vorbis/lib/codebook.c dependencies/vorbis/lib/envelope.c dependencies/vorbis/lib/floor0.c dependencies/vorbis/lib/floor1.c dependencies/vorbis/lib/info.c dependencies/vorbis/lib/lookup.c dependencies/vorbis/lib/lpc.c dependencies/vorbis/lib/lsp.c dependencies/vorbis/lib/mapping0.c dependencies/vorbis/lib/mdct.c dependencies/vorbis/lib/psy.c dependencies/vorbis/lib/registry.c dependencies/vorbis/lib/res0.c dependencies/vorbis/lib/sharedbook.c dependencies/vorbis/lib/smallft.c dependencies/vorbis/lib/synthesis.c dependencies/vorbis/lib/vorbisenc.c dependencies/vorbis/lib/vorbisfile.c dependencies/vorbis/lib/window.c) # vorbis-1.3.6 (https://xiph.org/downloads/)
set(celt_src   dependencies/opus/celt/bands.c dependencies/opus/celt/celt.c dependencies/opus/celt/celt_decoder.c dependencies/opus/celt/celt_encoder.c dependencies/opus/celt/celt_lpc.c dependencies/opus/celt/cwrs.c dependencies/opus/celt/entcode.c dependencies/opus/celt/entdec.c dependencies/opus/celt/entenc.c dependencies/opus/celt/kiss_fft.c dependencies/opus/celt/laplace.c dependencies/opus/celt/mathops.c dependencies/opus/celt/mdct.c dependencies/opus/celt/modes.c dependencies/opus/celt/pitch.c dependencies/opus/celt/quant_bands.c dependencies/opus/celt/rate.c dependencies/opus/celt/vq.c) # Xiph.Org's Celt
set(silk_src   dependencies/opus/silk/A2NLSF.c dependencies/opus/silk/ana_filt_bank_1.c dependencies/opus/silk/biquad_alt.c dependencies/opus/silk/bwexpander.c dependencies/opus/silk/bwexpander_32.c dependencies/opus/silk/check_control_input.c dependencies/opus/silk/CNG.c dependencies/opus/silk/code_signs.c dependencies/opus/silk/control_audio_bandwidth.c dependencies/opus/silk/control_codec.c dependencies/opus/silk/control_SNR.c dependencies/opus/silk/debug.c dependencies/opus/silk/decoder_set_fs.c dependencies/opus/silk/decode_core.c dependencies/opus/silk/decode_frame.c dependencies/opus/silk/decode_indices.c dependencies/opus/silk/decode_parameters.c dependencies/opus/silk/decode_pitch.c dependencies/opus/silk/decode_pulses.c dependencies/opus/silk/dec_API.c dependencies/opus/silk/encode_indices.c dependencies/opus/silk/encode_pulses.c dependencies/opus/silk/enc_API.c dependencies/opus/silk/gain_quant.c dependencies/opus/silk/HP_variable_cutoff.c dependencies/opus/silk/init_decoder.c dependencies/opus/silk/init_encoder.c dependencies/opus/silk/inner_prod_aligned.c dependencies/opus/silk/interpolate.c dependencies/opus/silk/lin2log.c dependencies/opus/silk/log2lin.c dependencies/opus/silk/LPC_analysis_filter.c dependencies/opus/silk/LPC_inv_pred_gain.c dependencies/opus/silk/LP_variable_cutoff.c dependencies/opus/silk/NLSF2A.c dependencies/opus/silk/NLSF_decode.c dependencies/opus/silk/NLSF_del_dec_quant.c dependencies/opus/silk/NLSF_encode.c dependencies/opus/silk/NLSF_stabilize.c dependencies/opus/silk/NLSF_unpack.c dependencies/opus/silk/NLSF_VQ.c dependencies/opus/silk/NLSF_VQ_weights_laroia.c dependencies/opus/silk/NSQ.c dependencies/opus/silk/NSQ_del_dec.c dependencies/opus/silk/pitch_est_tables.c dependencies/opus/silk/PLC.c dependencies/opus/silk/process_NLSFs.c dependencies/opus/silk/quant_LTP_gains.c dependencies/opus/silk/resampler.c dependencies/opus/silk/resampler_down2.c dependencies/opus/silk/resampler_down2_3.c dependencies/opus/silk/resampler_private_AR2.c dependencies/opus/silk/resampler_private_down_FIR.c dependencies/opus/silk/resampler_private_IIR_FIR.c dependencies/opus/silk/resampler_private_up2_HQ.c dependencies/opus/silk/resampler_rom.c dependencies/opus/silk/shell_coder.c dependencies/opus/silk/sigm_Q15.c dependencies/opus/silk/sort.c dependencies/opus/silk/stereo_decode_pred.c dependencies/opus/silk/stereo_encode_pred.c dependencies/opus/silk/stereo_find_predictor.c dependencies/opus/silk/stereo_LR_to_MS.c dependencies/opus/silk/stereo_MS_to_LR.c dependencies/opus/silk/stereo_quant_pred.c dependencies/opus/silk/sum_sqr_shift.c dependencies/opus/silk/tables_gain.c dependencies/opus/silk/tables_LTP.c dependencies/opus/silk/tables_NLSF_CB_NB_MB.c dependencies/opus/silk/tables_NLSF_CB_WB.c dependencies/opus/silk/tables_other.c dependencies/opus/silk/tables_pitch_lag.c dependencies/opus/silk/tables_pulses_per_block.c dependencies/opus/silk/table_LSF_cos.c dependencies/opus/silk/VAD.c dependencies/opus/silk/VQ_WMat_EC.c     dependencies/opus/silk/float/apply_sine_window_FLP.c dependencies/opus/silk/float/autocorrelation_FLP.c dependencies/opus/silk/float/burg_modified_FLP.c dependencies/opus/silk/float/bwexpander_FLP.c dependencies/opus/silk/float/corrMatrix_FLP.c dependencies/opus/silk/float/encode_frame_FLP.c dependencies/opus/silk/float/energy_FLP.c dependencies/opus/silk/float/find_LPC_FLP.c dependencies/opus/silk/float/find_LTP_FLP.c dependencies/opus/silk/float/find_pitch_lags_FLP.c dependencies/opus/silk/float/find_pred_coefs_FLP.c dependencies/opus/silk/float/inner_product_FLP.c dependencies/opus/silk/float/k2a_FLP.c dependencies/opus/silk/float/levinsondurbin_FLP.c dependencies/opus/silk/float/LPC_analysis_filter_FLP.c dependencies/opus/silk/float/LPC_inv_pred_gain_FLP.c dependencies/opus/silk/float/LTP_analysis_filter_FLP.c dependencies/opus/silk/float/LTP_scale_ctrl_FLP.c dependencies/opus/silk/float/noise_shape_analysis_FLP.c dependencies/opus/silk/float/pitch_analysis_core_FLP.c dependencies/opus/silk/float/prefilter_FLP.c dependencies/opus/silk/float/process_gains_FLP.c dependencies/opus/silk/float/regularize_correlations_FLP.c dependencies/opus/silk/float/residual_energy_FLP.c dependencies/opus/silk/float/scale_copy_vector_FLP.c dependencies/opus/silk/float/scale_vector_FLP.c dependencies/opus/silk/float/schur_FLP.c dependencies/opus/silk/float/solve_LS_FLP.c dependencies/opus/silk/float/sort_FLP.c dependencies/opus/silk/float/warped_autocorrelation_FLP.c dependencies/opus/silk/float/wrappers_FLP.c) # Skype Ltd.'s Silk
set(opus_src   dependencies/opus/src/analysis.c dependencies/opus/src/mlp.c dependencies/opus/src/mlp_data.c dependencies/opus/src/opus.c dependencies/opus/src/opus_decoder.c dependencies/opus/src/opus_encoder.c dependencies/opus/src/opus_multistream.c dependencies/opus/src/opus_multistream_decoder.c dependencies/opus/src/opus_multistream_encoder.c dependencies/opus/src/repacketizer.c ${celt_src} ${silk_src})
add_definitions(-DUSE_ALLOCA)
add_definitions(-DOPUS_BUILD)
#############
# video (codec)
set(theora_src dependencies/theora/lib/analyze.c dependencies/theora/lib/apiwrapper.c dependencies/theora/lib/bitpack.c dependencies/theora/lib/cpu.c dependencies/theora/lib/decapiwrapper.c dependencies/theora/lib/decinfo.c dependencies/theora/lib/decode.c dependencies/theora/lib/dequant.c dependencies/theora/lib/encapiwrapper.c dependencies/theora/lib/encfrag.c dependencies/theora/lib/encinfo.c dependencies/theora/lib/encode.c dependencies/theora/lib/enquant.c dependencies/theora/lib/fdct.c dependencies/theora/lib/fragment.c dependencies/theora/lib/huffdec.c dependencies/theora/lib/huffenc.c dependencies/theora/lib/idct.c dependencies/theora/lib/info.c dependencies/theora/lib/internal.c dependencies/theora/lib/mathops.c dependencies/theora/lib/mcenc.c dependencies/theora/lib/quant.c dependencies/theora/lib/rate.c dependencies/theora/lib/state.c dependencies/theora/lib/tokenize.c) # libtheora-1.1.1 (https://xiph.org/downloads/)
#############
# font
set(freetype_src dependencies/freetype/src/autofit/autofit.c dependencies/freetype/src/base/ftbase.c dependencies/freetype/src/base/ftbbox.c dependencies/freetype/src/base/ftbdf.c dependencies/freetype/src/base/ftbitmap.c dependencies/freetype/src/base/ftcid.c dependencies/freetype/src/base/ftfntfmt.c dependencies/freetype/src/base/ftfstype.c dependencies/freetype/src/base/ftgasp.c dependencies/freetype/src/base/ftglyph.c dependencies/freetype/src/base/ftgxval.c dependencies/freetype/src/base/ftinit.c dependencies/freetype/src/base/ftlcdfil.c dependencies/freetype/src/base/ftmm.c dependencies/freetype/src/base/ftotval.c dependencies/freetype/src/base/ftpatent.c dependencies/freetype/src/base/ftpfr.c dependencies/freetype/src/base/ftstroke.c dependencies/freetype/src/base/ftsynth.c dependencies/freetype/src/base/ftsystem.c dependencies/freetype/src/base/fttype1.c dependencies/freetype/src/base/ftwinfnt.c dependencies/freetype/src/bdf/bdf.c dependencies/freetype/src/bzip2/ftbzip2.c dependencies/freetype/src/cache/ftcache.c dependencies/freetype/src/cff/cff.c dependencies/freetype/src/cid/type1cid.c dependencies/freetype/src/gzip/ftgzip.c dependencies/freetype/src/lzw/ftlzw.c dependencies/freetype/src/pcf/pcf.c dependencies/freetype/src/pfr/pfr.c dependencies/freetype/src/psaux/psaux.c dependencies/freetype/src/pshinter/pshinter.c dependencies/freetype/src/psnames/psnames.c dependencies/freetype/src/raster/raster.c dependencies/freetype/src/sfnt/sfnt.c dependencies/freetype/src/smooth/smooth.c dependencies/freetype/src/truetype/truetype.c dependencies/freetype/src/type1/type1.c dependencies/freetype/src/type42/type42.c dependencies/freetype/src/winfonts/winfnt.c) #freetype-2.8.0 (https://www.freetype.org/download.html) (https://download.savannah.gnu.org/releases/freetype/)
add_definitions(-DFT2_BUILD_LIBRARY)
#############
# network
set(winsocket_src ws2_32.lib)
#############
# physics
set(box2d_src dependencies/box2d/Box2D/Box2D/Collision/b2BroadPhase.cpp dependencies/box2d/Box2D/Box2D/Collision/b2CollideCircle.cpp dependencies/box2d/Box2D/Box2D/Collision/b2CollideEdge.cpp dependencies/box2d/Box2D/Box2D/Collision/b2CollidePolygon.cpp dependencies/box2d/Box2D/Box2D/Collision/b2Collision.cpp dependencies/box2d/Box2D/Box2D/Collision/b2Distance.cpp dependencies/box2d/Box2D/Box2D/Collision/b2DynamicTree.cpp dependencies/box2d/Box2D/Box2D/Collision/b2TimeOfImpact.cpp dependencies/box2d/Box2D/Box2D/Collision/Shapes/b2CircleShape.cpp dependencies/box2d/Box2D/Box2D/Collision/Shapes/b2EdgeShape.cpp dependencies/box2d/Box2D/Box2D/Collision/Shapes/b2ChainShape.cpp dependencies/box2d/Box2D/Box2D/Collision/Shapes/b2PolygonShape.cpp dependencies/box2d/Box2D/Box2D/Common/b2BlockAllocator.cpp dependencies/box2d/Box2D/Box2D/Common/b2Draw.cpp dependencies/box2d/Box2D/Box2D/Common/b2Math.cpp dependencies/box2d/Box2D/Box2D/Common/b2Settings.cpp dependencies/box2d/Box2D/Box2D/Common/b2StackAllocator.cpp dependencies/box2d/Box2D/Box2D/Common/b2Timer.cpp dependencies/box2d/Box2D/Box2D/Dynamics/b2Body.cpp dependencies/box2d/Box2D/Box2D/Dynamics/b2ContactManager.cpp dependencies/box2d/Box2D/Box2D/Dynamics/b2Fixture.cpp dependencies/box2d/Box2D/Box2D/Dynamics/b2Island.cpp dependencies/box2d/Box2D/Box2D/Dynamics/b2World.cpp dependencies/box2d/Box2D/Box2D/Dynamics/b2WorldCallbacks.cpp dependencies/box2d/Box2D/Box2D/Dynamics/Contacts/b2CircleContact.cpp dependencies/box2d/Box2D/Box2D/Dynamics/Contacts/b2Contact.cpp dependencies/box2d/Box2D/Box2D/Dynamics/Contacts/b2ContactSolver.cpp dependencies/box2d/Box2D/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp dependencies/box2d/Box2D/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp dependencies/box2d/Box2D/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp dependencies/box2d/Box2D/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp dependencies/box2d/Box2D/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp dependencies/box2d/Box2D/Box2D/Dynamics/Contacts/b2PolygonContact.cpp dependencies/box2d/Box2D/Box2D/Dynamics/Joints/b2DistanceJoint.cpp dependencies/box2d/Box2D/Box2D/Dynamics/Joints/b2FrictionJoint.cpp dependencies/box2d/Box2D/Box2D/Dynamics/Joints/b2GearJoint.cpp dependencies/box2d/Box2D/Box2D/Dynamics/Joints/b2Joint.cpp dependencies/box2d/Box2D/Box2D/Dynamics/Joints/b2MotorJoint.cpp dependencies/box2d/Box2D/Box2D/Dynamics/Joints/b2MouseJoint.cpp dependencies/box2d/Box2D/Box2D/Dynamics/Joints/b2PrismaticJoint.cpp dependencies/box2d/Box2D/Box2D/Dynamics/Joints/b2PulleyJoint.cpp dependencies/box2d/Box2D/Box2D/Dynamics/Joints/b2RevoluteJoint.cpp dependencies/box2d/Box2D/Box2D/Dynamics/Joints/b2RopeJoint.cpp dependencies/box2d/Box2D/Box2D/Dynamics/Joints/b2WeldJoint.cpp dependencies/box2d/Box2D/Box2D/Dynamics/Joints/b2WheelJoint.cpp dependencies/box2d/Box2D/Box2D/Rope/b2Rope.cpp) # add_definitions(-DBOX2D_BUILD_STATIC)
set(bullet3_src)
#############
# video (playback)
set(??_src)
#############
# audio (playback)
if(WIN32)
set(openal_src ${CMAKE_CURRENT_SOURCE_DIR}/src/OpenAL32.lib)
else()
set(openal_src libopenal.so)
endif()
#############
# data (serialization; encryption)
set(sqlite_src dependencies/sqlite/sqlite3.c)
set(xml_src) # libxml2   install with ./configure --help ->  make -> make install (http://xmlsoft.org/xml.html) (can be used for reading Collada Dae(.dae) and Scalable Vector Graphics(.svg)) (tutorial: http://www.linuxfromscratch.org/blfs/view/svn/general/libxml2.html)(consider http://rapidxml.sourceforge.net/)
add_definitions(-DLIBXML_STATIC)
set(openssl_src ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/openssl/libssl.a ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/openssl/libcrypto.a) # openssl-1.1.1 (https://www.openssl.org/source/)
#############
# unicode
set(icu_src)
#############
if(LINUX)
   include_directories(/usr/include             )
   link_directories   (/usr/lib/x86_64-linux-gnu)
endif()
#############
# include_dir (.h and .hpp)
if(DOKUN_LUA51)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lua/lua-5.1.5/src)
endif()
if(DOKUN_LUA52)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lua/lua-5.2.4/src)
endif()
if(DOKUN_LUA53)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lua/lua-5.3.5/src)
endif()
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glew/include 
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/png
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/zlib
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/freetype/include
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/freetype/include/freetype
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/freetype/include/freetype/config
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/ogg/include
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/vorbis/include
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/vorbis/lib
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/theora/include
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/sqlite
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/utf8/source
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/xml
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/xml/include
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/xml/include/libxml
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/openal/include
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/openal-soft/include
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/jpeg
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/jpeg/jpeg-6b
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/tiff/libtiff
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/gif/lib
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/flac
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/flac/include
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/flac/src/libFLAC/include
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/opus/include
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/opus/celt # Xiph.Org's Celt
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/opus/silk # Skype Limited's Silk
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/opus/silk/fixed
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/opus/silk/float
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/opus/silk/mips
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/opus/win32
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/theoraplay
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/theoraplayer/include
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/biosig
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/mxe/src
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glfw/include
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/SDL2/SDL2-2.0.4/include
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/box2d/Box2D
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/bullet3/src
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/bullet3/Bullet3Common
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/portaudio/include
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/json/src
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glm # todo: remove glm and make a custom matrix library
   ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/openssl/include
)
#############
# static_library_dir (.a or .lib)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/src) # stored in 'src' since libraries are precompiled sources
#############
#############
# API definitions
add_definitions(-DDOKUN_OPENGL) # add_definitions(-DDOKUN_VULKAN)
add_definitions(-DDOKUN_DEBUG ) # dokun debug mode
#############
# output folder - '.a' and '.lib' will go in folder "src"
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/src)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/src)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}    )
#############
# build dokun
add_executable(dokun           ${dokun_src} ${lua_src} ${png_src} ${gif_src} ${jpeg_src} ${tiff_src} ${ogg_src} ${vorbis_src} ${theora_src} ${freetype_src} ${box2d_src} ${bullet3_src} ${sqlite_src} ${xml_src} ${zlib_src} ${glew_src} ${dokun_res}  )
add_library   (libdokun STATIC ${dokun_src} ${lua_src} ${png_src} ${gif_src} ${jpeg_src} ${tiff_src} ${ogg_src} ${vorbis_src} ${theora_src} ${freetype_src} ${box2d_src} ${bullet3_src} ${sqlite_src} ${xml_src} ${zlib_src} ${glew_src}               )
add_executable(editor          ${dokun_src} ${editor_src} ${lua_src} ${png_src} ${gif_src} ${jpeg_src} ${tiff_src} ${ogg_src} ${vorbis_src} ${theora_src} ${freetype_src} ${box2d_src} ${bullet3_src} ${sqlite_src} ${xml_src} ${zlib_src} ${glew_src} )
#############
# set properties
set_target_properties(dokun    PROPERTIES COMPILE_DEFINITIONS "DOKUN_BUILD_CONSOLE")
set_target_properties(libdokun PROPERTIES COMPILE_DEFINITIONS "DOKUN_BUILD_LIBRARY")
set_target_properties(libdokun PROPERTIES PREFIX ""                                ) # remove extra 'lib' prefix
set_target_properties(editor   PROPERTIES COMPILE_DEFINITIONS "DOKUN_BUILD_LIBRARY")
#############
#############
# on a Windows platform (tested with MinGW, g++ 5.3.0)
if(WIN32)
	find_package(OPENGL)
	if(OPENGL_FOUND) 
	    target_link_libraries(dokun  ${opengl_src})
	    target_link_libraries(editor ${opengl_src})
	endif()
	#find_package(Vulkan) # optional
	if(Vulkan_FOUND)
	    target_link_libraries(dokun  ${vulkan_src})
	    target_link_libraries(editor ${vulkan_src})
	endif()
	target_link_libraries(dokun  ${winsocket_src})
	target_link_libraries(dokun  ${openal_src}   )	
	target_link_libraries(editor ${winsocket_src})
	target_link_libraries(editor ${openal_src}   )
endif(WIN32)
#############
# on a Linux platform (tested on Ubuntu with g++-4.8)
if(LINUX)
    find_package(X11)
    if(X11_FOUND)
	    target_link_libraries(dokun  ${x11_src})
	    target_link_libraries(editor ${x11_src})
    endif()
	find_package(OpenGL)
    if(OPENGL_FOUND)
	    target_link_libraries(dokun  ${opengl_src})	
	    target_link_libraries(editor ${opengl_src})
    endif()
	#find_package(Vulkan) # optional
    if(Vulkan_FOUND)
	    target_link_libraries(dokun  ${vulkan_src})
	    target_link_libraries(editor ${vulkan_src})
    endif()
    find_package(OpenAL REQUIRED)
	if(OPENAL_FOUND)
		target_link_libraries(dokun  ${openal_src})
		target_link_libraries(editor ${openal_src})
	endif()
    #find_package(OpenSSL)
    #if(OPENSSL_FOUND)
    #	target_link_libraries(dokun  ${openssl_src})
    #	target_link_libraries(editor ${openssl_src})
    #endif()
	target_link_libraries(dokun  ${posix_src})
	target_link_libraries(editor ${posix_src})
endif(LINUX)
#############
if(MACOSX)
endif (MACOSX)
#############
if(ANDROID)
endif(ANDROID)
#############
