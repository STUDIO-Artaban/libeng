#include "Main.h" // Force to include this file first

#include <libeng/Tools/Tools.h>
#ifdef LIBENG_ENABLE_SOCIAL
#include <libeng/Social/Session.h>
#endif

#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <android/log.h>
#include <new>
#include <string>
#include <cstring>
#include <assert.h>

static const jchar fileNotFound[] = { 35, 70, 78, 70, 35 }; // == '#FNF#' java string

jbyte* camBuffer = NULL;
jsize camBufferLen = 0;

jshort* micBuffer = NULL;
jsize micBufferLen = 0;

jclass activityClass;
jobject activityObject;

#ifdef LIBENG_ENABLE_SOCIAL
jclass facebookClass;
jclass twitterClass;
jclass googleClass;
#endif

//////
JavaVM* javaVM = NULL;

jint JNI_OnLoad(JavaVM* pVM, void* reserved) {

    javaVM = pVM; // Get Java VM
    return JNI_VERSION_1_6;
}

#ifdef __cplusplus
extern "C" {
#endif

    JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_init(JNIEnv* env, jobject obj, jobject activity,
            jint millis, jfloat accelRange, jfloat xDpi, jfloat yDpi);
    JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_create(JNIEnv* env,jobject obj);
    JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_start(JNIEnv* env,jobject obj, jobject intent);
    JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_change(JNIEnv* env,jobject obj, jobject surface);
    JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_resume(JNIEnv* env, jobject obj, jint millis);
    JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_result(JNIEnv* env, jobject obj, jint req,
            jint res, jobject intent);
    JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_pause(JNIEnv* env, jobject obj, jboolean finishing,
            jboolean lockScreen);
    JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_destroy(JNIEnv* env, jobject obj);
    JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_stop(JNIEnv* env,jobject obj);
    JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_free(JNIEnv* env, jobject obj);

    JNIEXPORT jshort Java_com_studio_artaban_apptest_EngLibrary_loadTexture(JNIEnv* env, jobject obj, jshort id,
            jshort width, jshort height, jbyteArray data, jboolean grayscale);
    JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_loadOgg(JNIEnv* env, jobject obj, jshort id,
            jbyteArray data, jboolean queued);
    JNIEXPORT jboolean Java_com_studio_artaban_apptest_EngLibrary_loadFile(JNIEnv* env, jobject obj, jstring file,
            jstring content);
    JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_loadCamera(JNIEnv* env, jobject obj,
            jbyteArray data);
    JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_loadMic(JNIEnv* env, jobject obj, jint len,
            jshortArray data);
    JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_loadSocial(JNIEnv* env,jobject obj, jshort id,
            jshort request, jshort result, jshort width, jshort height, jbyteArray data);
    JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_loadStore(JNIEnv* env,jobject obj, jshort result);

    JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_touch(JNIEnv* env, jobject obj, jint id,
            jshort type, jfloat x, jfloat y);
    JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_accelerometer(JNIEnv* env, jobject obj,
            jfloat xRate, jfloat yRate, jfloat zRate);

#ifdef __cplusplus
};
#endif

