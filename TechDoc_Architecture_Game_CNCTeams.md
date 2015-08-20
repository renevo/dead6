[Back](TechDoc_Architecture.md)

# Overview #
Teams are handled with special care in Command & Conquer Mode game rules. A team runs as much more than an integer value attached to all players and buildings. Instead, teams have their own virtual identity running along side the game, with defined attributes and special scripts used to handle team behavior. Each team has its own XML file which identifies that team and sets its Purchase Menu content. Each team also has a Lua script that handles team behavior.

# [Script Functionality](TechDoc_Architecture_Game_CNCTeams_Functionality.md) #

# Team XML File #
The Team XML file is the blueprint for a team. It defines the team's attributes and is the lookup book used by the Dead 6 Core to create the team at the beginning. Loaded from the map's [CNCRules.xml](TechDoc_Architecture_Game_CNCRules.md) file, the team's XML file is located in the Teams\XML folder. (Game\Scripts\Teams\XML)

This file is only utilized if the team is defined in element (non-inline) form in the CNCRules.xml fil.

## Sample XML File ##
This is an example of a typical team XML file:
```
<?xml version="1.0" encoding="utf-8"?>
<Teams>
  <Team Name="GDI" LongName="Global Defense Initiative" Script="CNCTeam.lua" IsPlayerTeam="1">
    <Harvester Entity="Harvester" Capacity="300.0" BuildTime="10.0" LoadRate="10.0" UnloadRate="20.0" />
    <Purchases>
      <Group Name="Soldiers">
        <Purchase Name="Engineer" Description="Buy an engineer!" Icon="Soldiers/Engineer.tga" Cost="250" Inflation="250">
          <Prerequisites>
            <Prerequisite Enabled="1" Inflation="1">SoldierFactory</Prerequisite>
            <Prerequisite Enabled="0" Inflation="1">PowerPlant</Prerequisite>
          </Prerequisites>
          <Character Name="Engineer" />
        </Purchase>
      </Group>
    </Purchases>
  </Team>
</Teams>
```

The Team tag's attributes are as such:
  * Name - The short-hand name of the team.
  * LongName - The long-hand name of the team.
  * Script - The Lua script inherited and used by this team for team behavior controlling.
  * IsPlayerTeam - Boolean flag used to determine if a player can join this team or not.

The contained Harvester tag is used to define the Team's Harvester properties, which are inherited by all Harvesters created on the team. It's attributes are defined as such:
  * Entity - The entity to use for the Harvester
  * Capacity - How much tiberium the Harvester can hold in its tank
  * BuildTime - How long it takes the Harvester to be rebuilt once destroyed, in seconds
  * LoadRate - How long it takes the Harvester to load its tank with tiberium, in one part of tiberium per second. (A capacity of 300 with a load rate of 1 would take 300 seconds to fully load)
  * UnloadRate - How long it takes the Harvester to unload its tank, in one part of tiberium per second.

The contained Purchases tag is used to define the Team's Purchases for their Purchase Menu. This is explained further down.


# Team LUA File #
Each team has the ability to run a Lua script along side it to handle team behaviors. Special callbacks are made to it to control when players join/leave the team, when team members speak in public chat, when team-controlled buildings, vehicles and soldiers are damaged and killed, etc.


# Team Purchases #
Defined in the team's XML file are its purchase listings. These listings are used by the team to fill their Purchase Menu. Players are able to trade in the credits they earn during gameplay for new character classes, weapons and vehicles which they can utilize to pwn more Nod noobies into submission.

Purchases are divided up into **Groups** which are given a name. Within each group, many **Purchase** fields are listed to describe new purchase entries. Each field can have the following defined for it:
  * Name - The name of the purchase entry, as it is shown in the Purchase Menu
  * Description - The description of the entry
  * Icon - Purchase icon, used in the Purchase Menu to represent this entry
  * Cost - Base credit value for entry
  * Inflation - Increase from base credit value when inflation occurs due to loss of power
  * Prerequisites - List of buildings that affect this purchase entry
  * Purchase Types - Type definitions that describe what the player gets when they buy this

An entry's _Prerequisites_ are used to link this entry to the status of buildings. Each entry is defined using a **Prerequisite** tag, and the building class name is defined as the element of the tag. The following attributes can be used to describe how the Prerequisite works with the purchase entry:
  * Enabled - Set to '1' if the item can no longer be purchased once this building has been destroyed
  * Inflation - Set to '1' if the item enters inflation stage once this building loses power

An entry must also define its Purchase Type(s). Each entry can have more than one type. Each type has its own properties which are laid out [in the Purchase Types section](TechDoc_Architecture_Game_PurchaseTypes.md). The following are valid purchase types. To include one, simply create a new child tag with the following name:
  * Character - This entry turns the player into the given character class.
  * Vehicle - This entry creates a new vehicle through the team using its vehicle spawn points.
  * Weapon - This entry gives a new weapon to the player
  * Refill - This entry gives the player health, armor and/or specific ammo counts.


[Back](TechDoc_Architecture.md)