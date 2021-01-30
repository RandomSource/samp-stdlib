////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Credits:
// Script erstellt von Stas
// Offizieller Scripter der Xtreme - RolePlay - Community
// www.xtreme-roleplay.eu
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <a_samp>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Macros
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define SENDE_NACHRICHT // ausklammmern, falls unerwünscht
#define WAFFE_9MM    (22)
#define WAFFE_9MMs   (23)
#define WAFFE_DEAGLE (24)
#define WAFFE_GEWEHR (33)
#define WAFFE_SNIPER (34)
#define GEDRUECKT(%0) (((newkeys & (%0)) == (%0)) && ((oldkeys & (%0)) != (%0)))
#define SPIELER_GEDUCKT(%1) GetPlayerSpecialAction(%1) == SPECIAL_ACTION_DUCK
#define NOTSPIELER_GEDUCKT(%1) GetPlayerSpecialAction(%1) != SPECIAL_ACTION_DUCK
#define HAT_GESCHOSSEN(%1) GetPVarInt(%1, "HEADSHOT_GESCHOSSEN")
#define SETZE_GESCHOSSEN(%1) SetPVarInt(%1, "HEADSHOT_GESCHOSSEN", 1)
#define SETZE_NICHT_GESCHOSSEN(%1) SetPVarInt(%1, "HEADSHOT_GESCHOSSEN", 0)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CallBacks
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Überprüft, ob Spieler mit der Sniper abschießt
stock OnPlayerKeyStateChangeHeader(playerid, newkeys, oldkeys) {
	if(IsPlayerInFrakt(playerid,15)&&GEDRUECKT(KEY_FIRE) && ValideWaffe(playerid) && GetPlayerWeaponState(playerid)!=WEAPONSTATE_RELOADING) {
	    if(!HAT_GESCHOSSEN(playerid)) {
			SETZE_GESCHOSSEN(playerid);
	        SetTimerEx("Headshot_AntiSpam", 1000, false, "d", playerid);
			Headshot_ErmittleZielperson(playerid);
		}
 	}
	return 1;
}
// Setzt das AntiSpam
forward Headshot_AntiSpam(playerid);
public Headshot_AntiSpam(playerid) {
	SETZE_NICHT_GESCHOSSEN(playerid);
	return 1;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// private Funktionen
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Hier könnt ihr die Waffen setzen, mit welchen man Kopfschüße machen kann
stock ValideWaffe(waffe)
{
    //if(!IsPlayerInFrakt(waffe,15))return 0;
    new ValideWaffen[] = { WAFFE_SNIPER};
    for(new index = 0; index < sizeof(ValideWaffen); index++)
    {
        if(GetPlayerWeapon(waffe) == ValideWaffen[index]) return 1;
    }
    return 0;
}

stock TOETESPIELER(playerid,killerid)
{
        #pragma unused killerid
	 SetPlayerHealth(playerid,0);
	 CallRemoteFunction("OnPlayerDeath","iii",playerid,killerid,GetPlayerWeapon(killerid));
	return 1;
}
// Überprüft, ob Spieler die Zielperson in den Kopf trifft
stock Headshot_ErmittleZielperson(playerid) {
	// Erstellen der Speicherstellen für die Koordinaten des Vektors und der Kamera
	new Float:x,Float:y,Float:z,Float:FrontX,Float:FrontY,Float:FrontZ,Float:CamX,Float:CamY,Float:CamZ;
	// Setzen der Koordinaten in die Variablen
	// Funktionen werden im Tutorial erklärt
	GetPlayerCameraFrontVector(playerid, FrontX, FrontY, FrontZ);
 	GetPlayerCameraPos(playerid, CamX, CamY, CamZ);
	for(new Float:i = 0.0; i < 100; i = i + 0.5) {
	    // Berechnung der Koordinaten mit Hilfe einer bekannten Rechnung
 		x = FrontX * i + CamX; y = FrontY * i + CamY; z = FrontZ * i + CamZ;
		for(new zielperson = 0; zielperson < MAX_PLAYERS; zielperson ++) {
		    if(IsPlayerConnected(zielperson)) {
		        // Spieler kann sich zwar nicht selber anschießen, aber wir gehen auf eine Nummer sicher
		    	if(zielperson != playerid) {
		    	    // Falls die Zielperson geduckt
		    		if(SPIELER_GEDUCKT(zielperson)) {
		    		    // Überprüfen, ob die Zielperson tatsächlich auf den berechneten Koodinaten ist
		        		if(IsPlayerInRangeOfPoint(zielperson, 0.5, x, y, z)) {
		            		GameTextForPlayer(playerid, "~r~HEADSHOT", 2000, 6); // GameText für den Spieler
		            		//GameTextForPlayer(zielperson, "~r~HEADSHOT!", 2000, 6); // GameText für die Zielperson
		            		TOETESPIELER(zielperson,playerid); // Töten der Tielperson
		            		#if defined SENDE_NACHRICHT
		            		    SendDeathMessage(playerid, zielperson, 34);
		            		#endif
		        		}
					}
					// Falls die Zielperson steht
					if(NOTSPIELER_GEDUCKT(zielperson)) {
					    // Überprüfen, ob die Zielperson tatsächlich auf den berechneten Koodinaten ist
		    			if(IsPlayerInRangeOfPoint(zielperson, 0.5, x, y, z - 0.6)){
							GameTextForPlayer(playerid, "~r~HEADSHOT", 2000, 6); // GameText für den Spieler
							//GameTextForPlayer(zielperson, "~r~HEADSHOT", 2000, 6); // GameText für die Zielperson
							TOETESPIELER(zielperson,playerid); // Töten der Tielperson
							#if defined SENDE_NACHRICHT
		            		    SendDeathMessage(playerid, zielperson, 34);
		            		#endif
						}
					}
					// Falls die Zielperson in einem Auto sitzt
					if(IsPlayerInAnyVehicle(zielperson)) {
					    // Überprüfen, ob die Zielperson tatsächlich auf den berechneten Koodinaten ist
		    			if(IsPlayerInRangeOfPoint(zielperson, 0.5, x, y, z - 0.2)){
							GameTextForPlayer(playerid, "~r~HEADSHOT", 2000, 6); // GameText für den Spieler
							//GameTextForPlayer(zielperson, "~r~HEADSHOT", 2000, 6); // GameText für die Zielperson
							TOETESPIELER(zielperson,playerid); // Töten der Tielperson
							#if defined SENDE_NACHRICHT
		            		    SendDeathMessage(playerid, zielperson, 34);
		            		#endif
						}
					}
				}
			}
		}
	}
	return 1;
}