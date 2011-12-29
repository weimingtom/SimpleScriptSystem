package com.iteye.weimingtom.sss.applet;

public class SSSKeyboard {
    private static int status;
    private static int[] buffer = new int[SSSConfig.BUFSIZE];
    private static int head;
    private static int tail;
    private static boolean reading;
    private static boolean writing;
    
    private static int getBufNum(int head, int tail) {
    	if (tail > head) {
    	    return tail - head;
    	} else if (tail < head) {
    	    return SSSConfig.BUFSIZE - head + tail;
    	} else {
    	    return 0;
    	}
    }
    
    public static void init() {
        head = 0;
        tail = 0;
    }
    
    public static void release() {
        head = 0;
        tail = 1;
        buffer[0] = -1;
    }
    
    public static void press(int Key) {
    	if (getBufNum(head, tail) < SSSConfig.BUFSIZE - 1) {
    	    int s = 0;
    	    writing = true;
    	    buffer[tail] = ((int)Key) | (s << 8);
    	    if (tail == SSSConfig.BUFSIZE - 1) {
    		tail = 0;
    	    } else {
    		tail++;
    	    }
    	    writing = false;
    	}
    }
    
    public static int KeyboardGetKeyboardStatus() {
        int status;
        if (getBufNum(head, tail) > 0) {
            reading = true;
            status = buffer[head];
            if (head == SSSConfig.BUFSIZE - 1) {
    		head = 0;
            }else {
    		head++;
            }
            reading = false;
            return status;
        }
        return 0;
    }
}
