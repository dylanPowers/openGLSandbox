package me.digitain.openglsandbox;

import android.opengl.GLSurfaceView;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by dylan on 10/5/13.
 */
public class LessonOneRenderer implements GLSurfaceView.Renderer {

  // Model data in float buffers
  private final FloatBuffer mTriangle1Vertices;
  private final FloatBuffer mTriangle2Vertices;
  private final FloatBuffer mTriangle3Vertices;

  private final int mBytesPerFloat = 4; // DUH!!!?????

  public LessonOneRenderer() {

    // A red, green, and blue triangle
    final float[] triangle1VerticesData = {
        // X, Y, Z,
        // R, G, B, A
        -0.5f, -0.25f, 0.0f,
        1.0f, 0.0f, 0.0f, 1.0f,

        0.5f, -0.25f, 0.0f,
        0.0f, 0.0f, 1.0f, 1.0f,

        0.0f, 0.559016994f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f
    };

    // This triangle is yellow, cyan, and magenta.
    final float[] triangle2VerticesData = {
        // X, Y, Z,
        // R, G, B, A
        -0.5f, -0.25f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f,

        0.5f, -0.25f, 0.0f,
        0.0f, 1.0f, 1.0f, 1.0f,

        0.0f, 0.559016994f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f
    };

    // This triangle is white, gray, and black.
    final float[] triangle3VerticesData = {
        // X, Y, Z,
        // R, G, B, A
        -0.5f, -0.25f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f,

        0.5f, -0.25f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f,

        0.0f, 0.559016994f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    // Init our buffers
    mTriangle1Vertices = allocateVertexBuffer(triangle1VerticesData);
    mTriangle2Vertices = allocateVertexBuffer(triangle2VerticesData);
    mTriangle3Vertices = allocateVertexBuffer(triangle3VerticesData);

    mTriangle1Vertices.put(triangle1VerticesData).position(0);
    mTriangle2Vertices.put(triangle2VerticesData).position(0);
    mTriangle3Vertices.put(triangle3VerticesData).position(0);
  }

  private FloatBuffer allocateVertexBuffer(float[] verticesData) {
    return ByteBuffer
        .allocateDirect(verticesData.length * mBytesPerFloat)
        .order(ByteOrder.nativeOrder())
        .asFloatBuffer();
  }

  private float[] mViewMatrix = new float[16];

  @Override
  public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
    
  }

  @Override
  public void onSurfaceChanged(GL10 gl10, int i, int i2) {

  }

  @Override
  public void onDrawFrame(GL10 gl10) {

  }
}