//////
JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_init(JNIEnv* env, jobject obj, jobject activity,
        jint millis, jfloat accelRange, jfloat xDpi, jfloat yDpi) {

    std::string className(JAVA_PROJECT_NAME);
    className.append(LIBENG_ACTIVITY_CLASS);

    activityClass = static_cast<jclass>(env->NewGlobalRef(env->FindClass(className.c_str())));
    if (!activityClass)
        __android_log_print(ANDROID_LOG_ERROR, "JNI", "Failed to find activity class: %s", className.c_str());

    activityObject = env->NewGlobalRef(activity);
    if (!activityObject)
        __android_log_print(ANDROID_LOG_ERROR, "JNI", "Failed to get activity object");

#ifdef LIBENG_ENABLE_SOCIAL
    className = JAVA_PROJECT_NAME;
    className.append(LIBENG_FACEBOOK_CLASS);
    facebookClass = static_cast<jclass>(env->NewGlobalRef(env->FindClass(className.c_str())));
    if (!facebookClass)
        __android_log_print(ANDROID_LOG_ERROR, "JNI", "Failed to find facebook class");

    className = JAVA_PROJECT_NAME;
    className.append(LIBENG_TWITTER_CLASS);
    twitterClass = static_cast<jclass>(env->NewGlobalRef(env->FindClass(className.c_str())));
    if (!twitterClass)
        __android_log_print(ANDROID_LOG_ERROR, "JNI", "Failed to find twitter class");

    className = JAVA_PROJECT_NAME;
    className.append(LIBENG_GOOGLE_CLASS);
    googleClass = static_cast<jclass>(env->NewGlobalRef(env->FindClass(className.c_str())));
    if (!googleClass)
        __android_log_print(ANDROID_LOG_ERROR, "JNI", "Failed to find google class");
#endif

    PlatformData platformData;
    platformData.jvm = javaVM;
    platformData.jpn = JAVA_PROJECT_NAME;
    platformData.cls = activityClass;
    platformData.obj = activityObject;
#ifdef LIBENG_ENABLE_SOCIAL
    platformData.facebook = facebookClass;
    platformData.twitter = twitterClass;
    platformData.google = googleClass;
#endif
    platformData.accelRange = static_cast<float>(accelRange);
    platformData.xDpi = static_cast<float>(xDpi);
    platformData.yDpi = static_cast<float>(yDpi);

    init(platformData);
    platformInit(static_cast<long>(millis), &platformData);
}
JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_create(JNIEnv* env,jobject obj) {
    platformCreate();
}
JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_start(JNIEnv* env,jobject obj, jobject intent) {
    platformStart(intent);
}
JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_change(JNIEnv* env,jobject obj, jobject surface) {
    platformChange(ANativeWindow_fromSurface(env, surface));
}
JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_resume(JNIEnv* env, jobject obj, jint millis) {
    platformResume(static_cast<long>(millis));
}
JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_result(JNIEnv* env, jobject obj, jint req,
        jint res, jobject intent) {

    jobject localIntent = env->NewLocalRef(intent);
    platformResult(req, res, localIntent);
    env->DeleteLocalRef(localIntent);
}
JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_pause(JNIEnv* env, jobject obj, jboolean finishing,
        jboolean lockScreen) {

    if ((!static_cast<bool>(finishing)) && (camBuffer)) {

        delete [] camBuffer;
        camBuffer = NULL;
    }
    platformPause(static_cast<bool>(finishing), !static_cast<bool>(lockScreen));
    if ((!static_cast<bool>(finishing)) && (micBuffer)) {

        delete [] micBuffer;
        micBuffer = NULL;
    }
}
JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_destroy(JNIEnv* env, jobject obj) {
    platformDestroy();
}
JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_stop(JNIEnv* env,jobject obj) {
    platformStop();
}
JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_free(JNIEnv* env, jobject obj) {

    if (camBuffer)
        delete [] camBuffer;
    platformFree();
    if (micBuffer)
        delete [] micBuffer;

    env->DeleteGlobalRef(activityClass);
    env->DeleteGlobalRef(activityObject);
#ifdef LIBENG_ENABLE_SOCIAL
    env->DeleteGlobalRef(facebookClass);
    env->DeleteGlobalRef(twitterClass);
    env->DeleteGlobalRef(googleClass);
#endif
}

