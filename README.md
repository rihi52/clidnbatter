# clidnbatter (Command Line Interface DNd comBAT TrackER)

A command line tool used to run combat scenarios, designed for 5th edition.

## Features

### Combat Tracker

- Initiative Tracker
- Add and save custom players - _Cannot edit saved players yet_
  - Holds name, AC, and HP for players
- Add up to 50 monsters of the same type - _Not currently enforced_
- Add as many different types of monsters as needed, pulled from an included sqlite3 db containing basic info on all monsters in the 5E SRD
- Displays Initiative spot, Name, HP, and AC
  - Can track HP of all combatants

### Lookup Tool

- A sqlite database containing the below stats of all monsters in the 5E SRD:
  - Name
  - Type
  - CR
  - AC
- Monsters can be searched and displayed by any of the above stats
- Custom monsters can be imported to the db via sqlite or using the cli interface of the program itself

## Screenshots (Out of Date)

### Combat Tracking

![image](https://github.com/rihi52/clidnbatter/blob/main/assets/startcombat.png)

![image](https://github.com/rihi52/clidnbatter/blob/main/assets/monstercombat.png)

![image](https://github.com/rihi52/clidnbatter/blob/main/assets/combatorder.png)

![image](https://github.com/rihi52/clidnbatter/blob/main/assets/afterturn.png)

### Looking Up

![image](https://github.com/rihi52/clidnbatter/blob/main/assets/lookup.png)

</br>
</br>
</br>
This work includes material taken from the System Reference Document 5.1 (“SRD 5.1”) by Wizards of the Coast LLC and available at https://dnd.wizards.com/resources/systems-reference-document. The SRD 5.1 is licensed under the Creative Commons Attribution 4.0 International License available at https://creativecommons.org/licenses/by/4.0/legalcode.
