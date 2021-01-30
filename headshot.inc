/*************************************************
Infinite-Gaming.com Presents:
BoOm Headshot v.1 by Tannz0rz && Cyber_Punk
The closest thing to real headshots you can get in samp.

DO NOT REMOVE CREDITS
If you borrow bits of code would be nice to credit the original authors...

Thanks Kye and the Dev Team. R5+ is nice.

CONSERVATIVE mode is Recommended mode for large servers to save on processing.
This disables head shot checks when a player is holding KEY_FIRE + AIM (repeat fire).
To gain a head shot they must tap fire button. (While holding AIM)

***The Only time you can gain a headshot is in Target mode with the crosshairs.***

Left in the script is the original debug mode and "Line of Sight" (LOS) code.
To enable them just uncomment the lines below, when done simply comment them.
*************************************************/
#include <a_samp>

//======Config Options
//#define DEBUG
//#define NO_LOS //Uncomment this if you want no "Line of Sight" with your DEBUG mode
//#define FOR_EACH
#define CONSERVATIVE   //Comment for servers less than 100 players....Use @ your own risk.
#define DESTROY_LOS 500 //Time in milliseconds to remove line of sight objects
#undef MAX_PLAYERS
	#define MAX_PLAYERS 150 //Set to your max players

//======End Options

#if defined FOR_EACH
	#include <foreach>
#endif

#define KEY_AIMFIRE 132

#define PRESSED(%0) \
	(((newkeys & (%0)) == (%0)) && ((oldkeys & (%0)) != (%0)))

#if defined DEBUG
	new count1, count2;
#endif

//----WEAPON RANGES PULLED FROM WEAPONS.DAT--Country Rifle -10.0
new
	Float:ranges[39] =
	{
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		0.0,
		35.0,
		35.0,
		35.0,
		40.0,
		35.0,
		40.0,
		45.0,
		35.0,
		70.0,
		90.0,
		35.0,
		90.0,
		100.0,
		0.0,
		0.0,
		0.0,
		25.0
	};

#if defined FOR_EACH
	Itter_Create(Victim, MAX_PLAYERS);
#endif

stock OnPlayerKeyStateChangeHeader(playerid, newkeys, oldkeys)
{
    if(PRESSED(KEY_AIMFIRE) && NoBadCam(playerid))
    {
		switch(GetPlayerWeapon(playerid))
		{
   			#if !defined CONSERVATIVE
			    case WEAPON_SNIPER:
					if(GetPlayerWeaponState(playerid) == WEAPONSTATE_LAST_BULLET && IsPlayerInFrakt(playerid,15)) CheckHeadShot(playerid);
				case 22..33,38:
				{
					SetTimerEx("HSTimer", 100, 0, "i", playerid);
				}
			#else
	   			case WEAPON_SNIPER:
					if(GetPlayerWeaponState(playerid) == WEAPONSTATE_LAST_BULLET && IsPlayerInFrakt(playerid,15)) CheckHeadShot(playerid);
				case WEAPON_RIFLE, WEAPON_SHOTGUN:
				    if(GetPlayerWeaponState(playerid) != WEAPONSTATE_RELOADING) CheckHeadShot(playerid);
				case 24,30,31:
					if(GetPlayerWeaponState(playerid) != WEAPONSTATE_RELOADING) CheckHeadShot(playerid);
			#endif
		}
	}
    return 1;
}

