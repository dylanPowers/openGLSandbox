package me.digitain.openglsandbox;

/**
 * Created by dylan on 10/9/13.
 */
public class RendererJniWrapper {

  static {
      System.loadLibrary("renderer");
  }

  public static native void onDrawFrame();
  public static native void onSurfaceCreated();
  public static native void onSurfaceChanged(int width, int height);
}
