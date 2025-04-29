Get rid of initiatives table and put initiatives in the participants table.
Use zero for players/anyone the dm wants to do intiative at the table for.
```sql
DROP TABLE initiatives;
DROP TABLE participants;
CREATE TABLE IF NOT EXISTS "participants" (id INTEGER PRIMARY KEY, name TEXT, quantity INTEGER, initiative INTEGER, scenarioid INTEGER);
```

```c
int giCliDC_Combat_ReadsNameAndAsksInitiativeAssignsStruct()
{
    memset(namePlayers, '\0', sizeof(namePlayers));
    int nameIndex = 0;

    /* Read the inputted players into players[] one at a time */
    for (int i = startPosition; i <= length; i++)
    {
        if (players[i] != ',' && players[i] != '\n')
        {
            if (nameIndex < CHARACTER_BUFFER)
            {
                namePlayers[nameIndex] = players[i];
                nameIndex++;
            }
        }
        else
        {
            endchar = players[i];
            startPosition = i + 1;
            break;
        }
    }
    /* Null terminate player's name */
    namePlayers[nameIndex] = '\0';

    /* If there is no name do not attempt to create a player struct and restart loop */
    if ('\0' != namePlayers[0])
    {
        newPlayer = gvCliDC_Combat_CreatePlayer(namePlayers);
    }
    else
    {
        loop = 0;
        continue;
    }

    if (newPlayer == NULL)
    {
        printf("Please re-enter players' names or enter 'x' to return to home\n\n");
        loop = 0;
        continue;
    }

    gvCliDC_Combat_SetInitiative(newPlayer);

    if (endchar == '\n')
    {
        loop = 2;
        break;
    }

            /* return either 0, 1, or 2 for loop instructions. everything else is either passed through or global */
}
```