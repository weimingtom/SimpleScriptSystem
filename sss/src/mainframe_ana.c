/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

//BEGIN_INCLUDE(all)
#include "config.h"

#if USE_ANA

#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#include <Python.h>
#include <math.h>

/*
my header files
*/
#include "mainframe.h"
#include "script.h"
#include "mouse.h"
#include "keyboard.h"
#include "canvas.h"
#include "misc.h"

/*
my global variable
*/
static struct android_app *g_state;
static struct engine g_engine;

/**
 * Initialize an EGL context for the current display.
 */
static int engine_init_display(struct engine* engine) {
    // initialize OpenGL ES and EGL

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };
    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
    context = eglCreateContext(display, config, NULL, NULL);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOGW("Unable to eglMakeCurrent");
        return -1;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    engine->display = display;
    engine->context = context;
    engine->surface = surface;
    engine->width = w;
    engine->height = h;
    engine->state.angle = 0;

    // Initialize GL state.
    /*
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_DEPTH_TEST);
	*/
    // onSurfaceCreated
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	
    glShadeModel(GL_FLAT);	// 
    glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);	//     
    glEnableClientState(GL_VERTEX_ARRAY);	//set for  array as vertex 
    glEnableClientState(GL_COLOR_ARRAY);	//set for  array as color 
    glDisable(GL_TEXTURE_2D);	//no 2D texture binded, so we disable it for fast speed 
    glDisableClientState(GL_NORMAL_ARRAY);
    
    LOGI("Width = %d, Height = %d\n", engine->width, engine->height);
	glViewport(0, 0, engine->width, engine->height);
	glMatrixMode(GL_PROJECTION);	
	glLoadIdentity();
	//http://www.garagegames.com/community/forums/viewthread/60046
	//see http://www.talisman.org/opengl-1.1/Reference.html
	//glOrtho(left, right, bottom, top, -1.0, 1.0).	
	//gluOrtho2D(gl, 0.0f, 320, 0.0f, 240);
    glOrthof(0.0f, engine->width, engine->height, 0.0f, -1.0, 1.0);
    
    return 0;
}

/**
 * Tear down the EGL context currently associated with the display.
 */
static void engine_term_display(struct engine* engine) {
    if (engine->display != EGL_NO_DISPLAY) {
        eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (engine->context != EGL_NO_CONTEXT) {
            eglDestroyContext(engine->display, engine->context);
        }
        if (engine->surface != EGL_NO_SURFACE) {
            eglDestroySurface(engine->display, engine->surface);
        }
        eglTerminate(engine->display);
    }
    engine->animating = 0;
    engine->display = EGL_NO_DISPLAY;
    engine->context = EGL_NO_CONTEXT;
    engine->surface = EGL_NO_SURFACE;
}

/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
    struct engine* engine = (struct engine*)app->userData;
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
#if 0
        engine->animating = 1;
        engine->state.x = AMotionEvent_getX(event, 0);
        engine->state.y = AMotionEvent_getY(event, 0);
