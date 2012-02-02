package com.iteye.weimingtom.sss.applet;

//
//NOTE: This code is modified from minijoe
//
//Copyright 2008 Google Inc.
//
//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at
//
//http://www.apache.org/licenses/LICENSE-2.0
//
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS,
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and
//limitations under the License.

import java.util.Hashtable;
import java.util.Vector;

/**
 * @see http://code.google.com/p/minijoe/
 * @see G:\svn\minijoe\minijoe\javascript
 * @author Administrator
 *
 */
public class SSSContext2D {
	
	private static final int ID_BEGIN_PATH = 100;
	private static final int ID_CLOSE_PATH = 101;
	private static final int ID_FILL = 102;
	private static final int ID_FILL_RECT = 103;
	private static final int ID_FILL_STYLE = 104;
	private static final int ID_FILL_STYLE_SET = 105;

	private static final int ID_LINE_TO = 106;
	private static final int ID_MOVE_TO = 107;
	private static final int ID_QUADRATIC_CURVE_TO = 108;

	private static final int ID_RESTORE = 109;
	private static final int ID_ROTATE = 110;
	private static final int ID_SAVE = 111;
	private static final int ID_SCALE = 112;
	private static final int ID_STROKE = 113;
	private static final int ID_STROKE_RECT = 114;
	private static final int ID_TRANSLATE = 115;
	private static final int ID_DRAW_IMAGE = 116;
	private static final int ID_TEXT_STYLE = 117;
	private static final int ID_TEXT_STYLE_SET = 118;
	private static final int ID_MEASURE_TEXT = 119;
	private static final int ID_CLEAR_RECT = 120;
	private static final int ID_ARC = 121;
	private static final int ID_BEZIER_CURVE_TO = 122;
	private static final int ID_STROKE_STYLE = 123;
	private static final int ID_STROKE_STYLE_SET = 124;
	private static final int ID_DRAW_TEXT = 125;
	private static final Hashtable<String, Integer> COLORS = new Hashtable<String, Integer>();
	private static final double[] EXP2 = new double[15];
	private static final double[] EXP3 = new double[15];
	private static final double[] ZT1MT = new double[15];
	private static final double[] DT1MT2 = new double[15];

	static {
		COLORS.put("red", new Integer(0x0ff0000));
		COLORS.put("green", new Integer(0x000ff00));
		COLORS.put("blue", new Integer(0x00000ff));
		COLORS.put("white", new Integer(0x0ffffff));
		COLORS.put("black", new Integer(0));
		COLORS.put("yellow", new Integer(0x0ffff00));
		COLORS.put("grey", new Integer(0x0ff888888));
		double delta = 1.0 / 16.0;
		double pos = delta;
		for (int i = 0; i < 15; i++) {
			double l = pos * pos;
			EXP2[i] = l;
			EXP3[i] = l * pos;
			ZT1MT[i] = ((2.0 * pos * (1 - pos)));
			DT1MT2[i] = ((3.0 * pos * (1 - pos) * (1 - pos)));
			pos += delta;
		}
	}

	private static boolean virgin;
	// private Graphics graphics;
	private static int pathPos = 0;
	private static double[] path = new double[32];
	private static boolean fill;
	private static int scrStartX;
	private static int scrStartY;
	private static int currentScrX;
	private static int currentScrY;
	private static int currentFillColor = 0x0ff000000;
	private static int currentLineColor = 0x0ff000000;
	private static double translateX = 0;
	private static double translateY = 0;
	private static double scaleX = 1;
	private static double scew1 = 0;
	private static double scaleY = 1;
	private static double scew2 = 0;
	private static Vector<double[]> stack = new Vector<double[]>();

	private final static class Edge {
		private int yTop;
		private int xInt;
		private int deltaY;
		private int deltaX;
		private Edge next;
		private Edge reuse;		
	}
	
