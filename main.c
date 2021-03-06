#include <stdio.h>
#include <regex.h>
#include <mysql.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
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
            connector_username = malloc(sizeof(char) * (strlen(entries[i]->value) + 1));

            strcpy(connector_username, entries[i]->value);
        }

        if(!strcasecmp(entries[i]->key, "password")){
            connector_password = malloc(sizeof(char) * (strlen(entries[i]->value) + 1));

            strcpy(connector_password, entries[i]->value);
        }

        if(!strcasecmp(entries[i]->key, "host")){
            connector_host = malloc(sizeof(char) * (strlen(entries[i]->value) + 1));

            strcpy(connector_host, entries[i]->value);
        }

        if(!strcasecmp(entries[i]->key, "database")){
            connector_database = malloc(sizeof(char) * (strlen(entries[i]->value) + 1));

            strcpy(connector_database, entries[i]->value);
        }
    }

    return 0;
}

void strip_last_char(char *str) {
    int length = strlen(str);

    memmove(&str[length - 1], &str[length], 1);
}

char *get_value(const char *str) {
    MYSQL *MySQLConRet;
    MYSQL *MySQLConnection = NULL;

    MySQLConnection = mysql_init(NULL);

    MySQLConRet = mysql_real_connect(MySQLConnection, connector_host, connector_username, connector_password, connector_database, 0, NULL, 0);

    if (MySQLConRet == NULL) {
        printf("Connection fail\n");

        return NULL;
    }

    printf("MySQL Connection Info: %s\n", mysql_get_host_info(MySQLConnection));
    printf("MySQL Client Info: %s\n", mysql_get_client_info());
    printf("MySQL Server Info: %s\n", mysql_get_server_info(MySQLConnection));

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

    const char *sqlSelStatement = str;

    mysqlStatus = mysql_query(MySQLConnection, sqlSelStatement);

    if (mysqlStatus) {
        printf("%s\n", (char *)mysql_error(MySQLConnection));

        return NULL;
    }

    mysqlResult = mysql_store_result(MySQLConnection);

    if (mysqlResult) {
        numRows = mysql_num_rows(mysqlResult);

        numFields = mysql_field_count(MySQLConnection);

        numFields = mysql_num_fields(mysqlResult);

        printf("Number of rows=%u  Number of fields=%u\n", numRows, numFields);
    } else {
        printf("Result set is empty\n");

        return NULL;
    }

    mysqlFields = mysql_fetch_fields(mysqlResult);

    for (int i = 0; i < numFields; i++) {
        printf("%s\t", mysqlFields[i].name);
    }

    printf("\n");

    char *value = NULL;

    value = malloc(sizeof(char) * 200);

    strcpy(value, "{\"columns\":{");

    char **columns = NULL;

    columns = malloc(sizeof(char *) * numFields);

    for (int i = 0; i < numFields; i++) {
        columns[i] = malloc(sizeof(char) * 100);

        strcpy(columns[i], "\"");
        strcat(columns[i], mysqlFields[i].name);
        strcat(columns[i], "\":[");
    }

    while (mysqlRow = mysql_fetch_row(mysqlResult)) {
        for (int i = 0; i < numFields; i++) {
            char *column = mysqlRow[i] ? mysqlRow[i] : "NULL";

            strcat(columns[i], "\"");
            strcat(columns[i], column);
            strcat(columns[i], "\",");

            printf("%s\t", column);
        }

        printf("\n");
    }

    for (int i = 0; i < numFields; i++) {
        strip_last_char(columns[i]);

        strcat(value, columns[i]);
        strcat(value, "],");
    }

    strip_last_char(value);

    strcat(value, "}}");

    if (mysqlResult) {
        mysql_free_result(mysqlResult);

        mysqlResult = NULL;
    }

    mysql_close(MySQLConnection);

    for (int i = 0; i < numFields; i++) {
        free(columns[i]);
    }

    free(columns);

    return value;
}