forward HSTimer(playerid);
public HSTimer(playerid)
{
	new keys, ud, lr;
	GetPlayerKeys(playerid, keys, ud, lr);
	if((keys & KEY_FIRE) == (KEY_FIRE) && NoBadCam(playerid))
	{
		switch(GetPlayerWeapon(playerid))
		{
		    case WEAPON_COLT45, WEAPON_UZI, WEAPON_TEC9, WEAPON_MP5, WEAPON_AK47, WEAPON_M4:{
				if(GetPlayerWeaponState(playerid) != WEAPONSTATE_RELOADING){
                	CheckHeadShot(playerid);
					SetTimerEx("HSTimer", 150, 0, "i", playerid);
				}
			}
			case WEAPON_SHOTGSPA, WEAPON_SAWEDOFF:{
				if(GetPlayerWeaponState(playerid) != WEAPONSTATE_RELOADING){
					CheckHeadShot(playerid);
			    	SetTimerEx("HSTimer", 300, 0, "i", playerid);
				}
			}
			case WEAPON_SILENCED:{
				if(GetPlayerWeaponState(playerid) != WEAPONSTATE_RELOADING){
					CheckHeadShot(playerid);
					SetTimerEx("HSTimer", 500, 0, "i", playerid);
				}
			}
			case WEAPON_DEAGLE:{
			    if(GetPlayerWeaponState(playerid) != WEAPONSTATE_RELOADING){
					CheckHeadShot(playerid);
					SetTimerEx("HSTimer", 800, 0, "i", playerid);
				}
			}
			case WEAPON_SHOTGUN:
			{
				if(GetPlayerWeaponState(playerid) != WEAPONSTATE_NO_BULLETS){
					CheckHeadShot(playerid);
					SetTimerEx("HSTimer", 900, 0, "i", playerid);
				}
			}
			case WEAPON_RIFLE:
			{
				if(GetPlayerWeaponState(playerid) != WEAPONSTATE_RELOADING){
			    	CheckHeadShot(playerid);
			    	SetTimerEx("HSTimer", 1000, 0, "i", playerid);
				}
			}
		}
	}
	return 1;
}

