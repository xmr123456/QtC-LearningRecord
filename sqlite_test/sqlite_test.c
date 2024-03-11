#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>

int callback(void *para, int f_num, char **f_value, char **f_name)
{
    int i;
    for (i = 0; i < f_num; ++i)
    {
        printf("%s:%s\t", f_name[i], f_value[i]);
    }
    printf("\n");
    return 0;
}

int main()
{
    sqlite3 *db;
    char *errormsg;
    int ret;

    // 打开数据库
    ret = sqlite3_open("config.db", &db);
    if (ret != SQLITE_OK)
    {
        perror("open database failed!!!\n");
        return -1;
    }

    // 创建数据表
    ret = sqlite3_exec(db, "CREATE TABLE if not exists config (id INTEGER AUTO INCREMENT,\
                                                              applicationName varchar(50),\
                                                              applicationService varchar(50),\
                                                              applicationType varchar(50));",
                       NULL, NULL, &errormsg);
    if (ret != SQLITE_OK)
    {
        perror("create table config failed!!!\n");
        sqlite3_free(errormsg);
        return -2;
    }

    // 插入数据
    ret = sqlite3_exec(db, "INSERT into config (applicationName,applicationService,applicationType) VALUES('mxapp2','myir.cn.mxapp','qt');", NULL, NULL, &errormsg);
    if (ret != SQLITE_OK)
    {
        perror("insert data failed!!!\n");
        sqlite3_free(errormsg);
        return -3;
    }

    // 查询数据
    printf("after insert data:\n");
    sqlite3_exec(db, "SELECT * FROM config;", callback, NULL, &errormsg);
    printf("\n");

    // 删除数据
    ret = sqlite3_exec(db, "delete from config where applicationName='mxapp2';", NULL, NULL, &errormsg);
    if (ret != SQLITE_OK)
    {
        perror("delete data failed!!!\n");
        sqlite3_free(errormsg);
        return -4;
    }

    // 查询数据
    printf("after delete data:\n");
    sqlite3_exec(db, "SELECT * FROM config;", callback, NULL, &errormsg);
    printf("\n");

    // 关闭数据库
    sqlite3_close(db);

    return 0;
}