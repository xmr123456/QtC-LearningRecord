#include <stdio.h>
#include <string.h>
#include "cJSON.h"

/*
{
    "name": "MQ",
    "age": 25,
    "height": 183.5,
    "gender": false,
    "address": {
        "country": "China",
        "zip-code": 123456
    },
    "subject": [
        "政治",
        "数学",
        "英语",
        "专业课"
    ],
    "time": [
        123,
        456,
        789,
        150
    ],
    "grade": [
        66.51,
        118.52,
        61.53,
        128.54
    ],
    "student": [
        {
            "name": "张三",
            "age": 24,
            "gender": true
        },
        {
            "name": "李四",
            "age": 25,
            "gender": true
        },
        {
            "name": "王五",
            "age": 26,
            "gender": true
        }
    ]
}
*/

// 构造json文件
int write_json()
{
    double grade[4] = {66.51, 118.52, 61.53, 128.54};
    int time[4] = {123, 456, 789, 150};

    cJSON *TCP = cJSON_CreateObject(); // 创建一个对象

    cJSON_AddStringToObject(TCP, "name", "MQ");     // 添加字符串
    cJSON_AddNumberToObject(TCP, "age", 25);        // 添加整型数字
    cJSON_AddNumberToObject(TCP, "height", 183.52); // 添加浮点型数字
    cJSON_AddFalseToObject(TCP, "gender");          // 添加逻辑值false

    cJSON *ADD = cJSON_CreateObject();                // 创建一个对象
    cJSON_AddStringToObject(ADD, "country", "China"); // 添加字符串
    cJSON_AddNumberToObject(ADD, "zip-code", 123456); // 添加整型数字
    cJSON_AddItemToObject(TCP, "address", ADD);

    cJSON *SUB = cJSON_CreateArray();         // 创建一个数组
    cJSON_AddStringToObject(SUB, "", "政治"); // 添加字符串到数组
    cJSON_AddStringToObject(SUB, "", "数学");
    cJSON_AddStringToObject(SUB, "", "英语");
    cJSON_AddStringToObject(SUB, "", "专业课");
    cJSON_AddItemToObject(TCP, "subject", SUB); // 添加数组到对象

    cJSON *TIM = cJSON_CreateIntArray(time, 4); // 创建一个整型数组
    cJSON_AddItemToObject(TCP, "time", TIM);

    cJSON *GRA = cJSON_CreateDoubleArray(grade, 4); // 创建一个双浮点型数组
    cJSON_AddItemToObject(TCP, "grade", GRA);

    cJSON *STU = cJSON_CreateArray(); // 创建一个数组

    cJSON *Z3 = cJSON_CreateObject();            // 创建一个对象
    cJSON_AddStringToObject(Z3, "name", "张三"); // 添加字符串
    cJSON_AddNumberToObject(Z3, "age", 24);      // 添加整型数字
    cJSON_AddTrueToObject(Z3, "gender");         // 添加逻辑值
    cJSON_AddItemToArray(STU, Z3);               // 添加对象到数组中

    cJSON *L4 = cJSON_CreateObject();            // 创建一个对象
    cJSON_AddStringToObject(L4, "name", "李四"); // 添加字符串
    cJSON_AddNumberToObject(L4, "age", 25);      // 添加整型数字
    cJSON_AddTrueToObject(L4, "gender");         // 添加逻辑值
    cJSON_AddItemToArray(STU, L4);               // 添加对象到数组中

    cJSON *W5 = cJSON_CreateObject();            // 创建一个对象
    cJSON_AddStringToObject(W5, "name", "王五"); // 添加字符串
    cJSON_AddNumberToObject(W5, "age", 26);      // 添加整型数字
    cJSON_AddTrueToObject(W5, "gender");         // 添加逻辑值
    cJSON_AddItemToArray(STU, W5);               // 添加对象到数组中

    cJSON_AddItemToObject(TCP, "student", STU); // 添加数组到对象中

    char *json_data = cJSON_Print(TCP); // JSON数据结构转换为JSON字符串

    FILE *file = fopen("json_test.json", "w+");
    fwrite(json_data, strlen(json_data), 1, file);
    fclose(file);

    printf("%s\n", json_data); // 输出字符串
    cJSON_Delete(TCP);         // 清除结构体
    return 0;
}


