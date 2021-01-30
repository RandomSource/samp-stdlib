////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Credits:
// Script erstellt von Stas
// Offizieller Scripter der Xtreme - RolePlay - Community
// www.xtreme-roleplay.eu
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Einstellungen
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <streamer>
#define MAX_BLITZER 15 // Maximale Anzahl von Blitzern
#define BLITZER_TIMER_INTERVALL 500 // Intervall für das Blitzen (in Milliesekunden)
#define BLITZER_PAUSE 1 // Anzahl der Minuten, in denen der Spieler nicht mehr geblitzt werden kann

#define COLOR_BLUE 0x2a77a1
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// globale Variablen
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum eBlitzer {
	bID, Float:BlitzerX, Float:BlitzerY, Float:BlitzerZ, Radius, Geschwindigkeit, Erstellt
};
new Blitzer[MAX_BLITZER][eBlitzer];
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CallBacks
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
forward Blitzer_Timer();
public Blitzer_Timer() {
    new save, strafe, bspeed;
 	for (new playerid = 0; playerid < MAX_PLAYERS; playerid++) {
 	    save = GetPVarInt(playerid, "BlitzPause");
		if(save > 0) {
			save--;
			SetPVarInt(playerid, "BlitzPause", save);
			continue;
		}
 		if(!IsPlayerInAnyVehicle(playerid) || IsPlayerNPC(playerid)) continue;
 		if(GetPlayerState(playerid)!=PLAYER_STATE_DRIVER) continue;
		for(new i = 0; i < sizeof(Blitzer); i++) {
			if(Blitzer[i][Erstellt] == 1) {
			    if(IsPlayerInRangeOfPoint(playerid, Blitzer[i][Radius], Blitzer[i][BlitzerX], Blitzer[i][BlitzerY], Blitzer[i][BlitzerZ])) {
					bspeed = ErmittleGeschwindigkeit(playerid,true);
					if(bspeed > Blitzer[i][Geschwindigkeit]) {
						new string[128];
						// Einstellen des Momentes, in dem der Spieler kein weiteres mal geblitzt werden kann
						SetPVarInt(playerid, "BlitzPause", (BLITZER_TIMER_INTERVALL * BLITZER_PAUSE * 60) / 1000);
						// Strafe für Spieler
						strafe = (bspeed*2) - Blitzer[i][Geschwindigkeit];
						PlayerPlaySound(playerid, 1132, 0.0, 0.0, 0.0);
					   	GivePlayerMoney(playerid, -strafe);
					   	for(new j=0; j<GetMaxPlayers(); j++)
					   	{
					   	        if(IsPlayerInFrakt(j,1)||IsPlayerInFrakt(j,11))
					   	        {
                                                             new name[MAX_PLAYER_NAME];
                                                             GetPlayerName(playerid,name,sizeof(name));
                                                             format(string,sizeof(string),"HQ: %s wurde mit %d km/h geblitzt.",name,bspeed);
                                                             SendClientMessage(j,COLOR_BLUE,string);
					   	        }
					   	}
						// Text für Spieler
						SendClientMessage(playerid, 0xFF6347AA,"Du wurdest wegen zu hoher Geschwindigkeit geblitzt!");
						format(string,sizeof(string), "Du bist %d km/h gefahren.", bspeed);
						SendClientMessage(playerid, 0xFF6347AA, string);
					}
				}
			}
		}
	}
	return 1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Funktionen
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
stock AddBlitzer(playerid,bspeed,radius) {
	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid,x,y,z);
	for(new i = 0; i < sizeof(Blitzer); i++) {
  	    if(Blitzer[i][Erstellt] == 0) {
			Blitzer[i][BlitzerX] = x + 1.0;
		    Blitzer[i][BlitzerY] = y + 1.0;
		    Blitzer[i][BlitzerZ] = z;
		    Blitzer[i][Geschwindigkeit] = bspeed;
		    Blitzer[i][Radius] = radius;
		    Blitzer[i][Erstellt] = 1;
		    Blitzer[i][bID] = CreateDynamicObject(2600, x + 1.0, y + 1.0, z, 0.0, 0.0, 10);
		    return 1;
		}
	}
	return 0;
}
stock RemoveBlitzer(playerid) {
   	for(new i = 0; i < sizeof(Blitzer); i++) {
	   if(Blitzer[i][Erstellt] == 1) {
			if(IsPlayerInRangeOfPoint(playerid, Blitzer[i][Radius], Blitzer[i][BlitzerX], Blitzer[i][BlitzerY], Blitzer[i][BlitzerZ])) {
				Blitzer[i][Erstellt] = 0;
    			Blitzer[i][BlitzerX] = 0.0;
		        Blitzer[i][BlitzerY] = 0.0;
	         	Blitzer[i][BlitzerZ] = 0.0;
           		DestroyDynamicObject(Blitzer[i][bID]);
             	return 1;
  	        }
  	    }
  	}
   	return 0;
}
stock RemoveAllBlitzer() {
   	for(new i = 0; i < sizeof(Blitzer); i++) {
		if(Blitzer[i][Erstellt] == 1) {
			Blitzer[i][Erstellt] = 0;
			Blitzer[i][BlitzerX] = 0.0;
   			Blitzer[i][BlitzerY] = 0.0;
     		Blitzer[i][BlitzerZ] = 0.0;
    		DestroyDynamicObject(Blitzer[i][bID]);
  	    }
  	}
   	return 0;
}
stock CountBlitzer() {
   	new anzahl = 0;
	for(new i = 0; i < sizeof(Blitzer); i++) {
		if(Blitzer[i][Erstellt] == 1) anzahl++;
  	}
   	return anzahl;
}
stock ErmittleGeschwindigkeit(playerid,bool:kmh) {
    new Float:x,Float:y,Float:z,Float:rtn;
    if(IsPlayerInAnyVehicle(playerid)) GetVehicleVelocity(GetPlayerVehicleID(playerid),x,y,z); else GetPlayerVelocity(playerid,x,y,z);
    rtn = floatsqroot(x*x+y*y+z*z);
    return kmh?floatround(rtn * 100 * 1.61):floatround(rtn * 100);
}

