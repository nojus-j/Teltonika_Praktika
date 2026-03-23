#ifndef SERVER_H
#define SERVER_H

void select_best_server(const char* user_country, char* download_url, char* upload_url);
int find_working_server(char* download_url, char* upload_url);

#endif