	private static boolean useAlpha = false;
	private static boolean inFill = false;
	private static int lastColor = 0;
	private static boolean useFillArray;
	private static final int INITIAL_POINTS = 32;
	private static int[] fillArray = new int[256];
	private static int segmentStart;
	private static int pos;
	private static int currentX;
	private static int currentY;
	private static int[] pointsX = new int[INITIAL_POINTS];
	private static int[] pointsY = new int[INITIAL_POINTS];
	private static int[] nextPos = new int[INITIAL_POINTS];
	private static Edge reuseHead = new Edge();
	private static Edge reusePtr;
	private static Edge edges;
	private static int bottomY;
	
	private static String[] split(String s, char c) {
		int count = 1;
		for (int i = 0; i < s.length(); i++) {
			if (s.charAt(i) == c) {
				count++;
			}
		}
		String[] parts = new String[count];
		int lastCut = 0;
		int n = 0;
		for (int i = 0; i < s.length(); i++) {
			if (s.charAt(i) == c) {
				parts[n] = s.substring(lastCut, i);
				lastCut = i + 1;
			}
		}
		parts[n] = s.substring(lastCut, s.length());
		return parts;
	}

	private static int translateX(double x, double y) {
		return (int) (((x * scaleX + y * scew1)) + (translateX));
	}

	private static int translateY(double x, double y) {
		return (int) (((y * scaleY + x * scew2)) + (translateY));
	}

	private static void enlargeBuf(int i) {
		i = (path.length + i) * 3 / 2;

		double[] np = new double[i];
		System.arraycopy(path, 0, np, 0, pathPos);
		path = np;
	}

	private static void transform(double a2, double b2, double c2, double d2, double e2, double f2) {
		double tmp = b2;
		b2 = c2;
		c2 = tmp;
		double a1 = scaleX;
		double b1 = scew1;
		double c1 = scew2;
		double d1 = scaleY;
		double e1 = translateX;
		double f1 = translateY;
		scaleX = a1 * a2 + c1 * b2;
		scew1 = b1 * a2 + d1 * b2;
		scew2 = a1 * c2 + c1 * d2;
		scaleY = b1 * c2 + d1 * d2;
		translateX = (a1 * e2 + c1 * f2) + e1;
		translateY = (b1 * e2 + d1 * f2) + f1;
	}

	private static void drawPath() {
		int i = 0;
		virgin = true;
		while (i < pathPos) {
			int type = (int) path[i++];
			double x = path[i++];
			double y = path[i++];
			switch (type) {
			case ID_MOVE_TO:
				scrMoveTo(translateX(x, y), translateY(x, y));
				break;
				
			case ID_LINE_TO:
				scrLineTo(translateX(x, y), translateY(x, y));
				break;
				
			case ID_BEZIER_CURVE_TO:
				bezierCurveTo0(x, y, path[i], path[i + 1], path[i + 2], path[i + 3]);
				i += 4;
				break;
				
			case ID_QUADRATIC_CURVE_TO:
				quadraticCurveTo0(x, y, path[i], path[i + 1]);
				i += 2;
				break;
				
			case ID_ARC:
				arc0(x, y, path[i], path[i + 1], path[i + 2], path[i + 3] != 0);
				i += 4;
				break;
				
			case ID_CLOSE_PATH:
				scrLineTo(scrStartX, scrStartY);
				break;
			}
		}
	}

	private static void scrMoveTo(int i, int j) {
		currentScrX = scrStartX = i;
		currentScrY = scrStartY = j;
		virgin = false;
		if (fill) {
			moveTo0(i, j);
		}
	}

	private static void scrLineTo(int scrX, int scrY) {
		if (virgin) {
			scrMoveTo(scrX, scrY);
		} else if (fill) {
			lineTo0(scrX, scrY);
		} else {
			//SSS.drawLine(currentScrX, currentScrY, scrX, scrY);
			SSS.moveTo(currentScrX, currentScrY);
			SSS.lineTo(scrX, scrY);
		}
		currentScrX = scrX;
		currentScrY = scrY;
	}

