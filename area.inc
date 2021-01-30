#if defined _editarea_included_
	#endinput
#endif
#define _editarea_included_

#include a_samp.inc


//---------------------------------------------------//
//  PlayerEditArea v1.0 by  Patryk "Maku" Makowski  //
//---------------------------------------------------//


/*
	PlayerEditArea(playerid, areaid);
	IsPlayerEditArea(playerid);
	OnPlayerEditAreaFinish(playerid, areaid, Float:minx, Float:maxx, Float:miny, Float:maxy, success);
	OnPlayerEditAreaUpdate(playerid, areaid, Float:minx, Float:maxx, Float:miny, Float:maxy);
*/
#define UNDEFINED_EDITAREA -1

#define EDITAREA_REASON_SUCCESS 1
#define EDITAREA_REASON_FAIL 0
#define EDITAREA_REASON_CANCEL 2

#define EDITAREA_FINISHKEY KEY_YES
#define EDITAREA_CANCELKEY  KEY_NO

#define EDITAREA_OBJECTID 1318
#define EDITAREA_CAMERACHANGE (60.0)

forward OnPlayerEditAreaFinish(playerid, areaid, Float:minx, Float:maxx, Float:miny, Float:maxy, success);
forward OnPlayerEditAreaUpdate(playerid, areaid, Float:minx, Float:maxx, Float:miny, Float:maxy);

static PlayerObjects_EA[MAX_PLAYERS][40];
static PlayerEdit_EA[MAX_PLAYERS] = {UNDEFINED_EDITAREA,...};
static Float:PlayerArea_EA[MAX_PLAYERS][2];

stock IsPlayerEditArea(playerid)
{
	if(PlayerEdit_EA[playerid] == -1) return false;
	
	return true;
}

public OnPlayerDisconnect(playerid, reason)
{
	if(PlayerEdit_EA[playerid] != UNDEFINED_EDITAREA) EditAreaFinish_(playerid, PlayerEdit_EA[playerid], 0.0,0.0, EDITAREA_REASON_FAIL);

	CallLocalFunction("EDAREA_OnPlayerDisconnect", "ii", playerid, reason);
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(PlayerEdit_EA[playerid] != UNDEFINED_EDITAREA)
	{
		if(newkeys & EDITAREA_CANCELKEY)
		{
			new Float:x, Float:y, Float:z;
			GetPlayerPos(playerid, x,y,z);
			EditAreaFinish_(playerid, PlayerEdit_EA[playerid], x,y, EDITAREA_REASON_CANCEL);
			return 1;
		}
		if(newkeys & EDITAREA_FINISHKEY)
		{		
			new Float:x, Float:y, Float:z;
			GetPlayerPos(playerid, x,y,z);
			EditAreaFinish_(playerid, PlayerEdit_EA[playerid], x,y, EDITAREA_REASON_SUCCESS);
			return 1;
		}
	}
	CallLocalFunction("EDAREA_OnPlayerKeyStateChange", "iii", playerid, newkeys, oldkeys);
	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	if(newstate != PLAYER_STATE_ONFOOT && PlayerEdit_EA[playerid] != UNDEFINED_EDITAREA)
	{
		EditAreaFinish_(playerid, PlayerEdit_EA[playerid], 0.0,0.0, EDITAREA_REASON_FAIL);
	}
	CallLocalFunction("EDAREA_OnPlayerStateChange", "iii", playerid, newstate, oldstate);
	return 1;
}

stock EditAreaFinish_(playerid, areaid, Float:x, Float:y, success)
{
	for(new i; i < 40; i++) DestroyPlayerObject(playerid, PlayerObjects_EA[playerid][i]);
	
	new Float:minx, Float:miny, Float:maxx, Float:maxy;
	if(PlayerArea_EA[playerid][0] > x) {
		minx = x;
		maxx = PlayerArea_EA[playerid][0];
	} else { 
		minx = PlayerArea_EA[playerid][0];
		maxx = x;
	}
	if(PlayerArea_EA[playerid][1] > y) {
		miny = y;
		maxy = PlayerArea_EA[playerid][1];
	} else { 
		miny = PlayerArea_EA[playerid][1];
		maxy = y;
	}
		
	PlayerEdit_EA[playerid] = UNDEFINED_EDITAREA;
	CallLocalFunction("OnPlayerEditAreaFinish", "iiffffi", playerid, areaid, minx, maxx, miny, maxy, success);
	return 1;
}

