#include "com_spring_ndkdemo_MainActivity.h"
#include <jni.h>
#include <string.h>
#include <unistd.h>

/**
 * 发布的app 签名,只有和本签名一致的app 才会返回 AUTH_KEY
 * 这个RELEASE_SIGN的值是上一步用java代码获取的值
 */
const char* RELEASE_SIGN = "308202e4308201cc020101300d06092a864886f70d010105050030373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b30090603550406130255533020170d3230313131363131333431365a180f32303530313130393131333431365a30373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b300906035504061302555330820122300d06092a864886f70d01010105000382010f003082010a0282010100c34d5ff41e6e3a1db4b28096e8ca22925e06a34b2e566fa3344c629559782a546ecf26501563469250f58ba619abb7be6350514ace369d038f2df039952af62abbe6b22e6d1acb37c51792b33540eb1e08d507f6b84679da44f46c8fd5050fed63dd89ec0c1e86e55fc33c68d6b38cb3c5135ac53cb1dc2cd2fa4b203d877acbaf0c0353dfdbcb362af8cc21dc3907082db846b26b0657b8230cbe8937c178b78a20b3e2122b86a55e35f398d6841febb6a00c8a77b1860d82f26fe68a5889d7ed095f88ad4259b09f155b46a79a4ce9dfeda24e3a78e99c2c2210fef8e6336055721ff3645df032bdef341ff145a6bfaa3d67943d3386892cd870426ea6ddad0203010001300d06092a864886f70d010105050003820101002a437332243d3096dc2f71a3ddc17d2a8f94b30d7c6bf3aaae22b5f7f5c5453dd5372109ad664a03174baa24b52814b0dbb678173c988deb21cc740b5afc0119c47059308899c6e6505de80e9850fb224781005f404303c50004128f771e06d4312f6eaeabf083efe6525273e1d0d017a07069c1bedc91daf89e789a1d046284f1afd4d102c5a0efd1ee36e55823ea5778341b654814182b8dc92d488026c973d1e80132bae58de2006f7edf69b842c9b665a9a90c349c839d884acde9cb224453d3fb73c3bc00c64d154191bd269bf9e915cc07efc883ee3c676ae0ffd1399d6397298341ed8356af767a634d27560381db52fe25ea3004aba34e1cc65561e9";
const char* RELEASE_PACKAGE = "com.spring.ndkdemo";

JNIEXPORT jstring JNICALL Java_com_spring_ndkdemo_MainActivity_verificationPackage
  (JNIEnv *env, jobject jobj, jobject context_object){

      jclass context_class = env->GetObjectClass(context_object);

      //context.getPackageManager()
      jmethodID methodId = env->GetMethodID(context_class, "getPackageManager", "()Landroid/content/pm/PackageManager;");
      jobject package_manager_object = env->CallObjectMethod(context_object, methodId);
      if (package_manager_object == NULL) {
          return NULL;
      }

      //context.getPackageName()
      methodId = env->GetMethodID(context_class, "getPackageName", "()Ljava/lang/String;");
      jstring package_name_string = (jstring)env->CallObjectMethod(context_object, methodId);
      if (package_name_string == NULL) {
          return NULL;
      }

      const char* c_package = (char*)env->GetStringUTFChars(package_name_string, 0);
      if(strcmp(c_package, RELEASE_PACKAGE) == 0) {
          return (env)->NewStringUTF("package true");
      } else {
          return (env)->NewStringUTF("package error");
      }
  }



JNIEXPORT jstring JNICALL Java_com_spring_ndkdemo_MainActivity_verificationSign
  (JNIEnv *env, jobject jobj, jobject context_object){

        jclass context_class = env->GetObjectClass(context_object);

        jmethodID methodId = env->GetMethodID(context_class, "getPackageManager", "()Landroid/content/pm/PackageManager;");
        jobject package_manager_object = env->CallObjectMethod(context_object, methodId);
        if (package_manager_object == NULL) {
            return NULL;
        }

        //context.getPackageName()
        methodId = env->GetMethodID(context_class, "getPackageName", "()Ljava/lang/String;");
        jstring package_name_string = (jstring)env->CallObjectMethod(context_object, methodId);
        if (package_name_string == NULL) {
            return NULL;
        }

        env->DeleteLocalRef(context_class);

        //PackageManager.getPackageInfo(Sting, int)
        jclass pack_manager_class = env->GetObjectClass(package_manager_object);
        methodId = env->GetMethodID(pack_manager_class, "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
        env->DeleteLocalRef(pack_manager_class);
        jobject package_info_object = env->CallObjectMethod(package_manager_object, methodId, package_name_string, 64);
        if (package_info_object == NULL) {
            return NULL;
        }

        env->DeleteLocalRef(package_manager_object);

        //PackageInfo.signatures[0]
        jclass package_info_class = env->GetObjectClass(package_info_object);
        jfieldID fieldId = env->GetFieldID(package_info_class, "signatures", "[Landroid/content/pm/Signature;");
        env->DeleteLocalRef(package_info_class);
        jobjectArray signature_object_array = (jobjectArray)env->GetObjectField(package_info_object, fieldId);
        if (signature_object_array == NULL) {
            return NULL;
        }
        jobject signature_object = env->GetObjectArrayElement(signature_object_array, 0);

        env->DeleteLocalRef(package_info_object);

        //Signature.toCharsString()
        jclass signature_class = env->GetObjectClass(signature_object);
        methodId = env->GetMethodID(signature_class, "toCharsString", "()Ljava/lang/String;");
        env->DeleteLocalRef(signature_class);
        jstring signature_string = (jstring) env->CallObjectMethod(signature_object, methodId);

        const char* c_sign = (char*)env->GetStringUTFChars(signature_string, 0);

        //签名一致  返回合法的 api key，否则返回错误
        if(strcmp(c_sign, RELEASE_SIGN)==0) {
            return (env)->NewStringUTF("sign true");
        } else {
            return (env)->NewStringUTF("sign error");
       }
  }


 JNIEXPORT jstring JNICALL Java_com_spring_ndkdemo_MainActivity_getStrFromJNI
    (JNIEnv *env, jobject jobject){

        return (env)->NewStringUTF("This is my first NDK Application");
 }
