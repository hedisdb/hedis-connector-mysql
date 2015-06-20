#include <stdio.h>
#include <regex.h>
#include "hedis.h"

#define HEDIS_COMMAND_PATTERN ".+"
#define MAX_ERROR_MSG 0x1000

hedisConfigEntry **hedis_entries;
int hedis_entry_count;
char *connector_username;
char *connector_password;
char *connector_host;
char *connector_database;

int init(hedisConfigEntry **entries, int entry_count) {
    hedis_entries = entries;
    hedis_entry_count = entry_count;

    for (int i = 0; i < entry_count; i++) {
        if(!strcasecmp(entries[i]->key, "username")){
            connector_username = malloc(sizeof(char) * entries[i]->value);

            strcpy(connector_username, entries[i]->value);
        }

        if(!strcasecmp(entries[i]->key, "password")){
            connector_password = malloc(sizeof(char) * entries[i]->value);

            strcpy(connector_password, entries[i]->value);
        }

        if(!strcasecmp(entries[i]->key, "host")){
            connector_host = malloc(sizeof(char) * entries[i]->value);

            strcpy(connector_host, entries[i]->value);
        }

        if(!strcasecmp(entries[i]->key, "database")){
            connector_database = malloc(sizeof(char) * entries[i]->value);

            strcpy(connector_database, entries[i]->value);
        }
    }

    return 0;
}

char **parse_hedis_command(const char * to_match) {
    regex_t * r = malloc(sizeof(regex_t));

    int status = regcomp(r, HEDIS_COMMAND_PATTERN, REG_EXTENDED | REG_NEWLINE);

    if (status != 0) {
        char error_message[MAX_ERROR_MSG];

        regerror(status, r, error_message, MAX_ERROR_MSG);

        printf("Regex error compiling '%s': %s\n", HEDIS_COMMAND_PATTERN, error_message);

        return NULL;
    }

    char **str = malloc(sizeof(char *) * 1);

    /* "P" is a pointer into the string which points to the end of the
     *        previous match. */
    const char * p = to_match;
    /* "N_matches" is the maximum number of matches allowed. */
    const int n_matches = 10;
    /* "M" contains the matches found. */
    regmatch_t m[n_matches];

    int i = 0;
    int nomatch = regexec(r, p, n_matches, m, 0);

    if (nomatch) {
        printf("No more matches.\n");

        return NULL;
    }

    for (i = 0; i < n_matches; i++) {
        int start;
        int finish;

        if (m[i].rm_so == -1) {
            break;
        }

        start = m[i].rm_so + (p - to_match);
        finish = m[i].rm_eo + (p - to_match);

        if (i != 0) {
            int size = finish - start;

            str[i - 1] = malloc(sizeof(char) * size);

            sprintf(str[i - 1], "%.*s", size, to_match + start);
        }
    }

    p += m[0].rm_eo;

    return str;
}

char *get_value(const char *str) {
    MYSQL *MySQLConRet;
    MYSQL *MySQLConnection = NULL;

    char *hostname = connector_host;
    char *user = connector_username;
    char password = connector_password;
    char *db = "hedistest";

    MySQLConnection = mysql_init(NULL);

    printf("enter password: ");
    scanf("%s", password);

    MySQLConRet = mysql_real_connect(MySQLConnection, hostname, user, password, db, 0, NULL, 0);

    if (MySQLConRet == NULL) {
        printf("fail\n");

        return 1;
    }

    printf("MySQL Connection Info: %s \n", mysql_get_host_info(MySQLConnection));
    printf("MySQL Client Info: %s \n", mysql_get_client_info());
    printf("MySQL Server Info: %s \n", mysql_get_server_info(MySQLConnection));

    int mysqlStatus = 0;
    MYSQL_RES *mysqlResult = NULL;

    if (mysqlResult) {
        mysql_free_result(mysqlResult);
        mysqlResult = NULL;
    }

    MYSQL_ROW mysqlRow;
    MYSQL_FIELD *mysqlFields;
    my_ulonglong numRows;
    unsigned int numFields;

    char *sqlSelStatement = "select * from user";

    mysqlStatus = mysql_query(MySQLConnection, sqlSelStatement);

    if (mysqlStatus) {
        printf((char *)mysql_error(MySQLConnection));

        return 1;
    }

    mysqlResult = mysql_store_result(MySQLConnection);

    if (mysqlResult) {
        numRows = mysql_num_rows(mysqlResult);

        numFields = mysql_field_count(MySQLConnection);

        numFields = mysql_num_fields(mysqlResult);

        printf("Number of rows=%u  Number of fields=%u \n", numRows, numFields);
    } else {
        printf("Result set is empty\n");
    }

    mysqlFields = mysql_fetch_fields(mysqlResult);

    for (int i = 0; i < numFields; i++) {
        printf("%s\t", mysqlFields[i].name);
    }

    printf("\n");

    while (mysqlRow = mysql_fetch_row(mysqlResult)) {
        for (int i = 0; i < numFields; i++) {
            printf("%s\t", mysqlRow[i] ? mysqlRow[i] : "NULL");
        }

        printf("\n");
    }

    if (mysqlResult) {
        mysql_free_result(mysqlResult);

        mysqlResult = NULL;
    }

    mysql_close(MySQLConnection);

    return value;
}