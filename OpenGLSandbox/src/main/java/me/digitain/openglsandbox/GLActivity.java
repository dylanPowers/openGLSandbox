package me.digitain.openglsandbox;

import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;

public class GLActivity extends Activity {

  private GLSurfaceView mGLSurfaceView;

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);

    mGLSurfaceView = new GLSurfaceView(this);

    // Check the OpenGL version for 2.0 support
    final ActivityManager activityManager =
            (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
    final ConfigurationInfo configurationInfo =
            activityManager.getDeviceConfigurationInfo();
    final boolean supportsEs2 = configurationInfo.reqGlEsVersion >= 0x00020000;

    if (supportsEs2) {
      // Request a 2.0 compatible client
      mGLSurfaceView.setEGLContextClientVersion(2);

      // Set the renderer to the demo renderer
      mGLSurfaceView.setRenderer(new LessonOneRenderer());
    } else {
      // We'll give up if there's no OpenGL 2.0 support
      return;
    }


    setContentView(mGLSurfaceView);
  }

  @Override
  protected void onResume()
  {
    super.onResume();
    mGLSurfaceView.onResume();
  }

  @Override
  protected void onPause()
  {
    super.onPause();
    mGLSurfaceView.onPause();
  }

  @Override
  public boolean onCreateOptionsMenu(Menu menu) {
    // Inflate the menu; this adds items to the action bar if it is present.
    getMenuInflater().inflate(R.menu.gl, menu);
    return true;
  }
    
}