	private static int parseColor(String style) {
		if (style.startsWith("rgba")) {
			int cut0 = style.indexOf('(') + 1;
			int cut1 = style.indexOf(')', cut0);
			String[] rgba = split(style.substring(cut0, cut1), ',');
			return (Integer.parseInt(rgba[0].trim()) << 16)
					| (Integer.parseInt(rgba[1].trim()) << 8)
					| Integer.parseInt(rgba[2].trim())
					| (((int) (Double.parseDouble(rgba[3].trim()) * 255.0) & 0x0ff) << 24);
		} else if (style.startsWith("rgb")) {
			int cut0 = style.indexOf('(') + 1;
			int cut1 = style.indexOf(')', cut0);
			String[] rgba = split(style.substring(cut0, cut1), ',');
			return (Integer.parseInt(rgba[0].trim()) << 16)
					| (Integer.parseInt(rgba[1].trim()) << 8)
					| Integer.parseInt(rgba[2].trim()) | 0x0ff000000;
		} else if (style.startsWith("#")) {
			return Integer.parseInt(style.substring(1), 16) | 0x0ff000000;
		}
		Integer c = (Integer) COLORS.get(style);
		return 0x0ff000000 | (c == null ? 0 : c.intValue());
	}

	private static void bezierCurveTo0(double cp1x, double cp1y, double cp2x, double cp2y, double x, double y) {
		int p0x = currentScrX;
		int p0y = currentScrY;
		int p1x = translateX(cp1x, cp1y);
		int p1y = translateY(cp1x, cp1y);
		int p2x = translateX(cp2x, cp2y);
		int p2y = translateY(cp2x, cp2y);
		int p3x = translateX(x, y);
		int p3y = translateY(x, y);
		int d = Math.max((p0x - p3x) * (p0x - p3x) + (p0y - p3y) * (p0y - p3y),
				Math.max((p0x - p1x) * (p0x - p1x) + (p0y - p1y) * (p0y - p1y),
						(p2x - p3x) * (p2x - p3x) + (p2y - p3y) * (p2y - p3y)));
		int step;
		if (d > 1000) { //
			step = d > 4000 ? 1 : 2;
		} else {
			step = d > 250 ? 4 : 8;
		}
		for (int i = step - 1; i < 15; i += step) {
			int newScrX = (int) (EXP3[14 - i] * p0x + DT1MT2[i] * p1x
					+ DT1MT2[14 - i] * p2x + EXP3[i] * p3x);
			int newScrY = (int) (EXP3[14 - i] * p0y + DT1MT2[i] * p1y
					+ DT1MT2[14 - i] * p2y + EXP3[i] * p3y);
			scrLineTo(newScrX, newScrY);
		}
		scrLineTo(p3x, p3y);
	}
	
	private static void arc0(double x, double y, double radius, double startAngle, double endAngle, boolean counterclockwise) {
		double pi2 = 2.0 * Math.PI;
		double ang = startAngle % pi2;
		double end = endAngle % pi2;
		double delta = (int) (90 * (scaleX + scaleY) / radius);
		if (delta < Math.PI / 180.0) {
			delta = Math.PI / 180.0;
		}
		if (delta > Math.PI / 10) {
			delta = Math.PI / 10;
		}
		if (counterclockwise) {
			delta = -delta;
			if (end >= ang) {
				end -= pi2;
			}
		} else {
			if (end <= ang) {
				end += pi2;
			}
		}
		double cx;
		double cy;
		do {
			cx = x + radius * Math.cos(ang);
			cy = y + radius * Math.sin(ang);
			scrLineTo(translateX(cx, cy), translateY(cx, cy));
			ang += delta;
		} while (counterclockwise ? ang >= end : ang <= end);
		cx = x + radius * Math.cos(end);
		cy = y + radius * Math.sin(end);
		scrLineTo(translateX(cx, cy), translateY(cx, cy));
	}

	private static void quadraticCurveTo0(double cpx, double cpy, double x, double y) {
		int p0x = currentScrX;
		int p0y = currentScrY;
		int p1x = translateX(cpx, cpy);
		int p1y = translateY(cpx, cpy);
		int p2x = translateX(x, y);
		int p2y = translateY(x, y);
		int d = Math.max((p0x - p1x) * (p0x - p1x) + (p0y - p1y) * (p0y - p1y),
				(p1x - p2x) * (p1x - p2x) + (p1y - p2y) * (p1y - p2y));
		int step;
		if (d > 1000) {
			step = d > 4000 ? 1 : 2;
		} else {
			step = d > 250 ? 4 : 8;
		}
		for (int i = step - 1; i < 15; i += step) {
			int newScrX = (int) (EXP2[14 - i] * p0x + ZT1MT[i] * p1x + EXP2[i]
					* p2x);
			int newScrY = (int) (EXP2[14 - i] * p0y + ZT1MT[i] * p1y + EXP2[i]
					* p2y);
			scrLineTo(newScrX, newScrY);
		}
		scrLineTo(p2x, p2y);
	}
		