#endif
		if (AKeyEvent_getAction(event) == AMOTION_EVENT_ACTION_DOWN)
		{
			MouseLButtonDown(AMotionEvent_getX(event, 0), AMotionEvent_getY(event, 0));
		}
		else if (AKeyEvent_getAction(event) == AMOTION_EVENT_ACTION_UP)
		{
			MouseLButtonUp(AMotionEvent_getX(event, 0), AMotionEvent_getY(event, 0));
		}
		else if (AKeyEvent_getAction(event) == AMOTION_EVENT_ACTION_MOVE)
		{
			MouseMove(AMotionEvent_getX(event, 0), AMotionEvent_getY(event, 0));
		}
        return 1;
    } else if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY){
		if (AKeyEvent_getAction(event) == AKEY_EVENT_ACTION_DOWN)
		{
			KeyboardChar(AKeyEvent_getKeyCode(event));
		}
		else if (AKeyEvent_getAction(event) == AKEY_EVENT_ACTION_UP)
		{
			//
		}
		/*
		Return 0 to avoid BACK key or other system key being ignored.
		*/
        return 0;		
    }
    return 0;
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
    struct engine* engine = (struct engine*)app->userData;
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
        	LOGI("engine_handle_cmd:%s", "APP_CMD_SAVE_STATE");
        	// The system has asked us to save our current state.  Do so.
            engine->app->savedState = malloc(sizeof(struct saved_state));
            *((struct saved_state*)engine->app->savedState) = engine->state;
            engine->app->savedStateSize = sizeof(struct saved_state);
            break;
        case APP_CMD_INIT_WINDOW:
        	LOGI("engine_handle_cmd:%s", "APP_CMD_INIT_WINDOW");
            // The window is being shown, get it ready.
            if (engine->app->window != NULL) {
                engine_init_display(engine);
                /*
					engine_draw_frame(engine);
				*/
            }
            engine->animating = 1;
            break;
        case APP_CMD_TERM_WINDOW:
        	LOGI("engine_handle_cmd:%s", "APP_CMD_TERM_WINDOW");
        	// The window is being hidden or closed, clean it up.
            engine_term_display(engine);
            engine->animating = 0;
            break;
        case APP_CMD_GAINED_FOCUS:
        	LOGI("engine_handle_cmd:%s", "APP_CMD_GAINED_FOCUS");
        	// When our app gains focus, we start monitoring the accelerometer.
            if (engine->accelerometerSensor != NULL) {
                ASensorEventQueue_enableSensor(engine->sensorEventQueue,
                        engine->accelerometerSensor);
                // We'd like to get 60 events per second (in us).
                ASensorEventQueue_setEventRate(engine->sensorEventQueue,
                        engine->accelerometerSensor, (1000L/60)*1000);
            }
            engine->animating = 1;
            break;
        case APP_CMD_LOST_FOCUS:
        	LOGI("engine_handle_cmd:%s", "APP_CMD_LOST_FOCUS");
            // When our app loses focus, we stop monitoring the accelerometer.
            // This is to avoid consuming battery while not being used.
            if (engine->accelerometerSensor != NULL) {
                ASensorEventQueue_disableSensor(engine->sensorEventQueue,
                        engine->accelerometerSensor);
            }
            // Also stop animating.
            engine->animating = 0;
            /*
				engine_draw_frame(engine);
			*/
            break;
        case APP_CMD_PAUSE:
			LOGI("engine_handle_cmd:%s", "APP_CMD_PAUSE");
			engine->animating = 0;
			break;
		case APP_CMD_CONFIG_CHANGED:
			LOGI("engine_handle_cmd:%s", "APP_CMD_CONFIG_CHANGED");
			engine->animating = 1;
			break;
    }
}

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* state) {
    //struct engine engine;
	//struct engine g_engine;
	
    // Make sure glue isn't stripped.
    app_dummy();

    memset(&g_engine, 0, sizeof(g_engine));
    state->userData = &g_engine;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
    g_engine.app = state;
	
	g_state = state;
	
    // Prepare to monitor accelerometer
    g_engine.sensorManager = ASensorManager_getInstance();
    g_engine.accelerometerSensor = ASensorManager_getDefaultSensor(g_engine.sensorManager,
            ASENSOR_TYPE_ACCELEROMETER);
    g_engine.sensorEventQueue = ASensorManager_createEventQueue(g_engine.sensorManager,
            state->looper, LOOPER_ID_USER, NULL, NULL);

    if (state->savedState != NULL) {
        // We are starting with a previous saved state; restore from it.
        g_engine.state = *(struct saved_state*)state->savedState;
    }

    CanvasInit(g_state, &g_engine);
	MouseInit();
	KeyboardInit();
#if 0
	while (1) 
	{
		if (MainFrameGetMsg())
		{
			break;
		}
		CanvasLock();
		CanvasUnlock();
        MainFrameRefresh();
	}
#else
	ScriptRun();
#endif
}
//END_INCLUDE(all)

int MainFrameGetMsg(void) 
{
    int ident;
    int events;
    struct android_poll_source* source;
    
	/*
int ALooper_pollAll(int timeoutMillis, int* outFd, int* outEvents, void** outData);
* Like ALooper_pollOnce(), but performs all pending callbacks until all
* data has been consumed or a file descriptor is available with no callback.
* This function will never return ALOOPER_POLL_CALLBACK.
		
		10 ms delay
		
	*/
	// If not animating, we will block forever waiting for events.
	// If animating, we loop until all events are read, then continue
	// to draw the next frame of animation.
	while ((ident=ALooper_pollAll(0/*engine.animating ? 0 : -1*/, NULL, &events,
			(void**)&source)) >= 0) 
	{
		// Process this event.
		if (source != NULL) {
			source->process(g_state, source);
		}
		// If a sensor has data, process it now.
		if (ident == LOOPER_ID_USER) {
			if (g_engine.accelerometerSensor != NULL) {
				ASensorEvent event;
				while (ASensorEventQueue_getEvents(g_engine.sensorEventQueue,
						&event, 1) > 0) {
					LOGI("accelerometer: x=%f y=%f z=%f",
							event.acceleration.x, event.acceleration.y,
							event.acceleration.z);
				}
			}
		}
		// Check if we are exiting.
		if (g_state->destroyRequested != 0) {
			engine_term_display(&g_engine);
			return 1;
		}
	}
	return 0;
}

void MainFrameRefresh(void)
{
	/*
	not supported
	*/
}

void MainFrameResize(int w, int h)
{
	/*
	not supported
	*/
}

void MainFrameSetTitle(const char *str)
{
	/*
	not supported
	*/
}

#endif
