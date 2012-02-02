package com.iteye.weimingtom.sss.applet;

public class SSSMouse {
	private static final int PEN_DOWN = 0xC0;
	private static final int PEN_MOVE = 0x90;
	private static final int PEN_UP = 0x20;
	private static final int PEN_LEAVE = 0x8;

	private static int status; // Is dragging
	private static int[] buffer = new int[SSSConfig.BUFSIZE];
	private static int head; // Reading position
	private static int tail; // Writing position
	private static boolean reading; // Is reading
	private static boolean writing; // Is writing

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
		status = 0;
		writing = false;
		reading = false;
		head = 0;
		tail = 0;
	}

	public static void release() {
		head = 0;
		tail = 1;
		buffer[0] = -1;
	}

	public static void move(int X, int Y) {
		if (status == 1 && getBufNum(head, tail) < SSSConfig.BUFSIZE - 1) {
			writing = true;
			buffer[tail] = ((X & 0xFFF) << 12) | (Y & 0xFFF) | (PEN_MOVE << 24);
			if (tail == SSSConfig.BUFSIZE - 1) {
				tail = 0;
			} else {
				tail++;
			}
			writing = false;
		}
	}

	public static void lButtonDown(int X, int Y) {
		if (getBufNum(head, tail) < SSSConfig.BUFSIZE - 1) {
			status = 1;
			writing = true;
			buffer[tail] = ((X & 0xFFF) << 12) | (Y & 0xFFF) | (PEN_DOWN << 24);
			if (tail == SSSConfig.BUFSIZE - 1) {
				tail = 0;
			} else {
				tail++;
			}
			writing = false;
		}
	}

	public static void lButtonUp(int X, int Y) {
		if (status == 1) {
			writing = true;
			buffer[tail] = ((X & 0xFFF) << 12) | (Y & 0xFFF) | (PEN_UP << 24);
			if (tail == SSSConfig.BUFSIZE - 1) {
				tail = 0;
			} else {
				tail++;
			}
			writing = false;
			status = 0;
		}
	}

	public static int MouseGetMouseStatus() {
		int status;
		if (getBufNum(head, tail) > 0) {
			reading = true;
			status = buffer[head];
			if (head == SSSConfig.BUFSIZE - 1) {
				head = 0;
			} else {
				head++;
			}
			reading = false;
			return status;
		}
		return 0;
	}
}