	private static void strokeRect0(double x0, double y0, double x1, double y1) {
		beginPath();
		moveTo(x0, y0);
		lineTo(x1, y0);
		lineTo(x1, y1);
		lineTo(x0, y1);
		lineTo(x0, y0);
		stroke();
	}
	
	private static void fillRect0(double x0, double y0, double x1, double y1) {
		if (scew1 == 0 && scew2 == 0 && scaleX > 0 && scaleY > 0) {
			int sx0 = translateX(x0, y0);
			int sy0 = translateY(x0, y0);
			int sx1 = translateX(x1, y1);
			int sy1 = translateY(x1, y1);
			//graphics.setColor(currentFillColor);
			//graphics.fillRect(sx0, sy0, sx1 - sx0 + 1, sy1 - sy0 + 1);
			SSS.setColor(currentFillColor);
			SSS.fillRect(sx0, sy0, sx1, sy1);
		} else {
			beginPath();
			moveTo(x0, y0);
			lineTo(x1, y0);
			lineTo(x1, y1);
			lineTo(x0, y1);
			lineTo(x0, y0);
			fill();
		}
	}
	
	//-------------------------------------
	
	private static Edge newEdge() {
		Edge result = reusePtr;
		if (result.reuse == null) {
			result.reuse = new Edge();
		}
		reusePtr = result.reuse;
		return result;
	}

	private static void beginPath0() {
		if (inFill) {
			throw new RuntimeException("Fill Threading issue");
		}
		segmentStart = 0;
		pos = 0;
	}

	private static void lineTo0(int x, int y) {
		if (pos + 4 >= pointsX.length) {
			int l = pointsX.length * 3 / 2;
			int[] temp = new int[l];
			System.arraycopy(pointsX, 0, temp, 0, pos);
			pointsX = temp;
			temp = new int[l];
			System.arraycopy(pointsY, 0, temp, 0, pos);
			pointsY = temp;
			temp = new int[l];
			System.arraycopy(nextPos, 0, temp, 0, pos);
			nextPos = temp;
		}
		if (segmentStart == pos) {
			pointsX[pos] = currentX;
			pointsY[pos] = currentY;
			nextPos[pos] = pos + 1;
			pos++;
		}
		pointsX[pos] = currentX = x;
		pointsY[pos] = currentY = y;
		nextPos[pos] = pos + 1;
		pos++;
	}

	private static void moveTo0(int x, int y) {
		if (pos > segmentStart) {
			nextPos[pos - 1] = segmentStart;
		}
		segmentStart = pos;
		currentX = x;
		currentY = y;
	}

	private static final int nextY0(int k) {
		int compareY = pointsY[k];
		int newY;
		do {
			k = nextPos[k];
			newY = pointsY[k];
		} while (newY == compareY);
		return newY;
	}

	private static final void insertPoly(int p1x, int p1y, int p2x, int p2y,
			int yNext) {
		int dx = ((p2x - p1x) << 16) / (p2y - p1y);
		int x2 = p2x << 16;
		if (p2y > p1y && p2y < yNext) {
			p2y--;
			x2 -= dx;
		} else if (p2y < p1y && p2y > yNext) {
			p2y++;
			x2 += dx;
		}
		int dy = p2y - p1y;
		int maxX;
		int maxY;
		if (dy > 0) {
			maxY = p2y;
			maxX = x2;
			dy++;
		} else {
			maxY = p1y;
			maxX = p1x << 16;
			dy = 1 - dy;
		}
		Edge newEdge = newEdge();
		newEdge.yTop = maxY;
		newEdge.deltaY = dy;
		newEdge.xInt = maxX;
		newEdge.deltaX = dx;
		Edge edge1 = edges;
		while (edge1.next.yTop >= maxY) {
			edge1 = edge1.next;
		}
		newEdge.next = edge1.next;
		edge1.next = newEdge;
	}
	
