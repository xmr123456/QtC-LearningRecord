#include <stdio.h>
#include <stdbool.h>
#include <curl/curl.h>

// 回调函数，用于处理接收到的 FTP 数据
size_t writeCallback(char *buf, size_t size, size_t nmemb, void *stream) 
{
    size_t written = fwrite(buf, size, nmemb, (FILE *)stream);	
    return written;
}

//显示文件传输进度，dltotal代表文件大小，dlnow代表传输已经完成部分
//clientp是CURLOPT_PROGRESSDATA传入的值
int progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{	
    if (dltotal != 0)
    {
        printf("%lf / %lf (%lf %%)\n", dlnow, dltotal, dlnow*100.0 / dltotal);
    }	
    return 0;
}

int main() {
    CURL* curl;
    CURLcode res;

    // 初始化 libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);

    // 创建一个 curl 句柄
    curl = curl_easy_init();
    if (curl) {
        // 设置 FTP URL
        curl_easy_setopt(curl, CURLOPT_URL, "ftp://192.168.10.9/test.txt");

        // 设置用户名和密码（如果需要）
        curl_easy_setopt(curl, CURLOPT_USERPWD, "xmr:123456");

        // 设置写回调函数
        FILE *fd = fopen("download_test.txt","w");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fd);

        //实现下载进度
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_callback);
        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, NULL);

        // 执行 FTP 下载操作
        curl_easy_perform(curl);

        fclose(fd);

        // 清理 curl 句柄
        curl_easy_cleanup(curl);
    }

    // 清理 libcurl
    curl_global_cleanup();

    return 0;
}