forward CheckHeadShot(playerid);
public CheckHeadShot(playerid)
{
	#if defined DEBUG
		count1 = GetTickCount();
	#endif

	new Float:x[4], Float:y[4], Float:z[4], Float:dist = 1.0, Float:range = 0.0;
	GetPlayerCameraPos(playerid, x[0], y[0], z[0]);
	GetPlayerCameraFrontVector(playerid, x[1], y[1], z[1]);

	#if defined DEBUG
		printf("\nCamera Position: x=%.2f, y=%.2f, z=%.2f\nFrontVector: x=%.2f, y=%.2f, z=%.2f", x[0], y[0], z[0], x[1], y[1], z[1]);
	#endif

	range = ranges[GetPlayerWeapon(playerid)];

	if(range != 0.0)
	{
		switch(GetPlayerWeapon(playerid))
		{
			case WEAPON_RIFLE:
			{
				if(x[1] < 0.0 && y[1] > 0.0 )
				{
					x[1] += 0.0155;
					y[1] += 0.0155;
				}
				else if(x[1] > 0.0 && y[1] > 0.0)
				{
					x[1] += 0.0155;
					y[1] -= 0.0155;
				}
				else if(x[1] < 0.0 && y[1] < 0.0)
				{
					x[1] -= 0.0155;
					y[1] += 0.0155;
				}
				else if(x[1] > 0.0 && y[1] < 0.0)
				{
					x[1] -= 0.0155;
					y[1] -= 0.0155;
				}
				if(z[1] > 0.1) z[1] += 0.06;
				else z[1] += 0.05;
			}
			case WEAPON_AK47,WEAPON_M4:
			{
				if(x[1] < 0.0 && y[1] > 0.0 )
				{
					x[1] += 0.0225;
					y[1] += 0.0225;
				}
				else if(x[1] > 0.0 && y[1] > 0.0)
				{
					x[1] += 0.0225;
					y[1] -= 0.0225;
				}
				else if(x[1] < 0.0 && y[1] < 0.0)
				{
					x[1] -= 0.0225;
					y[1] += 0.0225;
				}
				else if(x[1] > 0.0 && y[1] < 0.0)
				{
					x[1] -= 0.0225;
					y[1] -= 0.0225;
				}
				if(z[1] > 0.0) z[1] += 0.08;
				else if(z[1] < -0.0 && z[1] > -0.5) z[1] += 0.06;
				else if(z[1] < -0.5) z[1] -= 0.05;
			}
			case 22..29,32,38:
			{
				if(x[1] < 0.0 && y[1] > 0.0 )
				{
					x[1] += 0.0325;
					y[1] += 0.0325;
				}
				else if(x[1] > 0.0 && y[1] > 0.0)
				{
					x[1] += 0.0325;
					y[1] -= 0.0325;
				}
				else if(x[1] < 0.0 && y[1] < 0.0)
				{
					x[1] -= 0.0325;
					y[1] += 0.0325;
				}
				else if(x[1] > 0.0 && y[1] < 0.0)
				{
					x[1] -= 0.0325;
					y[1] -= 0.0325;
				}
				if(z[1] > 0.0) z[1] += 0.125;
				else z[1] += 0.09;
			}
		}
        x[0] += (2.0 * x[1]);
		y[0] += (2.0 * y[1]);

		do
		{
        	x[0] += (x[1] * 1.0);
           	y[0] += (y[1] * 1.0);
           	z[0] += (z[1] * 1.0);

			#if defined DEBUG
			    #if !defined NO_LOS
	        		SetTimerEx("DestroyMe", DESTROY_LOS, 0, "i", CreateObject(1239, x[0], y[0], z[0], 0.0, 0.0, 0.0));
				#endif
			#endif

		#if !defined FOR_EACH
			for(new i = 0; i < MAX_PLAYERS; ++i)
			#if !defined DEBUG
				if(IsPlayerEingeloggt(i))
			#endif
		#else
			foreach(HSVictim, i)
		#endif
	        {
				if( i != playerid&&IsPlayerEingeloggt(i)&&GetPVarInt(i,"tot")==0)
				{
		            GetPlayerPos(i, x[2], y[2], z[2]);
		            
		            if(GetPlayerSpecialAction(i) == SPECIAL_ACTION_DUCK){ z[2] -= 0.6;}	else {z[2] += 0.6;}
					if(PointInRangeOfPoint(0.3, x[0], y[0], z[0], x[2], y[2], z[2]))
					{
                                           if(GetPVarInt(i,"tot")==0)
                                           {
                                           SetPlayerHealth(i,0);
  		                         OnPlayerDeath(i,playerid,GetPlayerWeapon(playerid));
  		                         SendClientMessage(playerid,0xFF0000AA,"Headshot!"); 
  		                         SetPVarInt(i,"geheadert",1);  
  		                         }
						#if defined DEBUG
							count2 = GetTickCount();
							count1 = count2 - count1;
							printf("\n Player hit\n Max Players Looped: %d\n Max Weapon Range: %.2f\n Distance to Hit: %.2f\n Total execution time: %8d ms",i,range,dist,count1);
			    			count1=count2;
						#endif
						return 1;
					}
				}
	        }
            dist += 1.0;
		}
		while( dist < range );
		#if defined DEBUG
			count2 = GetTickCount();
			count1 = count2 - count1;
			printf("\n Max Players Looped: %d\n Max Weapon Range: %.2f\n Total execution time: %8d ms",MAX_PLAYERS,range, count1);
			count1=count2;
		#endif
	}
	return 1;
}

stock NoBadCam(playerid) //Don't ask but it works
{

	new Float:x, Float:y, Float:z,Float:ang;
	GetPlayerCameraFrontVector(playerid, x, y, z);
	GetPlayerFacingAngle(playerid,ang);
	
	if((x < 0.0 && y > 0.0) && (ang > 90.0 && ang < 300.0)) return 0;
	else if((x > 0.0 && y > 0.0) && (ang > 88.16 && ang < 275.0)) return 0;
	else if((x < 0.0 && y < 0.0) && (ang > 257.30 && ang < 360.0)) return 0;
	else if((x > 0.0 && y < 0.0) && (ang < 88.16 && ang < 257.30)) return 0;

	return 1;
}

// As taken from Y_Less's IsPlayerInRangeOfPoint
stock PointInRangeOfPoint(Float:range, Float:x, Float:y, Float:z, Float:X, Float:Y, Float:Z)
{
    X -= x;
    Y -= y;
    Z -= z;
    return ((X * X) + (Y * Y) + (Z * Z)) < (range * range);
}

forward DestroyMe(id);
public DestroyMe(id)
{
    DestroyObject(id);
    return 1;
}