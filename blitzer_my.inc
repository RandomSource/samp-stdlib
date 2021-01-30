////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Credits:
// Script erstellt von Stas
// Offizieller Scripter der Xtreme - RolePlay - Community
// www.xtreme-roleplay.eu
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Einstellungen
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <streamer>
#define MAX_BLITZER 50 // Maximale Anzahl von Blitzern
#define BLITZER_TIMER_INTERVALL 300 // Intervall für das Blitzen (in Milliesekunden)
#define BLITZER_PAUSE 0 // Anzahl der Minuten, in denen der Spieler nicht mehr geblitzt werden kann

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// globale Variablen
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum eBlitzer {
	bID, Float:BlitzerX, Float:BlitzerY, Float:BlitzerZ, Radius, Geschwindigkeit, Erstellt
};
new Blitzer[MAX_BLITZER][eBlitzer];



stock Float:GetDistanceBetweenPoints(Float:X, Float:Y, Float:Z, Float:PointX, Float:PointY, Float:PointZ) return floatsqroot(floatadd(floatadd(floatpower(floatsub(X, PointX), 2.0), floatpower(floatsub(Y, PointY), 2.0)), floatpower(floatsub(Z, PointZ), 2.0)));

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CallBacks
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
forward Blitzer_Timer();
public Blitzer_Timer() {
    new save, strafe, bspeed;
 	for (new playerid = 0; playerid < MAX_PLAYERS; playerid++) {
		if(!IsPlayerConnected(playerid))continue;
		if(!IsPlayerInAnyVehicle(playerid)) continue;
		if(GetPlayerState(playerid)!=PLAYER_STATE_DRIVER) continue;
 	    save = GetPVarInt(playerid, "BlitzPause");
		if(save > 0) {
			save--;
			SetPVarInt(playerid, "BlitzPause", save);
			continue;
		} 		
		for(new i = 0; i < sizeof(Blitzer); i++) {
			if(Blitzer[i][Erstellt] == 1) {
			    if(IsInBlitzerRange(playerid, Blitzer[i][Radius], Blitzer[i][BlitzerX], Blitzer[i][BlitzerY], Blitzer[i][BlitzerZ],Blitzer[i][BlitzerZ]+1.8)) {
					bspeed = ErmittleGeschwindigkeit(playerid,true);
					if(bspeed > Blitzer[i][Geschwindigkeit]) {
						new sstring[128];
						// Einstellen des Momentes, in dem der Spieler kein weiteres mal geblitzt werden kann
						SetPVarInt(playerid, "BlitzPause", (BLITZER_TIMER_INTERVALL * BLITZER_PAUSE * 60) / 1000);
						// Strafe für Spieler
						strafe = (bspeed*2) - Blitzer[i][Geschwindigkeit];
						PlayerPlaySound(playerid, 1132, 0.0, 0.0, 0.0);
					   	GivePlayerMoney(playerid, -strafe);
					   	new name[MAX_PLAYER_NAME];
					   	if(GetPVarInt(playerid,"maske")==0){
                                                   GetPlayerName(playerid,name,sizeof(name));
                                                }else{
                                                name="Unbekannter";
					   	}
					   	for(new j=0; j<GetMaxPlayers(); j++)
					   	{
					   	        if(IsPlayerInFrakt(j,1)||IsPlayerInFrakt(j,11)||IsPlayerAnAdmin(j,6))
					   	        {
                                                             format(sstring,sizeof(sstring),"HQ: %s wurde mit %d km/h geblitzt.",name,bspeed);
                                                             SendClientMessage(j,0x2a77a1AA,sstring);
					   	        }
					   	}
						// Text für Spieler
						SendClientMessage(playerid, 0xFF6347AA,"Du wurdest wegen zu hoher Geschwindigkeit geblitzt!");
						format(sstring,sizeof(sstring), "Du bist %d km/h gefahren.", bspeed);
						SendClientMessage(playerid, 0xFF6347AA, sstring);
						if(isPlayerInSchulung(playerid)==1)
						{
							 OnPlayerExitVehicle(playerid,GetPlayerVehicleID(playerid));
							 RemovePlayerFromVehicle(playerid);
						}

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
stock IsInBlitzerRange(playerid,Float:R,Float:X,Float:Y,Float:Z,Float:MAX_Z)
{
	new const Float:Winkel = 60.0;
	new const Float:Weite = 30.0;
	new Float:Pos[8];
	GetPlayerPos(playerid,Pos[0],Pos[1],Pos[2]);
	Pos[4] = Y+(Weite*floatsin(Winkel+R,degrees));
	Pos[5] = Y-(Weite*floatsin(R-Winkel,degrees));
	Pos[6] = X+(Weite*floatcos(Winkel+R,degrees));
	Pos[7] = X-(Weite*floatcos(R-Winkel,degrees));
	return (((Y-Pos[4])*(Pos[0]-X) + (Pos[6]-X)*(Pos[1]-Y)) <0 ||((Pos[4]-Pos[5])*(Pos[0]-Pos[6]) + (Pos[7]-Pos[6])*(Pos[1]-Pos[4])) <0 ||((Pos[5]-Y)*(Pos[0]-Pos[7]) + (X-Pos[7])*(Pos[1]-Pos[5])) <0 || Pos[2] > MAX_Z || Pos[2] < Z) ? 0:1;
}
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
		    Blitzer[i][bID] = CreateDynamicObject(18880, x-1,y + 1.0, z-0.8, 0.0, 0.0, 10,-1,-1,-1,200.0);
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



stock ErmittleGeschwindigkeitWithoutZ(playerid) {
    new bool:kmh=true;
    new Float:x,Float:y,Float:z,Float:rtn;
    if(IsPlayerInAnyVehicle(playerid)) GetVehicleVelocity(GetPlayerVehicleID(playerid),x,y,z); else GetPlayerVelocity(playerid,x,y,z);
    rtn = floatsqroot(x*x+y*y);
    return kmh?floatround(rtn * 100 * 1.61):floatround(rtn * 100);
}

stock ErmittleGeschwindigkeit(playerid,bool:kmh) {
    new Float:x,Float:y,Float:z,Float:rtn;
    if(IsPlayerInAnyVehicle(playerid)) GetVehicleVelocity(GetPlayerVehicleID(playerid),x,y,z); else GetPlayerVelocity(playerid,x,y,z);
    rtn = floatsqroot(x*x+y*y+z*z);
    return kmh?floatround(rtn * 100 * 1.61):floatround(rtn * 100);
}