	private static void fillPoly(int color) {
		int alpha = color >>> 24;
		if (useAlpha ? alpha == 0 : alpha < 64) {
			return;
		}
		inFill = true;
		if (useAlpha ? alpha == 255 : alpha > 192) {
			useFillArray = false;
			//g.setColor(color & 0x0ffffff);
			SSS.setColor(color & 0x0ffffff);
		} else {
			useFillArray = true;
			if (color != lastColor) {
				lastColor = color;
				if (useAlpha) {
					for (int i = 0; i < 255; i++) {
						fillArray[i] = color;
					}
				} else {
					for (int i = 0; i < 255; i += 2) {
						fillArray[i] = color | 0xff000000;
						fillArray[i + 1] = color & 0xff000000;
					}
				}
			}
		}
		if (pos > segmentStart) {
			nextPos[pos - 1] = segmentStart;
		}
		reusePtr = reuseHead;
		edges = newEdge();
		Edge edge1 = newEdge();
		edges.next = edge1;
		edge1.next = null;
		edges.yTop = Integer.MAX_VALUE;
		edge1.yTop = Integer.MIN_VALUE;
		bottomY = Integer.MAX_VALUE;
		for (int k = 0; k < pos; k++) {
			int p1x = pointsX[k];
			int p1y = pointsY[k];
			int next = nextPos[k];
			if (pointsY[next] != p1y) {
				insertPoly(p1x, p1y, pointsX[next], pointsY[next], nextY0(next));
			} else {
				drawHorizontalLine(p1x, p1y, pointsX[next]);
			}
			if (p1y < bottomY) {
				bottomY = p1y;
			}
		}
		Edge lActEdge = edges.next;
		for (int scan = edges.next.yTop; scan >= bottomY; scan--) {
			if (scan > edges.next.yTop) {
				scan = edges.next.yTop;
			}
			while (lActEdge.next.yTop >= scan) {
				lActEdge = lActEdge.next;
			}
			Edge edge2 = edges.next;
			do {
				edge1 = edges;
				while (edge1.next.xInt < edge2.next.xInt) {
					edge1 = edge1.next;
				}
				if (edge1 != edge2) {
					Edge edge3 = edge2.next.next;
					edge2.next.next = edge1.next;
					edge1.next = edge2.next;
					edge2.next = edge3;
					if (edge1.next == lActEdge) {
						lActEdge = edge2;
					}
				} else {
					edge2 = edge2.next;
				}
			} while (edge2 != lActEdge);
			edge1 = edges;
			do {
				edge1 = edge1.next;
				int qx = (edge1.xInt + 32768) >> 16;
				edge1 = edge1.next;
				drawHorizontalLine(qx, scan, (edge1.xInt + 32768) >> 16);
			} while (edge1 != lActEdge);
			edge1 = edges;
			Edge prevEdge = edge1;
			do {
				edge1 = prevEdge.next;
				if (edge1.deltaY > 1) {
					edge1.deltaY--;
					edge1.xInt -= edge1.deltaX;
					prevEdge = edge1;
				} else {
					prevEdge.next = edge1.next;
					if (edge1 == lActEdge) {
						lActEdge = prevEdge;
					}
				}
			} while (prevEdge != lActEdge);
		}
		inFill = false;
	}
	
	private static final void drawHorizontalLine(int p1x, int p1y, int p2x) {
		if (useFillArray) {
			if (p2x < p1x) {
				int swap = p2x;
				p2x = p1x;
				p1x = swap;
			}
			//g.drawRGB(fillArray, (p1x + p1y) & 1, 256, p1x, p1y, p2x - p1x, 1, true);
			//FIXME:
		} else {
			//g.drawLine(p1x, p1y, p2x, p1y);
			SSS.moveTo(p1x, p1y);
			SSS.lineTo(p2x, p1y);
		}
	}
	
	//----------------------------------------

