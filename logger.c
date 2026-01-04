#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curl/curl.h>
#include <getopt.h>

void print_help() {
    printf("Usage: kaylog [options]\n\n"
           "Options:\n"
           "  -c, --csv <file>           CSV file path\n"
           "  -k, --webhook <url>        Discord webhook URL\n"
           "  -u, --user <name>          Name of the User\n"
           "  -s, --substance <name>     Substance name\n"
           "  -a, --salt <salt>          Salt, if known\n"
           "  -d, --dosage <amount>      Dosage value\n"
           "  -r, --route <roa>          Route of administration\n"
           "  -i, --site <site>          Site of administration\n"
           "  -n, --note <note>          Notes if any note worthy\n"
           "  -h, --help                 Show this help text\n"
           " Made By Kayrite :) 	 version 1.1\n" );
}

char *timestamp_now() {
    static char buffer[64];
    struct timespec ts;
    
    timespec_get(&ts, TIME_UTC);

    struct tm tm;
    gmtime_r(&ts.tv_sec, &tm);

    snprintf(buffer, sizeof(buffer),
             "%04d-%02d-%02dT%02d:%02d:%02d.%03ldZ",
             tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
             tm.tm_hour, tm.tm_min, tm.tm_sec,
             ts.tv_nsec / 1000000);

    return buffer;
}

void send_webhook(const char *url, const char *content) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Error: Could not initialise curl.\n");
        return;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);

    char json[4096];
    snprintf(json, sizeof(json), "{\"content\": \"%s\"}", content);

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER,
                     curl_slist_append(NULL, "Content-Type: application/json"));

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
        fprintf(stderr, "Webhook error: %s\n", curl_easy_strerror(res));

    curl_easy_cleanup(curl);
}

int main(int argc, char *argv[]) {
    char *csv_path = NULL;
    char *webhook = NULL;
    char *user = NULL;
    char *substance = NULL;
    char *salt = NULL;
    char *dosage = NULL;
    char *route = NULL;
    char *site = NULL;
    char *note = NULL;

    static struct option long_options[] = {
        {"csv", required_argument, 0, 'c'},
        {"webhook", required_argument, 0, 'k'},
        {"user", required_argument, 0, 'u'},
        {"substance", required_argument, 0, 's'},
        {"salt", required_argument, 0, 'a'},
        {"dosage", required_argument, 0, 'd'},
        {"route", required_argument, 0, 'r'},
        {"site", required_argument, 0, 'i'},
        {"note", required_argument, 0, 'n'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    int opt, option_index = 0;

    while ((opt = getopt_long(argc, argv, "c:k:u:s:a:d:r:i:n:h",
                              long_options, &option_index)) != -1) {
        switch (opt) {
            case 'c': csv_path = optarg; break;
            case 'k': webhook = optarg; break;
            case 'u': user = optarg; break;
            case 's': substance = optarg; break;
            case 'a': salt = optarg; break;
            case 'd': dosage = optarg; break;
            case 'r': route = optarg; break;
            case 'i': site = optarg; break;
            case 'n': note = optarg; break;
            case 'h': print_help(); return 0;
            default: print_help(); return 1;
        }
    }

    if (!csv_path || !user || !substance || !dosage || !route)
        return 0;

    char *ts = timestamp_now();

    char line[512] = "";
    snprintf(line, sizeof(line), "%s,%s,%s,%s,%s,", ts, user, substance, dosage, route);
    if (site) strcat(line, site), strcat(line, " ");
    if (salt) {
        strcat(line, salt);
        strcat(line, "-salt");
    }

    char logline[1024] = "";
    snprintf(logline, sizeof(logline), "%s: %s %s", user, dosage, substance);
    if (salt) {
        strcat(logline, " [");
        strcat(logline, salt);
        strcat(logline, "]");
    }
    strcat(logline, " via ");
    strcat(logline, route);
    if (site) {
        strcat(logline, " at ");
        strcat(logline, site);
        strcat(logline, " site");
    }
    if (note) {
        strcat(logline, " ");
        strcat(logline, note);
    }

    FILE *f = fopen(csv_path, "a");
    if (f) {
        fprintf(f, "%s\n", line);
        fclose(f);
    }

    printf("%s\n", logline);
    printf("%s: %s\n", csv_path, line);

    if (webhook)
        send_webhook(webhook, logline);

    return 0;
}
