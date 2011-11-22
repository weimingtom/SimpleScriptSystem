#ifndef __egl_h_
#define __egl_h_

/*
** Copyright 2002-2003 Promoters of the Khronos Group (3Dlabs, ARM Ltd.,
** ATI Technologies, Inc., Discreet, Ericsson Mobile, Imagination
** Technologies Group plc, Motorola, Inc., Nokia, Silicon Graphics, Inc.,
** SK Telecom, and Sun Microsystems).
**
** This document is protected by copyright, and contains information
** proprietary to The Khronos Group. Any copying, adaptation, distribution,
** public performance, or public display of this document without the
** express written consent of the copyright holders is strictly prohibited.
** The receipt or possession of this document does not convey any rights to
** reproduce, disclose, or distribute its contents, or to manufacture, use,
** or sell anything that it may describe, in whole or in part.
*/

/*------------------------------------------------------------------------
 *
 * Gerbera EGL 1.1 API
 * -------------------
 *
 * (C) 2001-2004 Hybrid Graphics, Ltd.
 * All Rights Reserved.
 *
 * This file consists of source code released by Hybrid 
 * Graphics under the terms of the accompanying End User License 
 * Agreement (EULA). Please do not use this program/source code 
 * before you have read the EULA and have agreed to be bound by 
 * its terms.
 * 
 * Use and/or disclosure outside these terms may result in 
 * irrepairable harm to Hybrid Graphics and legal action against 
 * the party in breach.
 *
 * The most up-to-date version of the EGL 1.1 specification
 * can be found from http://www.khronos.org/opengles/spec.html
 *
 * $Id: //depot/branches/gerbera/2.0.3/api_opengl/interface/GLES/egl.h#2 $
 * $Date: 2004/09/23 $
 * $Author: sampo $ *
 *
 *//**
 * \file
 * \brief   Gerbera EGL interface header.
 * \author  petri@hybrid.fi
 *//*-------------------------------------------------------------------*/

#if !defined(__gl_h_)
#   include "GLES/gl.h"
#endif

/*------------------------------------------------------------------------
 * Platform-independent types.
 *----------------------------------------------------------------------*/

typedef int             EGLint;
typedef unsigned int    EGLBoolean;
typedef void*           EGLConfig;
typedef void*           EGLContext;
typedef void*           EGLDisplay;
typedef void*           EGLSurface;

#if !defined(GL_APICALL)
#   error GL_APICALL not defined.
#endif
#define EGL_APICALL GL_APICALL

typedef EGLint          NativeDisplayType;
typedef void*           NativeWindowType;   /* HWND on win32        */
typedef void*           NativePixmapType;   /* HBITMAP on win32     */

#define EGL_DEFAULT_DISPLAY ((NativeDisplayType)0)
#define EGL_NO_CONTEXT      ((EGLContext)0)
#define EGL_NO_DISPLAY      ((EGLDisplay)0)
#define EGL_NO_SURFACE      ((EGLSurface)0)

/*------------------------------------------------------------------------
 * EGL Enumerants.
 *----------------------------------------------------------------------*/

/* Versioning and extensions */
#define EGL_VERSION_1_0                1
#define EGL_VERSION_1_1                1

/* Boolean */
#define EGL_FALSE                      0
#define EGL_TRUE                       1

/* Errors */
#define EGL_SUCCESS                    0x3000
#define EGL_NOT_INITIALIZED            0x3001
#define EGL_BAD_ACCESS                 0x3002
#define EGL_BAD_ALLOC                  0x3003
#define EGL_BAD_ATTRIBUTE              0x3004
#define EGL_BAD_CONFIG                 0x3005
#define EGL_BAD_CONTEXT                0x3006
#define EGL_BAD_CURRENT_SURFACE        0x3007
#define EGL_BAD_DISPLAY                0x3008
#define EGL_BAD_MATCH                  0x3009
#define EGL_BAD_NATIVE_PIXMAP          0x300A
#define EGL_BAD_NATIVE_WINDOW          0x300B
#define EGL_BAD_PARAMETER              0x300C
#define EGL_BAD_SURFACE                0x300D
#define EGL_CONTEXT_LOST               0x300E
/* 0x300F - 0x301F reserved for additional errors. */

