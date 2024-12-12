1   ```C 
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(pMonsterDb));
    ```
2   ```C
        fprintf(stderr, "Failed to bind name: %s\n", sqlite3_errmsg(pMonsterDb)); 
    ```
3   ```C 
        fprintf(stderr, "Failed to bind AC: %s\n", sqlite3_errmsg(pMonsterDb));
    ```
4   ```C
        fprintf(stderr, "Failed to bind HP: %s\n", sqlite3_errmsg(pMonsterDb));
    ```
5   ```C
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(pMonsterDb));
    ```
6   ```c
        static int CliDC_Combat_ChoosePlayers()
    ```