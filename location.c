#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include "location.h"

// Kadangi libcurl neturi jokio buferio, tai reikia nuspresti kur ir kaip saugoti duomenis
struct Memory {
	char *data; // rodykle i dinamini buferi
	size_t size; // kiek yra sukaupta duomenu
};

// Gauti duomenis is libcurl bibliotekos
// contents rodo i gautus duomenis; size vieno elemento dydis; nmemb elementu dydis; userp rodo i Memory
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
	size_t total_size = size * nmemb;
	struct Memory *mem = (struct Memory *)userp;

	char *ptr = realloc(mem->data, mem->size + total_size + 1);
	if(ptr == NULL) {
		return 0;
	}

	mem->data = ptr;

	// surasome i buferio gala
	memcpy(&(mem->data[mem->size]), contents, total_size);

	mem->size += total_size;
	mem->data[mem->size] = 0;

	return total_size;
}

// Gauti naudotojo esama sali
char* get_user_country() {
	CURL *curl;
	CURLcode res;

	struct Memory chunk;
	chunk.data = malloc(1);
	chunk.size = 0;

	curl = curl_easy_init();
	if(!curl) {
		printf("Curl init nepavyko\n");
		return NULL;
	}

	curl_easy_setopt(curl, CURLOPT_URL, "http://ip-api.com/json/");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk); // leidziamas tik vienas argumentas

	res = curl_easy_perform(curl);

	if(res != CURLE_OK) {
		printf("Prasymas nepavyko: %s\n", curl_easy_strerror(res));
		curl_easy_cleanup(curl);
		return NULL;
	}

	cJSON *json = cJSON_Parse(chunk.data);
	if(!json) {
		printf("JSON klaida\n");
		return NULL;
	}

	cJSON *country = cJSON_GetObjectItem(json, "country");
	if(!cJSON_IsString(country)) {
		printf("Salis nerasta\n");
		cJSON_Delete(json);
		return NULL;
	}

	char *result = strdup(country->valuestring);

	cJSON_Delete(json);
	curl_easy_cleanup(curl);
	free(chunk.data);

	return result;
}
