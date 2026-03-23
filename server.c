#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>
#include "server.h"
#include "net.h"

#define FILE_NAME "speedtest_server_list.json"

// failo nuskaitymas i stringa
// cJSON_Parse priima string, o ne failas
char* read_file(const char* filename) {
	FILE *f = fopen(filename, "r");
	if (!f) {
		printf("Nepavyko atidaryti failo\n");
		return NULL;
	}

	fseek(f, 0, SEEK_END);
	long length = ftell(f);
	fseek(f, 0, SEEK_SET);

	char *data = malloc(length + 1);
	fread(data, 1, length, f);
	data[length] = '\0';

	fclose(f);
	return data;
}

void select_best_server(const char* user_country,
                        char* download_url,
                        char* upload_url)
{
    char *file_data = read_file(FILE_NAME);
    if (!file_data) {
        printf("Nepavyko nuskaityti JSON failo, pereiname prie testinio varianto\n");
        goto fallback;
    }

    cJSON *json = cJSON_Parse(file_data);
    if (!json) {
        printf("JSON klaida. Fallback.\n");
        free(file_data);
        goto fallback;
    }

    int size = cJSON_GetArraySize(json);

    // Failų kandidatų sąrašas
    const char* candidates[] = {
        "speedtest/random2000x2000.jpg",
        "speedtest/random1000x1000.jpg",
        "10MB.zip",
        "5MB.zip",
        "1MB.zip",
        "100MB.bin",
        "10MB.bin",
        "1MB.bin"
    };

    int candidate_count = sizeof(candidates) / sizeof(candidates[0]);

    for (int i = 0; i < size; i++) {

        cJSON *item = cJSON_GetArrayItem(json, i);
        if (!cJSON_IsObject(item)) continue;

        cJSON *country = cJSON_GetObjectItem(item, "country");
        cJSON *host    = cJSON_GetObjectItem(item, "host");

        if (!cJSON_IsString(country) || !cJSON_IsString(host))
            continue;

        if (strcmp(country->valuestring, user_country) == 0) {

            printf("Rastas serveris salyje %s: %s\n",
                   user_country, host->valuestring);

            for (int c = 0; c < candidate_count; c++) {

                snprintf(download_url, 512,
                         "http://%s/%s",
                         host->valuestring,
                         candidates[c]);

                printf("  Tikrinamas failas: %s\n", candidates[c]);

                if (is_server_working(download_url)) {

                    snprintf(upload_url, 512,
                             "http://httpbin.org/post");

                    printf("  Pasirinktas failas: %s\n", candidates[c]);

                    cJSON_Delete(json);
                    free(file_data);
                    return;
                }
            }

            printf("  Serveris rastas, bet failai netinka. Bandome kitą.\n");
        }
    }

    printf("Nerasta tinkamu serveriu, naudojamas testinis variantas\n");

    cJSON_Delete(json);
    free(file_data);

fallback:

    snprintf(download_url, 512,
             "http://speedtest.tele2.net/10MB.zip");

    snprintf(upload_url, 512,
             "http://httpbin.org/post");
}


