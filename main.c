#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "location.h"
#include "server.h"
#include "net.h"


int main(int argc, char *argv[]) {

	int opt;
	int auto_mode = 0, download_flag = 0, upload_flag = 0, location_flag = 0;
	char server_host[1024] = {0};

	FILE *logf = fopen("speedtest.log", "a");
	if(!logf) {
		printf("Nepavyko sukurti log failo\n");
	}

	while ((opt = getopt(argc, argv, "adus:l")) != -1) {
		switch (opt) {
			case 'a': auto_mode = 1; break;
			case 'd': download_flag = 1; break;
			case 'u': upload_flag = 1; break;
			case 'l': location_flag = 1; break;
			case 's': strcpy(server_host, optarg); break;
			default:
				printf("Naudjimas: -a -d -u -l -s <server>\n");
				return 1;
		}
	}

	char *country = NULL;
	char download_url[1024], upload_url[1024];

	if (auto_mode || location_flag) {
		printf("Nustatoma vieta\n");
		country = get_user_country();

		if (!country) {
			printf("Nepavyko rasti vietoves\n");
			return 1;
		}
		printf("Vietove: %s\n", country);
	}

	if (auto_mode) {
		printf("Ieskomas geriausias serveris pagal vietove...\n");

		select_best_server(country, download_url, upload_url);

		printf("Pasirinktas serveris:\n");
		printf("Download URL: %s\n", download_url);
		printf("Upload URL: %s\n", upload_url);
	}

	double download_speed = 0, upload_speed = 0;

	if (download_flag && !auto_mode) {
        if (strlen(server_host) == 0) {
            printf("Download režimui reikia nurodyti serverį:\n");
            printf("Pvz.: ./speedtest -d -s speedtest.tele2.net\n");
            return 1;
        }

        const char* files[] = {
		"100MB.zip",
		"10MB.zip",
		"1MB.zip"
		};

		for (int i = 0; i < 3; i++) {
			snprintf(download_url, sizeof(download_url),
				"http://%s/%s", server_host, files[i]);

			if (is_server_working(download_url)) {
				break;
			}
		}
		printf("Atliekamas atsiuntimo testas...\n");
		download_speed = test_download(download_url);
	}
	
	if (auto_mode) {
        printf("Atliekamas atsiuntimo testas...\n");
        download_speed = test_download(download_url);

        printf("Atliekamas siuntimo testas...\n");
        upload_speed = test_upload(upload_url);
    }

    if (upload_flag && !auto_mode) {
        if (strlen(server_host) == 0) {
            printf("Upload režimui reikia nurodyti serverį:\n");
            printf("Pvz.: ./speedtest -u -s speedtest.tele2.net\n");
            return 1;
        }

        snprintf(upload_url, sizeof(upload_url),
                 "http://httpbin.org/post");
				 
		printf("Atliekamas siuntimo testas...\n");
		upload_speed = test_upload(upload_url);
    }

	printf("\n---------- gauti rezultatai ----------\n");

	printf("Naudotojo vietove: %s\n", country ? country : "Nerasta");
	printf("Serverio atsiuntimo url: %s\n", download_url);
	printf("Serverio siuntimo url: %s\n", upload_url);

	printf("Atsiuntimo greitis: %.2f Mbps\n", download_speed);
	printf("Siuntimo greitis: %.2f Mbps\n", upload_speed);
	
	if (logf) {
    fprintf(logf, "----- SPEEDTEST LOG -----\n");
    fprintf(logf, "Vietove: %s\n", country ? country : "Nerasta");
    fprintf(logf, "Download URL: %s\n", download_url);
    fprintf(logf, "Upload URL: %s\n", upload_url);
    fprintf(logf, "Atsiuntimo greitis: %.2f Mbps\n", download_speed);
    fprintf(logf, "Siuntimo greitis: %.2f Mbps\n", upload_speed);
    fprintf(logf, "--------------------------\n\n");
    fclose(logf);
	}	

	return 0 ;
}
