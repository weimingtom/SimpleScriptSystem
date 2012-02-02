package com.iteye.weimingtom.sss.applet;

import java.util.Date;

public class SSSTimer {
	public static int getSeconds() {
		return new Date().getSeconds();
	}
	
	public static int getMinutes() {
		return new Date().getMinutes();
	}
	
	public static int getHours() {
		return new Date().getHours();
	}
}