	//clearRect
	public static void clearRect(double x, double y, double w, double h) {
        int sfc = currentFillColor;
        currentFillColor = 0x0ffffffff;
		fillRect0(x, y, x + w - 1, y + h - 1);
		currentFillColor = sfc;
	}
	
	//quadraticCurveTo
	public static void quadraticCurveTo(double x1, double y1, double x2, double y2) {
        if (pathPos + 5 >= path.length) {
            enlargeBuf(5);
          }
          path[pathPos++] = ID_QUADRATIC_CURVE_TO;
          path[pathPos++] = x1;
          path[pathPos++] = y1;
          path[pathPos++] = x2;
          path[pathPos++] = y2;
	}
	
	//bezierCurveTo
	public static void bezierCurveTo(double x1, double y1, double x2, double y2, double x3, double y3) {
        if (pathPos + 7 >= path.length) {
            enlargeBuf(7);
          }
          path[pathPos++] = ID_BEZIER_CURVE_TO;
          path[pathPos++] = x1;
          path[pathPos++] = y1;
          path[pathPos++] = x2;
          path[pathPos++] = y2;
          path[pathPos++] = x3;
          path[pathPos++] = y3;
	}
	
	//rotate
	public static void rotate(double angle) {
        double ang = -angle; 
        double sin = Math.sin(ang);
        double cos = Math.cos(ang);
        transform(cos, -sin, sin, cos, 0, 0);
	}
	
	//scale
	public static void scale(double x, double y) {
		transform(x, 0, 0, y, 0, 0);
	}
	
	//translate
	public static void translate(double x, double y) {
		transform(1, 0, 0, 1, x, y);
	}
	
	//arc
	public static void arc(double x1, double y1, double x2, double y2, double x3, double y3) {
        if (pathPos + 7 >= path.length) {
        	enlargeBuf(7);
        }
		path[pathPos++] = ID_ARC;
		path[pathPos++] = x1;
		path[pathPos++] = y1;
		path[pathPos++] = x2;
		path[pathPos++] = y2;
		path[pathPos++] = x3;
		path[pathPos++] = y3;
	}
	
	//restore
	public static void restore() {
		double[] state = stack.elementAt(stack.size() - 1);
		scaleX = state[0];
		scew1 = state[1];
		scew2 = state[2];
		scaleY = state[3];
		translateX = state[4];
		translateY = state[5];
		stack.removeElementAt(stack.size() - 1);
	}

	//save
	public static void save() {
		stack.addElement(new double[] { scaleX, scew1, scew2, scaleY, translateX, translateY });
	}
	
	//fill
	public static void fill() {
		fill = true;
		beginPath0();
		drawPath();
		fillPoly(currentFillColor);
	}

	//moveTo
	public static void moveTo(double x, double y) {
		if (pathPos + 3 >= path.length) {
			enlargeBuf(3);
		}
		path[pathPos++] = ID_MOVE_TO;
		path[pathPos++] = x;
		path[pathPos++] = y;
	}
	

	//lineTo
	public static void lineTo(double x, double y) {
		if (pathPos + 3 >= path.length) {
			enlargeBuf(3);
		}
		path[pathPos++] = ID_LINE_TO;
		path[pathPos++] = x;
		path[pathPos++] = y;
	}

	//closePath
	public static void closePath() {
		if (pathPos + 3 >= path.length) {
			enlargeBuf(3);
		}
		path[pathPos] = ID_CLOSE_PATH;
		// two ignored parameter to simplify drawing logic
		pathPos += 3;
	}

	//stroke
	public static void stroke() {
		if ((currentLineColor & 0x0ff000000) == 0) {
			return;
		}
		fill = false;
		//graphics.setColor(currentLineColor & 0x0ffffff);
		SSS.setColor(currentLineColor & 0x0ffffff);
		drawPath();
	}
	
	//beginPath
	public static void beginPath() {
		pathPos = 0;
	}

	//strokeRect
	public static void strokeRect(double x, double y, double w, double h) {
		strokeRect0(x, y, x + w - 1, y + h - 1);
	}
	
	//fillRect
	public static void fillRect(double x, double y, double w, double h) {
		fillRect0(x, y, x + w - 1, y + h - 1);
	}
	
