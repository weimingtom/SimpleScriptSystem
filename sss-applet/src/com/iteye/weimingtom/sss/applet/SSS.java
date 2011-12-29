package com.iteye.weimingtom.sss.applet;

public class SSS {
    public static void resize(int w, int h) {
	SSSMainframe.mainframeResize(w, h);
    }
    
    public static void setTitle(String title) {
	SSSMainframe.mainframeSetTitle(title);
    }
    
    public static void refresh() {
	SSSMainframe.mainfreamRefresh();
    }
    
    public static int getMsg() {
	return SSSMainframe.mainframeGetMsg();
    }
    
    //-----------------------------
    
    public static int getMouseStatus() {
	return SSSMouse.MouseGetMouseStatus();
    }
    
    //-----------------------------
    
    public static int getKeyboardStatus() {
	return SSSKeyboard.KeyboardGetKeyboardStatus();
    }
    
    //-----------------------------
    
    public static void trace(String str) {
	SSSMisc.MiscTrace(str);
    }
    
    //-----------------------------
    
    public static void setPixel(int x, int y, int color) {
	SSSCanvas.canvasSetPixel(x, y, color);
    }
    
    public static int getPixel(int x, int y) {
	return SSSCanvas.canvasGetPixel(x, y);
    }
    
    public static void setColor(int color) {
	SSSCanvas.canvasSetColor(color);
    }
    
    public static void moveTo(int x, int y) {
	SSSCanvas.canvasMoveTo(x, y);
    }
    
    public static void lineTo(int x, int y) {
	SSSCanvas.canvasLineTo(x, y);
    }
    
    public static void drawLine(int x1, int y1, int x2, int y2, int color) {
	SSSCanvas.canvasDrawLine(x1, y1, x2, y2, color);
    }
    
    public static void lock() {
	SSSCanvas.canvasLock();
    }
    
    public static void unlock() {
	SSSCanvas.canvasUnlock();
    }
    
    public static int rgb(int r, int g, int b) {
	return SSSCanvas.canvasRGB(r, g, b);
    }
}
