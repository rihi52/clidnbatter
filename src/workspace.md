Get rid of initiatives table and put initiatives in the participants table.
Use zero for players/anyone the dm wants to do intiative at the table for.
```sql
DROP TABLE initiatives;
DROP TABLE participants;
CREATE TABLE IF NOT EXISTS "participants" (id INTEGER PRIMARY KEY, name TEXT, quantity INTEGER, initiative INTEGER, scenarioid INTEGER);
```