	//setFillStyle
	public static void setFillStyle(String style) {
		currentFillColor = parseColor(style);
	}
	
	//getFillStyle
	public static String getFillStyle() {
        StringBuffer buf = new StringBuffer(Integer.toString(currentFillColor, 16));
        while (buf.length() < 8) {
        	buf.insert(0, '0');
        }
        buf.insert(0, '#');
        return buf.toString();
	}
	
	//setStrokeStyle
	public static void setStrokeStyle(String style) {
		currentLineColor = parseColor(style);
	}
	
	//getStrokeStyle
	public static String getStrokeStyle() {
        StringBuffer buf = new StringBuffer(Integer.toString(currentLineColor, 16));
        while (buf.length() < 8) {
        	buf.insert(0, '0');
        }
        buf.insert(0, '#');
        return buf.toString();
	}
	
	public static void test1() {
		int width = 800;
		int height = 600;
		SSSMainframe.mainframeResize(width, height);
		while (true) {
			if (SSSMainframe.mainframeGetMsg() != 0) {
				break;
			}
			SSSCanvas.canvasLock();
			//
			setFillStyle("#ff0000");
			setStrokeStyle("#cccccc");
			
			fillRect(30, 100, 100, 100);
			
			moveTo(0, 0);
			lineTo(150, 300);
			lineTo(0, 300);
			lineTo(0, 0);
			
			fill();
			stroke();
			//
			SSSCanvas.canvasUnlock();
			SSSMainframe.mainfreamRefresh();
		}
	}
	
	public static void test2() {
		int w = 800;
		int h = 600;
		SSSMainframe.mainframeResize(w, h);
		while (true) {
			if (SSSMainframe.mainframeGetMsg() != 0) {
				break;
			}
			SSSCanvas.canvasLock();
			//
			save();
			
			translate(w / 2, h / 2);
			double scale = Math.min(w, h) / 400;
			scale(scale, scale);
			beginPath();
			setFillStyle("#325fA2");
			arc(0, 0, 142 + 6, 0, Math.PI * 2, 1);
			fill();
			
			beginPath();
			setFillStyle("#ffffff");
			arc(0, 0, 142 - 6, 0, Math.PI * 2, 1);
			fill();

			rotate(-Math.PI / 2);
			setStrokeStyle("black");
			setFillStyle("black");
			//setLineWidth(8);
			//setLineCap("round");
			
			for (int i = 0; i < 12; i++) {
			    save();
			    rotate(i * Math.PI / 6);
			    fillRect(110, -2, 10, 4);
			    restore();
			}
			
			//setLineWidth(5);
			for (int i = 0; i < 60; i++) {
				if (i % 5 != 0) {
					save();
					rotate(i * Math.PI / 30);
					beginPath();
					moveTo(117, 0);
					lineTo(120, 0);
					stroke();
					restore();
				}
			}
			
			int sec = SSSTimer.getSeconds();
			int min = SSSTimer.getMinutes();
			int hr = SSSTimer.getHours();
			hr = hr % 12;
			
			// write Hours
			save();
			rotate(hr * (Math.PI / 6) + (Math.PI / 360) * min + (Math.PI / 21600) * sec);
			fillRect(-5, -5, 85, 10);
			restore();

			// write Minutes
			save();
			rotate((Math.PI / 30) * min + (Math.PI / 1800) * sec);
			fillRect(-3, -3, 112 + 6, 6);
			restore();
			
			// Write seconds
			save();
			rotate(sec * Math.PI / 30);
			setStrokeStyle("#D40000");
			setFillStyle("#D40000");
			//setLineWidth(6);
			beginPath();
			moveTo(-30, 0);
			lineTo(83, 0);
			stroke();
			
			beginPath();
			arc(0, 0, 10, 0, Math.PI * 2, 1);
			fill();

			// outer circle
			beginPath();
			arc(95, 0, 10, 0, Math.PI * 2, 1);
			stroke();

			setFillStyle("#555");
			beginPath();
			arc(0, 0, 3, 0, Math.PI * 2, 1);
			fill();
			restore();

			restore();
			
			//
			SSSCanvas.canvasUnlock();
			SSSMainframe.mainfreamRefresh();
		}
	}
}
