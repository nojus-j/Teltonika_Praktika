#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include "net.h"

// Ignoruojame visus gautus duomenis, kad neisvestu duomenu i terminala
static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    return size * nmemb;
}

double test_download(const char* url) {

    CURL *curl = curl_easy_init();
    if (!curl) {
        printf("Curl iniciavimas nepavyko\n");
        return -1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        printf("Atsiuntimo greicio nustatymas nepavyko\n");
        curl_easy_cleanup(curl);
        return -1;
    }

    curl_off_t raw_speed = 0;
    curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD_T, &raw_speed);

    curl_easy_cleanup(curl);

    if (raw_speed <= 0) {
        printf("Atsiuntimo greicio duomenu nepavyko gauti\n");
        return -1;
    }

    double speed_mbps = (raw_speed * 8.0) / 1000000.0;
    printf("Apskaiciuotas greitis: %.2f Mbps\n", speed_mbps);

    return speed_mbps;
}

double test_upload(const char* url) {

    CURL *curl = curl_easy_init();
    if (!curl) {
        printf("Curl iniciavimas nepavyko\n");
        return -1;
    }

    int data_size = 5 * 1024 * 1024;
    char *data = malloc(data_size);
    if (!data) {
        printf("Duomenu alokacija nepavyko\n");
        return -1;
    }

    memset(data, 'A', data_size);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data_size);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);

    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        printf("Siuntimas nepavyko\n");
        curl_easy_cleanup(curl);
        free(data);
        return -1;
    }

    curl_off_t raw_speed = 0;
    curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD_T, &raw_speed);

    curl_easy_cleanup(curl);
    free(data);

    if (raw_speed <= 0) {
        printf("Siuntimo greicio duomenu nepavyko gauti\n");
        return -1;
    }

    double speed_mbps = (raw_speed * 8.0) / 1000000.0;
    return speed_mbps;
}

int is_server_working(const char* url) {

    if (!url || strlen(url) < 5) return 0;

    if (strstr(url, "upload.php") || strstr(url, "post"))
        return 1;

    CURL *curl = curl_easy_init();
    if (!curl) return 0;

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_RANGE, "0-50000");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    CURLcode res = curl_easy_perform(curl);

    double downloaded = 0;
    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &downloaded);

    curl_easy_cleanup(curl);

    return (res == CURLE_OK && downloaded > 1000);
}
