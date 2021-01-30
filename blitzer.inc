////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Credits:
// Script erstellt von Stas
// Offizieller Scripter der Xtreme - RolePlay - Community
// www.xtreme-roleplay.eu
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Einstellungen
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_BLITZER 5 // Maximale Anzahl von Blitzern
#define BLITZER_TIMER_INTERVALL 500 // Intervall für das Blitzen (in Milliesekunden)
#define BLITZER_PAUSE 2 // Anzahl der Minuten, in denen der Spieler nicht mehr geblitzt werden kann
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// globale Variablen
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum eBlitzer {
	ID,Erstellt
};
new Blitzer[MAX_BLITZER][eBlitzer];

enum blitz {
	Float:bbX,
	Float:bbY,
	Float:bbZ,
	Float:bbrZ,
	geschwindigkeit,
	Radios
};

new blitzernew[MAX_BLITZER][blitz] = {
{1352.1973,-1232.6274,11.3946,0.00,90,50},
{1070.8563,-937.6418,41.9385,90.00,90,20},
{369.8865,-1708.5394,5.1802,90.00,90,30},
{1482.1112,-966.6192,32.3406,-90.00,90,10},
{1484.8876,-945.5833,32.7087,90.00,90,10}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CallBacks
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
forward Blitzer_Timer();
public Blitzer_Timer() {
    new save, strafe;
 	for (new playerid = 0; playerid < MAX_PLAYERS; playerid++) {
 	    save = GetPVarInt(playerid, "BlitzPause");
		if(save > 0) {
			save--;
			SetPVarInt(playerid, "BlitzPause", save);
			continue;
		}
 		if(!IsPlayerInAnyVehicle(playerid) || IsPlayerNPC(playerid)) continue;
		for(new i = 0; i < sizeof(blitzernew); i++) {
			new playerState = GetPlayerState(playerid);
		    if(playerState == PLAYER_STATE_DRIVER)
		    {
			//if(Blitzer[i][Erstellt] == 1) {
			    if(IsPlayerInRangeOfPoint(playerid, blitzernew[i][Radios], blitzernew[i][bbX], blitzernew[i][bbY], blitzernew[i][bbZ])) {
					new speed = ErmittleGeschwindigkeit(playerid,true);
					if(speed > blitzernew[i][geschwindigkeit]) {
						new string[128];
						// Einstellen des Momentes, in dem der Spieler kein weiteres mal geblitzt werden kann
						SetPVarInt(playerid, "BlitzPause", (BLITZER_TIMER_INTERVALL * BLITZER_PAUSE * 60) / 1000);
						// Strafe für Spieler
						strafe = (speed*2) - blitzernew[i][geschwindigkeit];
						PlayerPlaySound(playerid, 1132, 0.0, 0.0, 0.0);
					   	GivePlayerMoney(playerid, -strafe);
						// Text für Spieler
						SendClientMessage(playerid, 0xFF6347AA,"Du wurdest wegen zu hoher Geschwindigkeit geblitzt!");
						format(string,sizeof(string), "Du bist %d km/h gefahren und musst $%d Bußgeld bezahlen.", speed, strafe);
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
stock BlitzerCreate()
{
	for(new i = 0; i < sizeof(blitzernew); i++) {
	Blitzer[i][ID] = CreateObject(18880,blitzernew[i][bbX],blitzernew[i][bbY],blitzernew[i][bbZ],0,0,blitzernew[i][bbrZ]); //CreateBlitzer
	}
	CreateObject(18880,369.8618,-1710.9755,5.2199,0.0,0.0,270.00); //CreateBlitzer
	CreateObject(18880,1347.8944,-1231.2930,11.3950,0.0,0.0,180.00); //CreateBlitzer
	CreateObject(18880,376.7186,-1712.6194,23.2960,0.0,0.0,0.0); //CreateBlitzerpeer
	CreateObject(18880,363.5103,-1712.4454,23.3459,0.0,0.0,180.00); //CreateBlitzerpeer1
}
/*stock AddBlitzer(playerid,speed,radius) {
	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid,x,y,z);
	for(new i = 0; i < sizeof(Blitzer); i++) {
  	    if(Blitzer[i][Erstellt] == 0) {
			Blitzer[i][BlitzerX] = x + 1.0;
		    Blitzer[i][BlitzerY] = y + 1.0;
		    Blitzer[i][BlitzerZ] = z;
		    Blitzer[i][Geschwindigkeit] = speed;
		    Blitzer[i][Radius] = radius;
		    Blitzer[i][Erstellt] = 1;
		    Blitzer[i][ID] = CreateObject(1278, x + 1.0, y + 1.0, z, 0.0, 0.0, 10);
		    return 1;
		}
	}
	return 0;
}
stock RemoveBlitzer(playerid) {
   	for(new i = 0; i < sizeof(Blitzer); i++) {
	   if(Blitzer[i][Erstellt] == 1) {
			if(IsPlayerInRangeOfPoint(playerid, Blitzer[i][eRadius], Blitzer[i][BlitzerX], Blitzer[i][BlitzerY], Blitzer[i][BlitzerZ])) {
				Blitzer[i][Erstellt] = 0;
    			Blitzer[i][BlitzerX] = 0.0;
		        Blitzer[i][BlitzerY] = 0.0;
	         	Blitzer[i][BlitzerZ] = 0.0;
           		DestroyObject(Blitzer[i][ID]);
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
    		DestroyObject(Blitzer[i][ID]);
  	    }
  	}
   	return 0;
}*/
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
