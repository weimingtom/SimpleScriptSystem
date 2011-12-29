package com.iteye.weimingtom.sss.applet;

import java.applet.Applet;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Image;

public class SSSCanvas {
    private static Graphics bufGraph;
    private static Image bufImage;
    private static int currentX;
    private static int currentY;
        
    public static void init(Applet applet) {
	if (bufGraph == null) {
	    bufImage = applet.createImage(SSSConfig.WINDOW_WIDTH, SSSConfig.WINDOW_HEIGHT);
	    if (bufImage != null) {
		bufGraph = bufImage.getGraphics();
	    }
	}
	if (bufGraph != null) {
	    bufGraph.clearRect(0, 0, SSSConfig.WINDOW_WIDTH, SSSConfig.WINDOW_HEIGHT);
	}
    }
    
    public static Image getImage() {
	return bufImage;
    }
    
    public static void canvasSetPixel(int x, int y, int color) {
	canvasDrawLine(x, y, x, y, color);
    }
    
    public static int canvasGetPixel(int x, int y) {
	//not supported
	return 0;
    }
    
    public static void canvasSetColor(int color) {
	if (bufGraph != null) {
	    bufGraph.setColor(new Color(color));
	}
    }
    
    public static void canvasMoveTo(int x, int y) {
	currentX = x;
	currentY = y;
    }
    
    public static void canvasLineTo(int x, int y) {
	if (bufGraph != null) {
	    bufGraph.drawLine(x, y, currentX, currentY);
	}
    }
    
    public static void canvasDrawLine(int x1, int y1, int x2, int y2, int color) {
	if (bufGraph != null) {
	    bufGraph.setColor(new Color(color));
	    bufGraph.drawLine(x1, y1, x2, y2);
	}
    }
    
    public static void canvasLock() {
	
    }
    
    public static void canvasUnlock() {
	
    }
    
    public static int canvasRGB(int r, int g, int b) {
	return new Color(r, g, b).getRGB();
    }
}