JNIEXPORT jshort Java_com_studio_artaban_apptest_EngLibrary_loadTexture(JNIEnv* env, jobject obj, jshort id,
        jshort width, jshort height, jbyteArray data, jboolean grayscale) {

    if (!data) {
        __android_log_print(ANDROID_LOG_ERROR, "JNI", "loadTexture (i:%d): NULL buffer", id);
        return static_cast<jshort>(0xFF); // NO_TEXTURE_IDX
    }
    jsize len = env->GetArrayLength(data);
    try {

        __android_log_print(ANDROID_LOG_INFO, "JNI", "loadTexture (i:%d): w:%d; h:%d len:%d", id, width, height, len);

        jbyte* buffer = new jbyte[len];
        env->GetByteArrayRegion(data, 0, len, buffer);
        if (grayscale) {

            // Convert RGBA color buffer into LA buffer (Luminance Alpha)
            unsigned int graySize = width * height;
            jbyte* grayBuffer = new jbyte[graySize * 2];

            for (unsigned int i = 0; i < graySize; ++i) {

                grayBuffer[(i * 2) + 0] = buffer[(i * 4) + 0]; // Luminance (Red == Green == Blue: 0 -> Red)
                grayBuffer[(i * 2) + 1] = buffer[(i * 4) + 3]; // Alpha
            }
            delete []  buffer;
            buffer = grayBuffer;
        }
        return static_cast<jshort>(platformLoadTexture(static_cast<unsigned char>(id), width, height,
                reinterpret_cast<unsigned char*>(buffer), static_cast<bool>(grayscale)));
    }
    catch (const std::bad_alloc &e) {
        __android_log_print(ANDROID_LOG_ERROR, "JNI", "loadTexture (i:%d): %s", id, e.what());
        return static_cast<jshort>(0xFF); // NO_TEXTURE_IDX
    }
}
JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_loadOgg(JNIEnv* env, jobject obj, jshort id,
        jbyteArray data, jboolean queued) {

    if (!data) {
        __android_log_print(ANDROID_LOG_ERROR, "JNI", "loadOgg (i:%d): NULL buffer", id);
        return;
    }
    jsize len = env->GetArrayLength(data);
    try {

        __android_log_print(ANDROID_LOG_INFO, "JNI", "loadOgg (i:%d): len:%d", id, len);

        jbyte* buffer = new jbyte[len];
        env->GetByteArrayRegion(data, 0, len, buffer);
        platformLoadOgg(static_cast<unsigned char>(id), len, reinterpret_cast<unsigned char*>(buffer),
                static_cast<bool>(queued));
    }
    catch (const std::bad_alloc &e) {
        __android_log_print(ANDROID_LOG_ERROR, "JNI", "loadOgg (i:%d): %s", id, e.what());
    }
}
JNIEXPORT jboolean Java_com_studio_artaban_apptest_EngLibrary_loadFile(JNIEnv* env, jobject obj, jstring file,
        jstring content) {

    if (!file) {
        __android_log_print(ANDROID_LOG_ERROR, "JNI", "loadFile: NULL file buffer");
        return static_cast<jboolean>(false);
    }
    const char* strFile = env->GetStringUTFChars(file, 0);

    if (!content) {
        __android_log_print(ANDROID_LOG_ERROR, "JNI", "loadFile (f:%s): NULL content buffer", strFile);
        env->ReleaseStringUTFChars(file, strFile);
        return static_cast<jboolean>(false);
    }

    __android_log_print(ANDROID_LOG_INFO, "JNI", "loadFile '%s'", strFile);
    assert(sizeof(jchar) == 2);
    assert(sizeof(wchar_t) == 4);

    jsize len = env->GetStringLength(content);
    const jchar* jstrContent = env->GetStringChars(content, 0);
    if (len % 2) { // The only way to get a none modulos length is...
        if ((sizeof(fileNotFound) == (len << 1)) && (!std::memcmp(jstrContent, fileNotFound, len))) { // ...when file is not found

            platformLoadFile(strFile, LIBENG_FILE_NOT_FOUND);
            env->ReleaseStringChars(content, jstrContent);
        }
        else {
            __android_log_print(ANDROID_LOG_ERROR, "JNI", "Unexpected '%s' file format", strFile);
            env->ReleaseStringChars(content, jstrContent);
            env->ReleaseStringUTFChars(file, strFile);
            return static_cast<jboolean>(false);
        }
    }
    else {

        wchar_t* wstrContent = java2wstring(jstrContent, len);
        platformLoadFile(strFile, wstrContent);
        delete [] wstrContent;
        env->ReleaseStringChars(content, jstrContent);
    }
    env->ReleaseStringUTFChars(file, strFile);
    return static_cast<jboolean>(true);
}
JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_loadCamera(JNIEnv* env, jobject obj, jbyteArray data) {

    if (!data) {
        __android_log_print(ANDROID_LOG_ERROR, "JNI", "loadCamera: NULL buffer");
        return;
    }
    jsize len = env->GetArrayLength(data);
    if (!camBuffer) {

        try {

            __android_log_print(ANDROID_LOG_INFO, "JNI", "loadCamera (len:%d)", len);

            camBufferLen = len;
            camBuffer = new jbyte[len];
        }
        catch (const std::bad_alloc &e) {
            __android_log_print(ANDROID_LOG_ERROR, "JNI", "loadCamera: %s", e.what());
            return;
        }
    }
    assert(camBufferLen == len);
    env->GetByteArrayRegion(data, 0, len, camBuffer);
    platformLoadCamera(reinterpret_cast<const unsigned char*>(camBuffer));
}
JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_loadMic(JNIEnv* env, jobject obj, jint len,
        jshortArray data) {

    if (!data) {
        __android_log_print(ANDROID_LOG_ERROR, "JNI", "loadMic: NULL buffer");
        return;
    }
    if (!micBuffer) {

        try {

            micBufferLen = env->GetArrayLength(data);
            __android_log_print(ANDROID_LOG_INFO, "JNI", "loadMic (len:%d)", micBufferLen);
            micBuffer = new jshort[micBufferLen];
        }
        catch (const std::bad_alloc &e) {
            __android_log_print(ANDROID_LOG_ERROR, "JNI", "loadMic: %s", e.what());
            return;
        }
    }
    assert(micBufferLen == env->GetArrayLength(data));
    assert(micBufferLen >= len);
    env->GetShortArrayRegion(data, 0, len, micBuffer);
    platformLoadMic(static_cast<int>(len), reinterpret_cast<const short*>(micBuffer));
}
JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_loadSocial(JNIEnv* env,jobject obj, jshort id, jshort request,
        jshort result, jshort width, jshort height, jbyteArray data) {

#ifdef LIBENG_ENABLE_SOCIAL
    switch (static_cast<Session::RequestID>(request)) {

        case Session::REQUEST_LOGIN:
        case Session::REQUEST_INFO:
        case Session::REQUEST_SHARE_LINK:
        case Session::REQUEST_SHARE_VIDEO: {

            __android_log_print(ANDROID_LOG_INFO, "JNI", "Load social result: i:%d; r:%d; r:%d (w:%d; h:%d; d:%x)",
                    id, request, result, width, height, data);
            assert(!width);
            assert(!height);
            assert(!data);

            platformLoadSocial(static_cast<unsigned char>(id), static_cast<unsigned char>(request),
                    static_cast<unsigned char>(result), 0, 0, NULL);
            break;
        }
        case Session::REQUEST_PICTURE: {
            if (!data) {

                __android_log_print(ANDROID_LOG_WARN, "JNI", "Load social picture (i:%d): NULL buffer", id);
                assert(!width);
                assert(!height);

                platformLoadSocial(static_cast<unsigned char>(id), static_cast<unsigned char>(request),
                        static_cast<unsigned char>(result), 0, 0, NULL);
                break;
            }
            jsize len = env->GetArrayLength(data);
            try {

                __android_log_print(ANDROID_LOG_INFO, "JNI", "Load social picture (i:%d; r:%d; r:%d): w:%d; h:%d len:%d",
                        id, request, result, width, height, len);

                jbyte* buffer = new jbyte[len];
                env->GetByteArrayRegion(data, 0, len, buffer);
                platformLoadSocial(static_cast<unsigned char>(id), static_cast<unsigned char>(request),
                        static_cast<unsigned char>(result), width, height,
                        reinterpret_cast<unsigned char*>(buffer));
            }
            catch (const std::bad_alloc &e) {
                __android_log_print(ANDROID_LOG_ERROR, "JNI", "Social picture (i:%d): %s", id, e.what());
                assert(NULL);
            }
            break;
        }
        default: {
            __android_log_print(ANDROID_LOG_ERROR, "JNI", "Invalid request ID: %d", id);
            assert(NULL);
            break;
        }
    }
#else
    __android_log_print(ANDROID_LOG_FATAL, "JNI", "'loadSocial' function called without LIBENG_ENABLE_SOCIAL flag");
    assert(NULL);
#endif
}
void Java_com_studio_artaban_apptest_EngLibrary_loadStore(JNIEnv* env,jobject obj, jshort result) {

#ifdef LIBENG_ENABLE_STORAGE
    platformLoadStore(static_cast<unsigned char>(result));
#else
    __android_log_print(ANDROID_LOG_FATAL, "JNI", "'loadStore' function called without LIBENG_ENABLE_STORAGE flag");
    assert(NULL);
#endif
}

JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_touch(JNIEnv* env, jobject obj, jint id, jshort type,
        jfloat x, jfloat y) {
    platformTouch(static_cast<unsigned int>(id), static_cast<unsigned char>(type), static_cast<short>(x),
            static_cast<short>(y));
}
JNIEXPORT void Java_com_studio_artaban_apptest_EngLibrary_accelerometer(JNIEnv* env, jobject obj, jfloat xRate,
        jfloat yRate, jfloat zRate) {
    platformAccelerometer(static_cast<float>(xRate), static_cast<float>(yRate), static_cast<float>(zRate));
}
