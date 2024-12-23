CREATE TABLE IF NOT EXISTS "scenarios" (id INTEGER PRIMARY KEY, name TEXT);
-- Creates new entry upon scenario creation
-- Deletes entry if exit without saving

CREATE TABLE IF NOT EXISTS "participants" (id INTEGER PRIMARY KEY, name TEXT, quantity INTEGER, scenarioid INTEGER);
-- Creates entry for each participant type upon selection and ties them together with scenario id

CREATE TABLE IF NOT EXISTS "initiatives" (partid INTEGER, initiative INTEGER, initspot INTEGER, scenarioid INTEGER);
-- Creates entry for each entry and ties it together with partid and scenarioid