package com.geometria.samples

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log

class MainActivity : AppCompatActivity() {
    companion object {
        init {
            System.loadLibrary("Samples")
        }

        val TAG = "GeometriaSamples"
    }

    external fun callNative(): Int

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        Log.d(TAG, "Native fun returned `${callNative()}`")
    }
}