/* Config attributes */
#define EGL_BUFFER_SIZE                0x3020
#define EGL_ALPHA_SIZE                 0x3021
#define EGL_BLUE_SIZE                  0x3022
#define EGL_GREEN_SIZE                 0x3023
#define EGL_RED_SIZE                   0x3024
#define EGL_DEPTH_SIZE                 0x3025
#define EGL_STENCIL_SIZE               0x3026
#define EGL_CONFIG_CAVEAT              0x3027
#define EGL_CONFIG_ID                  0x3028
#define EGL_LEVEL                      0x3029
#define EGL_MAX_PBUFFER_HEIGHT         0x302A
#define EGL_MAX_PBUFFER_PIXELS         0x302B
#define EGL_MAX_PBUFFER_WIDTH          0x302C
#define EGL_NATIVE_RENDERABLE          0x302D
#define EGL_NATIVE_VISUAL_ID           0x302E
#define EGL_NATIVE_VISUAL_TYPE         0x302F
/*#define EGL_PRESERVED_RESOURCES        0x3030*/
#define EGL_SAMPLES                    0x3031
#define EGL_SAMPLE_BUFFERS             0x3032
#define EGL_SURFACE_TYPE               0x3033
#define EGL_TRANSPARENT_TYPE           0x3034
#define EGL_TRANSPARENT_BLUE_VALUE     0x3035
#define EGL_TRANSPARENT_GREEN_VALUE    0x3036
#define EGL_TRANSPARENT_RED_VALUE      0x3037
#if defined(GERBERA_ES_1_1)
#   define EGL_BIND_TO_TEXTURE_RGB        0x3039
#   define EGL_BIND_TO_TEXTURE_RGBA       0x303A
#   define EGL_MAX_SWAP_INTERVAL          0x303B
#   define EGL_MIN_SWAP_INTERVAL          0x303C
#endif /* GERBERA_ES_1_1 */

/* Config attribute and value */
#define EGL_NONE                       0x3038
/* 0x303D - 0x304F reserved for additional config attributes. */

/* Config values */
#define EGL_DONT_CARE                  ((EGLint) -1)
#define EGL_PBUFFER_BIT                0x01
#define EGL_PIXMAP_BIT                 0x02
#define EGL_WINDOW_BIT                 0x04
#define EGL_SLOW_CONFIG                0x3050
#define EGL_NON_CONFORMANT_CONFIG      0x3051
#define EGL_TRANSPARENT_RGB            0x3052
#if defined(GERBERA_ES_1_1)
#   define EGL_NO_TEXTURE                 0x305C
#   define EGL_TEXTURE_RGB                0x305D
#   define EGL_TEXTURE_RGBA               0x305E
#   define EGL_TEXTURE_2D                 0x305F
#endif /* GERBERA_ES_1_1 */

/* String names */
#define EGL_VENDOR                     0x3053
#define EGL_VERSION                    0x3054
#define EGL_EXTENSIONS                 0x3055

/* Surface attributes */
#define EGL_HEIGHT                     0x3056
#define EGL_WIDTH                      0x3057
#define EGL_LARGEST_PBUFFER            0x3058
#if defined(GERBERA_ES_1_1)
#   define EGL_TEXTURE_FORMAT             0x3080
#   define EGL_TEXTURE_TARGET             0x3081
#   define EGL_MIPMAP_TEXTURE             0x3082
#   define EGL_MIPMAP_LEVEL               0x3083

    /* BindTexImage/ReleaseTexImage buffer target */
#   define EGL_BACK_BUFFER                0x3084
#endif /* GERBERA_ES_1_1 */

/* Current surfaces */
#define EGL_DRAW                       0x3059
#define EGL_READ                       0x305A

