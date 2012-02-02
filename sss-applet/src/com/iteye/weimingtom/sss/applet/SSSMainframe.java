package com.iteye.weimingtom.sss.applet;

import java.applet.Applet;
import java.awt.Dimension;
import java.awt.Frame;
import java.awt.Graphics;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

public class SSSMainframe extends Applet implements Runnable, KeyListener,
		MouseListener, MouseMotionListener {
	private static final long serialVersionUID = -6776404354323666713L;

	// for test
	private static final boolean ENABLE_SCRIPT = false;
	private static final boolean ENABLE_TESTCONTEXT = true;
	
	private static Frame frame; // AWT window
	private static SSSMainframe mainframe; // Applet window

	// TODO: prevent stopping applet's thread
	private Thread drawThread;
	private static boolean isStopped = false;

	/**
	 * must be public, used by Applet.
	 */
	public SSSMainframe() {

	}

	/**
	 * AWT start here
	 * 
	 * @param args
	 */
	public static final void main(String[] args) {
		frame = new Frame();
		mainframe = new SSSMainframe();
		frame.add(mainframe);

		SSSMainframe.mainframeResize(SSSConfig.WINDOW_WIDTH,
				SSSConfig.WINDOW_HEIGHT);

		frame.addWindowListener(new WindowAdapter() {
			@Override
			public void windowClosing(WindowEvent e) {
				mainframe.stop();
				mainframe.releaseAll();
				System.exit(0);
			}
		});

		frame.setVisible(true);
		mainframe.init();
		mainframe.start();
	}

	/**
	 * Applet start here. the size is specified by environment.
	 */
	@Override
	public void init() {
		if (mainframe == null) {
			mainframe = this;
		}
		SSSCanvas.init(this);
		SSSKeyboard.init();
		SSSMouse.init();
		mainframeSetTitle(SSSConfig.WINDOW_TITLE);
		this.addMouseListener(this);
		this.addKeyListener(this);
		this.addMouseMotionListener(this);
	}

	@Override
	public void start() {
		drawThread = new Thread(this);
		drawThread.start();
	}

	@Override
	public void run() {
		if (ENABLE_SCRIPT) {
			SSSScript.ScriptRun();
		} else if (ENABLE_TESTCONTEXT) {
			SSSContext2D.test2();
		} else {
			int width = 800;
			int height = 600;
			SSSMainframe.mainframeResize(width, height);
			while (true) {
				if (SSSMainframe.mainframeGetMsg() != 0) {
					break;
				}
				SSSCanvas.canvasLock();
				SSSCanvas.canvasDrawLine(0, 0, width, height, 0xff0000);
				SSSCanvas.canvasUnlock();
				SSSMainframe.mainfreamRefresh();
			}
		}
		releaseAll();
		System.exit(0);
	}
	
	@Override
	public void stop() {
		isStopped = true;
	}

	public void releaseAll() {
		SSSKeyboard.release();
		SSSMouse.release();
	}

	@Override
	public void update(Graphics g) {
		paint(g);
	}

	@Override
	public void paint(Graphics g) {
		//System.out.println("paint!");
		// g.clearRect(0, 0, SSSConfig.WINDOW_WIDTH, SSSConfig.WINDOW_HEIGHT);
		g.drawImage(SSSCanvas.getImage(), 0, 0, this);
	}

	@Override
	public void keyPressed(KeyEvent e) {
		SSSKeyboard.press(e.getKeyCode());
	}

	@Override
	public void keyReleased(KeyEvent e) {

	}

	@Override
	public void keyTyped(KeyEvent e) {

	}

	@Override
	public void mouseClicked(MouseEvent e) {

	}

	@Override
	public void mouseEntered(MouseEvent e) {

	}

	@Override
	public void mouseExited(MouseEvent e) {

	}

	@Override
	public void mousePressed(MouseEvent e) {
		SSSMouse.lButtonDown(e.getX(), e.getY());
	}

	@Override
	public void mouseReleased(MouseEvent e) {
		SSSMouse.lButtonUp(e.getX(), e.getY());
	}

	@Override
	public void mouseDragged(MouseEvent e) {
		// SSSMouse.lButtonDown(e.getX(), e.getY());
		SSSMouse.move(e.getX(), e.getY());
	}

	@Override
	public void mouseMoved(MouseEvent e) {
		// SSSMouse.move(e.getX(), e.getY());
	}

	// -----------------------------------------

	public static int mainframeGetMsg() {
		if (isStopped) {
			return 1;
		} else {
			return 0;
		}
	}

	public static void mainframeResize(int w, int h) {
		// Is AWT window ?
		if (frame != null) {
			mainframe.setPreferredSize(new Dimension(w, h));
			frame.pack();
			frame.setResizable(false);
			frame.setLocationRelativeTo(null);
		}
	}

	public static void mainfreamRefresh() {
		mainframe.repaint();
		try {
			Thread.sleep(10);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	public static void mainframeSetTitle(String title) {
		if (frame != null) {
			frame.setTitle(title);
		}
	}
}
