#include <jni.h>
#include <string>
#include "parson.h"
#include "android/log.h"

#define  LOG_TAG    "dds_log"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,  LOG_TAG, __VA_ARGS__ )


extern "C" JNIEXPORT jstring JNICALL
Java_com_dds_parson_Parson_test(
        JNIEnv *env,
        jclass clazz) {
    std::string hello = "Hello from C++";
    // 包装一段jsonObject
    JSON_Value *root_value = json_value_init_object();
    char *serialized_string = 0;
    json_object_set_string(json_object(root_value), "name", "大大帅");
    json_object_set_number(json_object(root_value), "age", 27);
    json_object_set_string(json_object(root_value), "occupation", "Programmer");
    json_object_dotset_string(json_object(root_value), "address.city", "Cupertino");
    json_object_dotset_value(json_object(root_value), "sub.habbit",
                             json_parse_string(R"(["看书","打球","美女"])"));
    json_object_dotset_value(json_object(root_value), "contact.emails",
                             json_parse_string(R"(["email@example.com","email2@example.com"])"));

    // 导入object
    JSON_Value *pJsonSub = json_value_init_object();
    json_object_set_value(json_object(root_value), "address", pJsonSub);
    json_object_set_string(json_object(pJsonSub), "dds", "ddssingsong");

    // 导入array
    pJsonSub = json_value_init_array();
    json_object_set_value(json_object(root_value), "links", pJsonSub);

    JSON_Value *pJsonSubSub = json_value_init_object();
    json_array_append_value(json_array(pJsonSub), pJsonSubSub);
    json_object_set_string(json_object(pJsonSubSub), "dds", "ddssingsong");
    json_object_set_string(json_object(pJsonSubSub), "address", "https://baidu.com");
    pJsonSubSub = json_value_init_object();
    json_object_set_string(json_object(pJsonSubSub), "dds", "ddssingsong");
    json_object_set_string(json_object(pJsonSubSub), "address", "https://baidu.com");

    serialized_string = json_serialize_to_string_pretty(root_value);
    json_value_free(root_value);

    return env->NewStringUTF(serialized_string);
}


extern "C"
JNIEXPORT jobject JNICALL
Java_com_dds_parson_Parson_parseJson(JNIEnv *env, jclass clazz, jstring json) {
    JSON_Value *root_value;
    const char *cJson = env->GetStringUTFChars(json, JNI_FALSE);

    root_value = json_parse_string(cJson);
    JSON_Value_Type jsonValueType = json_value_get_type(root_value);
    if (jsonValueType == JSONArray) {
        LOGD("json start with jsonObject");
    }
    char g_log_info[200];
    if (jsonValueType == JSONObject) {
        LOGD("json start with jsonObject");
        JSON_Object *jsonObject = json_value_get_object(root_value);
        const char *name = json_object_get_string(jsonObject, "name");
        sprintf(g_log_info, "%s",name);
        LOGD("name : %s", g_log_info);
        const char *occu = json_object_get_string(jsonObject, "occupation");
        sprintf(g_log_info, "%s",occu);
        LOGD("occupation : %s", g_log_info);
        JSON_Array *habbits = json_object_dotget_array(jsonObject, "sub.habbit");
        size_t a = 0;
        size_t size = json_array_get_count(habbits);
        for (a = 0; a < size; a++) {
            const char *ha = json_array_get_string(habbits, a);
            sprintf(g_log_info, "%s",ha);
            LOGD("habbit : %s", g_log_info);
        }
    }
    json_value_free(root_value);
    return NULL;

}

