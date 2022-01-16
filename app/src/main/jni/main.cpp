//
// Created by PCAditiaID29 on 2/10/2021.
//

#include "main.h"
#include <jni.h>

#include "CanvasEngine.h"
#include "HacksDrawing.h"

CanvasEngine canvasEngine;


extern "C"
JNIEXPORT void JNICALL
Java_android_raincitygming_view_ROverlayView_setValueBoolean(JNIEnv *env, jclass clazz,
                                                             jint feature, jboolean is_true) {
    switch (feature) {
        case 1:
            showName = is_true;
            break;
        case 2:
            showDistance = is_true;
            break;
        case 3:
            showHealth = is_true;
            break;
        case 4:
            showTeamID = is_true;
            break;
        case 5:
            showBox = is_true;
            break;
        case 6:
            showRadarLine = is_true;
            break;
        case 7:
            showSkeleton = is_true;
            break;
        case 8:
            showWeapon = is_true;
            break;
        case 9:
            showEnemyCount = is_true;
            break;
        case 10:
            show360Warning = is_true;
            break;
        case 11:
            showHeadDots = is_true;
            break;
        case 12:
            showGrenadeAlert = is_true;
            break;
        case 13:
            isLessRecoil = is_true;
            break;
        case 14:
            isInstantHit = is_true;
            break;
        case 15:
            isFastParachute = is_true;
            break;
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_android_raincitygming_view_ROverlayView_GetInitCanvasDrawing(JNIEnv *env, jclass clazz,
                                                                  jobject r_canvas_draw,
                                                                  jobject canvas) {
    canvasEngine = CanvasEngine(env, r_canvas_draw, canvas);
    if (canvasEngine.isValid()){
        MainDraw(canvasEngine);
    }
}

extern "C"
JNIEXPORT jint JNICALL
Java_android_raincitygming_service_RService_InitBase(JNIEnv *env, jobject thiz, jint version_code,
                                                     jint width, jint heigth) {
    if (!Create()){
        LOGE("[NDK] Socket can't create");
        return -1;
    }
    if (!Bind()){
        LOGE("[NDK] Socket can't bind");
        return -1;
    }
    if (!Listen()){
        LOGE("[NDK] Socket can't listen");
        return -1;
    }
    if (Accept()){
        LOGI("[NDK] Socket accepted");
        Request request{};
        request.Mode = InitMode;
        request.ScreenWidth = width;
        request.ScreenHeight = heigth;
        request.VersionCode = version_code;
        send((void*) &request, sizeof(request));
    }
    return 1;
}
extern "C"
JNIEXPORT void JNICALL
Java_android_raincitygming_service_RService_CloseSocket(JNIEnv *env, jclass clazz) {
    Close();
}