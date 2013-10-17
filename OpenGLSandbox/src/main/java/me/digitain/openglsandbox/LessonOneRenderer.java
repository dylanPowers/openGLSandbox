package me.digitain.openglsandbox;

import android.content.res.AssetManager;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;
import android.os.SystemClock;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by dylan on 10/5/13.
 */
public class LessonOneRenderer implements GLSurfaceView.Renderer {
  private static final String TAG = "OpenGLSandbox Renderer";

  private static final boolean RUN_NATIVE = true;

  // Model data in float buffers
  private final FloatBuffer mTriangle1Vertices;
  private final FloatBuffer mTriangle2Vertices;
  private final FloatBuffer mTriangle3Vertices;

  private final int mBytesPerFloat = 4; // DUH!!!?????

  private AssetManager mAssetsManager;

  private int mMVPMatrixHandle;

  public LessonOneRenderer(AssetManager assets) {
    RendererJniWrapper.construct();
    mAssetsManager = assets;



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

  private int createShader(String shaderSource, int shaderType) throws RuntimeException {
    int shaderHandle = GLES20.glCreateShader(shaderType);
    String shaderError = null;

    if (shaderHandle != 0) {
      GLES20.glShaderSource(shaderHandle, shaderSource);
      GLES20.glCompileShader(shaderHandle);
      final int[] compileStatus = new int[1];
      GLES20.glGetShaderiv(shaderHandle, GLES20.GL_COMPILE_STATUS, compileStatus, 0);
      if (compileStatus[0] == 0) {
        shaderError = GLES20.glGetShaderInfoLog(shaderHandle);
        GLES20.glDeleteShader(shaderHandle);
        shaderHandle = 0;
      }
    }
    if (shaderHandle == 0) {
      StringBuilder error = new StringBuilder("Error creating the ");
      switch(shaderType) {
        case GLES20.GL_VERTEX_SHADER:
          error.append("vertex");
          break;
        case GLES20.GL_FRAGMENT_SHADER:
          error.append("fragment");
          break;
        default:
          error.append("unknown");
          break;
      }
      error.append(" shader. Shader Log: ");
      error.append(shaderError);
      throw new RuntimeException(error.toString());
    }

    return shaderHandle;
  }

  private float[] mMVPMatrix = new float[16];
  private final int mStrideBytes = 7 * mBytesPerFloat;
  private final int mPositionOffset = 0;
  private final int mPositionDataSize = 3;
  private final int mColorOffset = 3;
  private final int mColorDataSize = 4;

  private void drawTriangle(final FloatBuffer aTriangleBuffer) {
    aTriangleBuffer.position(mPositionOffset);
    GLES20.glVertexAttribPointer(mPositionHandle, mPositionDataSize,
                                 GLES20.GL_FLOAT, false, mStrideBytes,
                                 aTriangleBuffer);
    GLES20.glEnableVertexAttribArray(mPositionHandle);

    aTriangleBuffer.position(mColorOffset);
    GLES20.glVertexAttribPointer(mColorHandle, mColorDataSize, GLES20.GL_FLOAT,
                                 false, mStrideBytes, aTriangleBuffer);
    GLES20.glEnableVertexAttribArray(mColorHandle);

    Matrix.multiplyMM(mMVPMatrix, 0, mViewMatrix, 0, mModelMatrix, 0);

    Matrix.multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mMVPMatrix, 0);

    GLES20.glUniformMatrix4fv(mMVPMatrixHandle, 1, false, mMVPMatrix, 0);
    GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, 3);
  }

  private int mPositionHandle;
  private int mColorHandle;
  @Override
  public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
    final String vertexShader = readAsset("VertexShader.glsl");
    final String fragmentShader = readAsset("FragmentShader.glsl");
    if (RUN_NATIVE) {
      RendererJniWrapper.onSurfaceCreated(fragmentShader, vertexShader);
    } else {
      GLES20.glClearColor(0.5f, 0.5f, 0.5f, 0.5f);

      final float eyeX = 0.0f;
      final float eyeY = 0.0f;
      final float eyeZ = 1.0f;

      final float lookX = 0.0f;
      final float lookY = 0.0f;
      final float lookZ = -5.0f;

      final float upX = 0.0f;
      final float upY = 1.0f;
      final float upZ = 0.0f;

      Matrix.setLookAtM(mViewMatrix, 0,
                        eyeX, eyeY, eyeZ,
                        lookX, lookY, lookZ,
                        upX, upY, upZ);



      int vertexShaderHandle = createShader(vertexShader, GLES20.GL_VERTEX_SHADER);
      int fragmentShaderHandle = createShader(fragmentShader, GLES20.GL_FRAGMENT_SHADER);

      int programHandle = GLES20.glCreateProgram();

      if (programHandle != 0) {
        GLES20.glAttachShader(programHandle, vertexShaderHandle);
        GLES20.glAttachShader(programHandle, fragmentShaderHandle);

        GLES20.glBindAttribLocation(programHandle, 0, "a_Position");
        GLES20.glBindAttribLocation(programHandle, 1, "a_Color");

        GLES20.glLinkProgram(programHandle);

        final int[] linkStatus = new int[1];
        GLES20.glGetProgramiv(programHandle, GLES20.GL_LINK_STATUS, linkStatus, 0);
        if (linkStatus[0] == 0)
        {
          GLES20.glDeleteProgram(programHandle);
          programHandle = 0;
        }
      }
      if (programHandle == 0) {
        throw new RuntimeException("Error creating the GLES 2.0 program.");
      }

      mMVPMatrixHandle = GLES20.glGetUniformLocation(programHandle, "u_MVPMatrix");
      mPositionHandle = GLES20.glGetAttribLocation(programHandle, "a_Position");
      mColorHandle = GLES20.glGetAttribLocation(programHandle, "a_Color");

      GLES20.glUseProgram(programHandle);
    }
  }


  private float[] mProjectionMatrix = new float[16];

  @Override
  public void onSurfaceChanged(GL10 gl10, int width, int height) {
    if (RUN_NATIVE) {
      RendererJniWrapper.onSurfaceChanged(width, height);
    } else {
      GLES20.glViewport(0, 0, width, height);

      float left = -1.0f;
      float right = 1.0f;
      float bottom = -1.0f;
      float top = 1.0f;
      final float near = 1.0f;
      final float far = 10.0f;

      final float ratio = (float) width / height;
      if (ratio > 1.0) { // Horizontal
        bottom = -1 / ratio;
        top = 1 / ratio;
      } else { // Vertical
        left = -ratio;
        right = ratio;
      }

      Matrix.setIdentityM(mProjectionMatrix, 0);
      //Matrix.frustumM(mProjectionMatrix, 0, left, right, bottom, top, near, far);
      Log.d(TAG, mProjectionMatrix.toString());
    }
  }


  private float[] mModelMatrix = new float[16];

  @Override
  public void onDrawFrame(GL10 gl10) {
    if (RUN_NATIVE) {
      RendererJniWrapper.onDrawFrame();
    } else {
      GLES20.glClear(GLES20.GL_DEPTH_BUFFER_BIT | GLES20.GL_COLOR_BUFFER_BIT);

      long time = SystemClock.uptimeMillis() % 5000L;
      float angleInDregrees = 0.0f;//(360.0f / 5000.0f) * ((int) time);

      Matrix.setIdentityM(mModelMatrix, 0);
      Matrix.rotateM(mModelMatrix, 0, angleInDregrees, 0.0f, 0.0f, 1.0f);
      drawTriangle(mTriangle1Vertices);

//      Matrix.setIdentityM(mModelMatrix, 0);
//      Matrix.translateM(mModelMatrix, 0, 0.0f, -1.0f, 0.0f);
//      Matrix.rotateM(mModelMatrix, 0, 90.0f, 1.0f, 0.0f, 0.0f);
//      Matrix.rotateM(mModelMatrix, 0, angleInDregrees, 0.0f, 0.0f, 1.0f);
//      drawTriangle(mTriangle2Vertices);
//
//      Matrix.setIdentityM(mModelMatrix, 0);
//      Matrix.translateM(mModelMatrix, 0, 1.0f, 0.0f, 0.0f);
//      Matrix.rotateM(mModelMatrix, 0, 90.0f, 0.0f, 1.0f, 0.0f);
//      Matrix.rotateM(mModelMatrix, 0, angleInDregrees, 0.0f, 0.0f, 1.0f);
//      drawTriangle(mTriangle3Vertices);
    }
  }

  private String readAsset(String assetPath) {
    final int BUFFER_LENGTH = 1024;
    final StringBuilder builder = new StringBuilder(BUFFER_LENGTH);

    try {
      InputStream stream = mAssetsManager.open(assetPath);

      int bytesRead;
      byte[] bytes = new byte[BUFFER_LENGTH];

      while ((bytesRead = stream.read(bytes)) != -1) {
        builder.append(new String(bytes, 0, bytesRead));
      }


    } catch (IOException e) {
      Log.e(TAG, "Exception while opening the asset " + assetPath, e);
    }

    return builder.toString();
  }
}