stock PlayerEditArea(playerid, areaid)
{
	if(!IsPlayerConnected(playerid) || areaid < 0 || areaid == UNDEFINED_EDITAREA) return 0;
	if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return 0;
	if(PlayerEdit_EA[playerid] != UNDEFINED_EDITAREA) return 0;
	
	PlayerEdit_EA[playerid] = areaid;
	
	new Float:ER_z;
	GetPlayerPos(playerid, PlayerArea_EA[playerid][0], PlayerArea_EA[playerid][1], ER_z);

	PlayerObjects_EA[playerid][0] = CreatePlayerObject(playerid, EDITAREA_OBJECTID, PlayerArea_EA[playerid][0], PlayerArea_EA[playerid][1], ER_z - 1.5, 0.0, 0.0, 0.0, 500.0);	
	return 1;
}
new counter[MAX_PLAYERS];
public OnPlayerUpdate(playerid)
{
	counter[playerid]++;
	
	if(counter[playerid]> 20 && PlayerEdit_EA[playerid] != UNDEFINED_EDITAREA)
	{
		counter[playerid]=0;
		for(new i; i < 40; i++) if(IsValidPlayerObject(playerid, PlayerObjects_EA[playerid][i])) DestroyPlayerObject(playerid, PlayerObjects_EA[playerid][i]);
		
		new Float:x, Float:y, Float:z;
		new Float:minx, Float:miny, Float:maxx, Float:maxy;
		GetPlayerPos(playerid, x,y,z);
		if(PlayerArea_EA[playerid][0] > x) {
			minx = x;
			maxx = PlayerArea_EA[playerid][0];
		} else { 
			minx = PlayerArea_EA[playerid][0];
			maxx = x;
		}
		if(PlayerArea_EA[playerid][1] > y) {
			miny = y;
			maxy = PlayerArea_EA[playerid][1];
		} else { 
			miny = PlayerArea_EA[playerid][1];
			maxy = y;
		}
		for(new i; i < 10; i++)
		{
			PlayerObjects_EA[playerid][i] = CreatePlayerObject(playerid, EDITAREA_OBJECTID,	minx + (((maxx - minx) * 0.1) * i), miny, z - 0.05, 0.0, 0.0, 0.0, 500.0);
            PlayerObjects_EA[playerid][i+10] = CreatePlayerObject(playerid, EDITAREA_OBJECTID, minx + (((maxx - minx) * 0.1) * i), maxy, z - 0.05, 0.0, 0.0, 0.0, 500.0);
			PlayerObjects_EA[playerid][i+20] = CreatePlayerObject(playerid, EDITAREA_OBJECTID, minx, maxy - (((maxy - miny) * 0.1) * i), z - 0.05, 0.0, 0.0, 0.0, 500.0);
			PlayerObjects_EA[playerid][i+30] = CreatePlayerObject(playerid, EDITAREA_OBJECTID, maxx, maxy - (((maxy - miny) * 0.1) * i), z - 0.05, 0.0, 0.0, 0.0, 500.0);
		}
	
		CallLocalFunction("OnPlayerEditAreaUpdate", "iiffff", playerid, PlayerEdit_EA[playerid], minx, maxx, miny, maxy);
	}
	CallLocalFunction("EDAREA_OnPlayerUpdate", "i", playerid);
	return 1;
}

#if defined _ALS_OnPlayerUpdate
	#undef OnPlayerUpdate
#else
	#define _ALS_OnPlayerUpdate
#endif
#define OnPlayerUpdate EDAREA_OnPlayerUpdate
forward EDAREA_OnPlayerUpdate(playerid);

#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect EDAREA_OnPlayerDisconnect
forward EDAREA_OnPlayerDisconnect(playerid, reason);


#if defined _ALS_OnPlayerStateChange
	#undef OnPlayerStateChange
#else
	#define _ALS_OnPlayerStateChange
#endif
#define OnPlayerStateChange EDAREA_OnPlayerStateChange
forward EDAREA_OnPlayerStateChange(playerid, newstate, oldstate);


#if defined _ALS_OnPlayerKeyStateChange
	#undef OnPlayerKeyStateChange
#else
	#define _ALS_OnPlayerKeyStateChange
#endif
#define OnPlayerKeyStateChange EDAREA_OnPlayerKeyStateChange
forward EDAREA_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);