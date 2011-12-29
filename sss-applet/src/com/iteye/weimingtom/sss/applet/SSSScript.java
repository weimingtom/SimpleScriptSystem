package com.iteye.weimingtom.sss.applet;

import org.python.util.PythonInterpreter;
import org.python.core.*;

public class SSSScript {
    private static String script = 
	"width = 800\n" +
	"height = 600\n" +
	"SSS.resize(width, height)\n" +
	"while 1:\n" +
	"	if SSS.getMsg():\n" +
	"		break\n" +
	"	SSS.lock()\n" +
	"	SSS.setColor(SSS.rgb(0xff, 0, 0))\n" +
	"	SSS.moveTo(0, 0)\n" +
	"	SSS.lineTo(width * 2, height * 2)\n" +
	"	for k in range(height / 4, height / 2):" +
	"		SSS.setPixel(width / 2, k, SSS.rgb(0xff, 0, 0xff))\n" +
	"	i = SSS.getPixel(width / 4, height / 2)\n" +
	"	SSS.drawLine(-width, height * 2, width * 2, -height, SSS.rgb(0, 0, 0xff))\n" +
	"	SSS.unlock()\n" +
	"	ms = SSS.getMouseStatus()\n" +
	"	if ms != 0:\n" +
	"		SSS.trace(\"mouse status : %d\" % ms)\n" +
	"		SSS.setTitle(\"mouse status : %d\" % ms)\n" +
	"	ks = SSS.getKeyboardStatus()\n" +
	"	if ks != 0:\n" +
	"		SSS.trace(\"keyboard status : %d\" % ks)\n" +
	"		SSS.setTitle(\"keyboard status : %d\" % ks)\n" +
	"	SSS.refresh()\n" +
	"";
    
    public static void ScriptRun() {
        PythonInterpreter interp = new PythonInterpreter();
        interp.exec("from " + 
        	SSS.class.getPackage().getName() + 
        	" import " + 
        	SSS.class.getSimpleName());
        try {
            interp.exec(script);
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            interp.cleanup();
        }
    }
    
    
    /**
     * Test Jython
     * @param args
     * @throws PyException
     */
    public static void main(String []args) throws PyException {
        PythonInterpreter interp = new PythonInterpreter();

        System.out.println("Hello, brave new world");
        interp.exec("import sys");
        interp.exec("print sys");
        
        interp.set("a", new PyInteger(42));
        interp.exec("print a");
        interp.exec("x = 2+2");
        PyObject x = interp.get("x");
        
        System.out.println("x: "+x);
        System.out.println("Goodbye, cruel world");
        interp.exec("from java.lang import System");
        interp.exec("from " + 
        	SSSConfig.class.getPackage().getName() + 
        	" import " + 
        	SSSConfig.class.getSimpleName());
        interp.exec("System.out.println(SSSConfig.WINDOW_TITLE)");
        interp.cleanup();
    }
}