/* Engines */
#define EGL_CORE_NATIVE_ENGINE         0x305B

/* 0x305C-0x3FFFF reserved for future use */

/*------------------------------------------------------------------------
 * EGL Functions.
 *----------------------------------------------------------------------*/

#if defined (__cplusplus)
extern "C" {
#endif

EGL_APICALL EGLint      eglGetError             (void);

EGL_APICALL EGLDisplay  eglGetDisplay           (NativeDisplayType displayID);
EGL_APICALL EGLBoolean  eglInitialize           (EGLDisplay dpy, EGLint* major, EGLint* minor);
EGL_APICALL EGLBoolean  eglTerminate            (EGLDisplay dpy);

EGL_APICALL const char* eglQueryString          (EGLDisplay dpy, EGLint name);

EGL_APICALL EGLBoolean  eglGetConfigs           (EGLDisplay dpy, EGLConfig* configs, EGLint config_size, EGLint* num_config);
EGL_APICALL EGLBoolean  eglChooseConfig         (EGLDisplay dpy, const EGLint* attrib_list, EGLConfig* configs, EGLint config_size, EGLint* num_config);
EGL_APICALL EGLBoolean  eglGetConfigAttrib      (EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint* value);

EGL_APICALL EGLSurface  eglCreateWindowSurface  (EGLDisplay dpy, EGLConfig config, NativeWindowType win, const EGLint* attrib_list);
EGL_APICALL EGLSurface  eglCreatePbufferSurface (EGLDisplay dpy, EGLConfig config, const EGLint* attrib_list);
EGL_APICALL EGLSurface  eglCreatePixmapSurface  (EGLDisplay dpy, EGLConfig config, NativePixmapType pixmap, const EGLint* attrib_list);
EGL_APICALL EGLBoolean  eglDestroySurface       (EGLDisplay dpy, EGLSurface surface);
EGL_APICALL EGLBoolean  eglQuerySurface         (EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint* value);

#if defined(GERBERA_ES_1_1)
    /* EGL 1.1 render-to-texture APIs */
    EGL_APICALL EGLBoolean eglSurfaceAttrib         (EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value);
    EGL_APICALL EGLBoolean eglBindTexImage          (EGLDisplay dpy, EGLSurface surface, EGLint buffer);
    EGL_APICALL EGLBoolean eglReleaseTexImage       (EGLDisplay dpy, EGLSurface surface, EGLint buffer);

    /* EGL 1.1 swap control API */
    EGL_APICALL EGLBoolean  eglSwapInterval         (EGLDisplay dpy, EGLint interval);
#endif /* GERBERA_ES_1_1 */

EGL_APICALL EGLContext  eglCreateContext        (EGLDisplay dpy, EGLConfig config, EGLContext share_list, const EGLint* attrib_list);
EGL_APICALL EGLBoolean  eglDestroyContext       (EGLDisplay dpy, EGLContext ctx);
EGL_APICALL EGLBoolean  eglMakeCurrent          (EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx);

EGL_APICALL EGLContext  eglGetCurrentContext    (void);
EGL_APICALL EGLSurface  eglGetCurrentSurface    (EGLint readdraw);
EGL_APICALL EGLDisplay  eglGetCurrentDisplay    (void);
EGL_APICALL EGLBoolean  eglQueryContext         (EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint* value);

EGL_APICALL EGLBoolean  eglWaitGL               (void);
EGL_APICALL EGLBoolean  eglWaitNative           (EGLint engine);
EGL_APICALL EGLBoolean  eglSwapBuffers          (EGLDisplay dpy, EGLSurface surface);
EGL_APICALL EGLBoolean  eglCopyBuffers          (EGLDisplay dpy, EGLSurface surface, NativePixmapType target);

EGL_APICALL void (*eglGetProcAddress(const char *procname)) (void);

#if defined (__cplusplus)
}
#endif

/*----------------------------------------------------------------------*/
#endif /* __egl_h_ */
