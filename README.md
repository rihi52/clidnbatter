# clidnbatter (Command Line Interface DNd comBAT TrackER)

A command line tool used to run combat scenarios, which includes a database with a table of SRD monsters and
a table for player management, designed for 5th edition.

## Features

### Combat Tracker

- Initiative Tracker
- Add and save custom players
  - Holds name, AC, and HP for players
- Add up to 50 monsters of the same type - _Not currently enforced_
- Add as many different types of monsters as needed, pulled from an included sqlite3 db containing basic info on all monsters in the 5E SRD
- Displays Initiative spot, Name, HP, and AC
  - Can track HP of all combatants

### Lookup Tool

- A sqlite database containing the below stats of all monsters in the 5E SRD:
  - Name
  - Type
  - size
  - CR
  - AC
- Monsters can be searched and displayed by any of the above stats
- Custom monsters can be imported to the db via sqlite or using the cli interface of the program itself

## Future

 - Combat menu for scenario set up options before running scenario
 - Save scenarios in database
 - Add custom monster option
 - EXP values/total in combat tracker

## Far Future

- Option to display full SRD statblock of individual monster
- Package manager
  
## Install

For windows, download clidnbatter.zip and place clidnbatter.exe and monsters.db inside in the same directory. If they .exe and .db are not in the same location the program will not be able to find the database and therefore will not work.

If you wish to build yourself on linux systems, you will need the sqlite dev files (ubuntu shown below):
```shell
sudo apt install libsqlite3-dev
```
Clone the repository and
```shell
cd clidnbatter
make
```
Run from project directory with
```shell
./bin/clidnbatter
```

## Screenshots

### Home Menu

![image](https://github.com/rihi52/clidnbatter/blob/main/assets/homemenu.png)

### Combat Tracking

![image](https://github.com/rihi52/clidnbatter/blob/main/assets/playerinfo.png)

![image](https://github.com/rihi52/clidnbatter/blob/main/assets/enemyinfo.png)

![image](https://github.com/rihi52/clidnbatter/blob/main/assets/combatmenu.png)

### Look Up Menu

![image](https://github.com/rihi52/clidnbatter/blob/main/assets/lookupmenu.png)

![image](https://github.com/rihi52/clidnbatter/blob/main/assets/crlookup.png)

### Modify Player Database

![image](https://github.com/rihi52/clidnbatter/blob/main/assets/modifymenu.png)

</br>
</br>
</br>
This work includes material taken from the System Reference Document 5.1 (“SRD 5.1”) by Wizards of the Coast LLC and available at https://dnd.wizards.com/resources/systems-reference-document. The SRD 5.1 is licensed under the Creative Commons Attribution 4.0 International License available at https://creativecommons.org/licenses/by/4.0/legalcode.
