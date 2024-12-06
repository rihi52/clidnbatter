#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"

int main(void)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    char *errmsg;

    rc = sqlite3_open("monsters.db", &db);
    if (rc != SQLITE_OK){
        printf("ERROR opening SQLite DB in memory: %s\n", sqlite3_errmsg(db));
        goto out;
    }

     printf("opened SQLite handle successfully.\n");

    /* use the database... */
    sqlite3_prepare(db, "SELECT * FROM monsters", -1, &stmt, NULL);

    while (sqlite3_step(stmt) != SQLITE_DONE) {
		int i;
		int num_cols = sqlite3_column_count(stmt);
		
		for (i = 0; i < num_cols; i++)
		{
			switch (sqlite3_column_type(stmt, i))
			{
			case (SQLITE3_TEXT):
				printf("%s, ", sqlite3_column_text(stmt, i));
				break;
			case (SQLITE_INTEGER):
				printf("%d, ", sqlite3_column_int(stmt, i));
				break;
			case (SQLITE_FLOAT):
				printf("%g, ", sqlite3_column_double(stmt, i));
				break;
			default:
				break;
			}
		}
		printf("\n");

	}

out:
    /*
     * close SQLite database
     */
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    printf("database closed.\n");
}