// 解析json文件
int parse_json()
{
    char json_string[4096] = {0};
    FILE *file = fopen("json_test.json","r");
    if(file == NULL){
        printf("could not open file\n");
        return -1;
    }
    fread(json_string,sizeof(json_string),1,file);
    fclose(file);
    
    cJSON *cjson = cJSON_Parse(json_string); // 将JSON字符串转换成JSON结构体
    if (cjson == NULL){                      // 判断转换是否成功
        printf("cjson error...\r\n");
        return -1;
    }
    else{
        printf("%s\n", cJSON_Print(cjson)); // 打包成功调用cJSON_Print打印输出
    }

    printf("/*********************以下就是提取的数据**********************/\n");
    char *name = cJSON_GetObjectItem(cjson, "name")->valuestring; // 解析字符串
    printf("%s\n", name);
    int age = cJSON_GetObjectItem(cjson, "age")->valueint; // 解析整型
    printf("%d\n", age);
    double height = cJSON_GetObjectItem(cjson, "height")->valuedouble; // 解析双浮点型
    printf("%.1f\n", height);
    int gender = cJSON_GetObjectItem(cjson, "gender")->type; // 解析逻辑值---输出逻辑值对应的宏定义数值
    printf("%d\n", gender);

    cJSON *ADD = cJSON_GetObjectItem(cjson, "address");               // 解析对象
    char *country = cJSON_GetObjectItem(ADD, "country")->valuestring; // 解析对象中的字符串
    printf("%s\n", country);
    int zip = cJSON_GetObjectItem(ADD, "zip-code")->valueint; // 解析对象中的整型数字
    printf("%d\n", zip);

    cJSON *SUB = cJSON_GetObjectItem(cjson, "subject"); // 解析数组
    int SUB_size = cJSON_GetArraySize(SUB);             // 获取数组成员个数
    int i = 0;
    for (i = 0; i < SUB_size; i++)
    {
        printf("%s ", cJSON_GetArrayItem(SUB, i)->valuestring); // 解析数组中的字符串
    }
    printf("\n");
    cJSON *TIM = cJSON_GetObjectItem(cjson, "time"); // 解析数组
    int TIM_size = cJSON_GetArraySize(TIM);          // 获取数组成员个数
    for (i = 0; i < TIM_size; i++)
    {
        printf("%d ", cJSON_GetArrayItem(TIM, i)->valueint); // 解析数组中的整型数字
    }
    printf("\n");
    cJSON *GRA = cJSON_GetObjectItem(cjson, "grade"); // 解析数组
    int GRA_size = cJSON_GetArraySize(GRA);           // 获取数组成员个数
    for (i = 0; i < GRA_size; i++)
    {
        printf("%f ", cJSON_GetArrayItem(GRA, i)->valuedouble); // 解析数组中的浮点型数字
    }
    printf("\n");
    cJSON *STU = cJSON_GetObjectItem(cjson, "student"); // 解析数组
    int STU_size = cJSON_GetArraySize(STU);             // 获取数组成员个数
    cJSON *STU_item = STU->child;                       // 获取子对象
    for (i = 0; i < STU_size; i++)
    {
        printf("%s ", cJSON_GetObjectItem(STU_item, "name")->valuestring); // 解析数组中对象中的字符串
        printf("%d ", cJSON_GetObjectItem(STU_item, "age")->valueint);     // 解析数组中对象中的整型数字
        printf("%d\n", cJSON_GetObjectItem(STU_item, "gender")->type);     // 解析数组中对象中的逻辑值---输出逻辑值对应的宏定义数值
        STU_item = STU_item->next;                                         // 跳转到下一个对象中
    }
    cJSON_Delete(cjson); // 清除结构体
    return 0;
}


int main()
{
    printf("构造JSON文件:\n");
    write_json();
    printf("\n");

    printf("解析JSON文件内容:\n");
    parse_json();
    printf("\n");
    
    return 0;
}