/*
 *  sscanf 2.8.2
 *  Created by Y_Less, updated by Emmet_.
 *
 *  Version: MPL 1.1
 *
 *  The contents of this file are subject to the Mozilla Public License Version
 *  1.1 (the "License"); you may not use this file except in compliance with
 *  the License. You may obtain a copy of the License at
 *  [url]http://www.mozilla.org/MPL/[/url]
 *
 *  Software distributed under the License is distributed on an "AS IS" basis,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 *  for the specific language governing rights and limitations under the
 *  License.
 *
 *  The Original Code is the sscanf 2.0 SA:MP plugin.
 *
 *  The Initial Developer of the Original Code is Alex "Y_Less" Cole.
 *  Portions created by the Initial Developer are Copyright (C) 2010
 *  the Initial Developer. All Rights Reserved.
 *
 *  Contributor(s):
 *
 *  Special Thanks to:
 *
 *      SA:MP Team past, present and future
 */

#if defined _inc_a_npc
	#pragma library sscanf
#elseif !defined _samp_included
	#error Please include <a_npc> or <a_samp> first.
#endif

#define SSCANF:%0(%1) forward sscanf_%0(%1);public sscanf_%0(%1)

#if defined sscanf
	#error sscanf (possibly the PAWN version) already defined.
#endif

native sscanf(const data[], const format[], {Float,_}:...);
native unformat(const data[], const format[], {Float,_}:...) = sscanf;
native SSCANF_Init(players, invalid, len);
native SSCANF_Join(playerid, const name[], npc);
native SSCANF_Leave(playerid);
native SSCANF_IsConnected(playerid);

native SSCANF_Option(const name[], value);

stock const
	SSCANF_QUIET[] = "SSCANF_QUIET",
	OLD_DEFAULT_NAME[] = "OLD_DEFAULT_NAME",
	MATCH_NAME_PARTIAL[] = "MATCH_NAME_PARTIAL",
	CELLMIN_ON_MATCHES[] = "CELLMIN_ON_MATCHES",
	OLD_DEFAULT_KUSTOM[] = "OLD_DEFAULT_KUSTOM",
	OLD_DEFAULT_CUSTOM[] = "OLD_DEFAULT_CUSTOM";

static stock
	bool:SSCANF_gInit = false,
	SSCANF_g_sPlayers[MAX_PLAYERS char];

#if defined _inc_a_npc
	forward SSCANF_PlayerCheck();

	/*
	  OnNPCModeInit

	  Called when the script starts if it is a NPC mode, sets up the system,
	  then calls the "real" OnNPCModeInit (using the new ALS 2 hook method).
	*/

	public OnNPCModeInit()
	{
		SSCANF_Init(MAX_PLAYERS, INVALID_PLAYER_ID, MAX_PLAYER_NAME);
		#if !defined SSCANF_NO_PLAYERS
			// Initialise the system.
			SSCANF_PlayerCheck();
			SetTimer("SSCANF_PlayerCheck", 1, 1);
		#endif
		#if defined SSCANF_OnNPCModeInit
			SSCANF_OnNPCModeInit();
		#endif
		return 1;
	}

	#if defined _ALS_OnNPCModeInit
		#undef OnNPCModeInit
	#else
		#define _ALS_OnNPCModeInit
	#endif
	#define OnNPCModeInit SSCANF_OnNPCModeInit
	#if defined SSCANF_OnNPCModeInit
		forward SSCANF_OnNPCModeInit();
	#endif

	/*
	  SSCANF_PlayerCheck

	  NPC modes have no "OnPlayerConnect callback, so we need to simulate one.
	*/

	#if !defined SSCANF_NO_PLAYERS
		public SSCANF_PlayerCheck()
		{
			for (new i = 0; i != MAX_PLAYERS; ++i)
			{
				if (IsPlayerConnected(i))
				{
					if (!SSCANF_g_sPlayers{i})
					{
						new
							name[MAX_PLAYER_NAME];
						GetPlayerName(i, name, sizeof (name));
						// We have no way to know if they are an NPC or not!
						SSCANF_Join(i, name, 0);
						SSCANF_g_sPlayers{i} = 1;
					}
				}
				else
				{
					if (SSCANF_g_sPlayers{i})
					{
						SSCANF_Leave(i);
						SSCANF_g_sPlayers{i} = 0;
					}
				}
			}
		}
	#endif
#else
	/*
	  OnFilterScriptInit

	  Called when the script starts if it is a filterscript, sets up the system,
	  then calls the "real" OnFilterScriptInit (using the new ALS 2 hook
	  method).
	*/

	public OnFilterScriptInit()
	{
	    new
     		name[MAX_PLAYER_NAME];
	
		SSCANF_Init(GetMaxPlayers(), INVALID_PLAYER_ID, MAX_PLAYER_NAME);
		SSCANF_gInit = true;
		
		// Check if there are any players that aren't initialized.
		for (new i = 0; i < MAX_PLAYERS; i ++)
		{
		    if (IsPlayerConnected(i) && !SSCANF_IsConnected(i))
		    {
		        GetPlayerName(i, name, MAX_PLAYER_NAME);
		        SSCANF_Join(i, name, IsPlayerNPC(i));
		    }
		}
		
		#if defined SSCANF_OnFilterScriptInit
			SSCANF_OnFilterScriptInit();
		#endif
		return 1;
	}

	#if defined _ALS_OnFilterScriptInit
		#undef OnFilterScriptInit
	#else
		#define _ALS_OnFilterScriptInit
	#endif
	#define OnFilterScriptInit SSCANF_OnFilterScriptInit
	#if defined SSCANF_OnFilterScriptInit
		forward SSCANF_OnFilterScriptInit();
	#endif

	/*
	  OnGameModeInit

	  Called when the script starts if it is a gamemode.  This callback is also
	  called in filterscripts so we don't want to reinitialise the system in
	  that case.
	*/

	public OnGameModeInit()
	{
		if (!SSCANF_gInit)
		{
		    new
		        name[MAX_PLAYER_NAME];
		
			SSCANF_Init(GetMaxPlayers(), INVALID_PLAYER_ID, MAX_PLAYER_NAME);
			SSCANF_gInit = true;
			
			// Check if there are any players that aren't initialized.
			for (new i = 0; i < MAX_PLAYERS; i ++)
			{
			    if (IsPlayerConnected(i) && !SSCANF_IsConnected(i))
			    {
			        GetPlayerName(i, name, MAX_PLAYER_NAME);
			        SSCANF_Join(i, name, IsPlayerNPC(i));
			    }
			}
		}
		#if defined SSCANF_OnGameModeInit
			SSCANF_OnGameModeInit();
		#endif
		return 1;
	}

	#if defined _ALS_OnGameModeInit
		#undef OnGameModeInit
	#else
		#define _ALS_OnGameModeInit
	#endif
	#define OnGameModeInit SSCANF_OnGameModeInit
	#if defined SSCANF_OnGameModeInit
		forward SSCANF_OnGameModeInit();
	#endif

	/*
	  OnPlayerConnect

	  Called when a player connects.  Actually increments an internal count so
	  that if a script ends and "OnPlayerDisconnect" is called then "sscanf"
	  still knows that the player is really connected.  Also stores their name
	  internally.
	*/

	public OnPlayerConnect(playerid)
	{
		new
			name[MAX_PLAYER_NAME];
		GetPlayerName(playerid, name, sizeof (name));
		SSCANF_Join(playerid, name, IsPlayerNPC(playerid));
		#if defined SSCANF_OnPlayerConnect
			SSCANF_OnPlayerConnect(playerid);
		#endif
		return 1;
	}

	#if defined _ALS_OnPlayerConnect
		#undef OnPlayerConnect
	#else
		#define _ALS_OnPlayerConnect
	#endif
	#define OnPlayerConnect SSCANF_OnPlayerConnect
	#if defined SSCANF_OnPlayerConnect
		forward SSCANF_OnPlayerConnect(playerid);
	#endif

	/*
	  OnPlayerDisconnect

	  Called when a player disconnects, or when a script is ended.
	*/

	public OnPlayerDisconnect(playerid, reason)
	{
		#if defined SSCANF_OnPlayerDisconnect
			SSCANF_OnPlayerDisconnect(playerid, reason);
		#endif
		SSCANF_Leave(playerid);
		return 1;
	}

	#if defined _ALS_OnPlayerDisconnect
		#undef OnPlayerDisconnect
	#else
		#define _ALS_OnPlayerDisconnect
	#endif
	#define OnPlayerDisconnect SSCANF_OnPlayerDisconnect
	#if defined SSCANF_OnPlayerDisconnect
		forward SSCANF_OnPlayerDisconnect(playerid, reason);
	#endif
#endif

#define SSCANF_Init
#define SSCANF_Join
#define SSCANF_Leave

#define extract%0->%1; EXTRN%1;unformat(_:EXTRZ:EXTRV:EXTRX:%0,##,%1,,);
#define unformat(_:EXTRZ:EXTRV:EXTRX:%0,##,%1);%2else if (unformat(_:EXTRV:EXTRX:%0,##,%1))

#define EXTRV:EXTRX:%0<%3>##,%9new%1,%2) EXTRY:%0##P<%3>,|||%1|||%2)
#define EXTRX:%0##,%9new%1,%2) EXTRY:%0##,|||%1|||%2)
#define EXTRY: EXTR8:EXTR9:EXTR0:EXTR1:EXTR2:EXTR3:EXTR4:

#define EXTR8:EXTR9:EXTR0:EXTR1:EXTR2:EXTR3:EXTR4:%0##%1,%2|||%6:%3=%9|||%4) %6_EXTRO:%0##%1,%2|||%3=%9|||%4)
#define EXTR9:EXTR0:EXTR1:EXTR2:EXTR3:EXTR4:%0##%1,%2|||%3=%9|||%4) __EXTRO:%0##%1,%2|||%3=%9|||%4)
#define EXTR0:EXTR1:EXTR2:EXTR3:EXTR4:%0##%1,%2|||%6:%3[%7]|||%4) %6_EXTRW:%0##%1,%2|||%3[%7]|||%4)
#define EXTR1:EXTR2:EXTR3:EXTR4:%0##%1,%2|||%3[%7]|||%4) __EXTRW:%0##%1,%2|||%3|||%4)
#define EXTR2:EXTR3:EXTR4:%0##%1,%2|||%6:%3|||%4) %6_EXTRN:%0##%1,%2|||%3|||%4)
#define EXTR3:EXTR4:%0##%1,,%2||||||%4) %0##%1,%2)
#define EXTR4:%0##%1,%2|||%3|||%4) __EXTRN:%0##%1,%2|||%3|||%4)

// Optional specifiers.
#define __EXTRO:%0##%1,%2|||%3=%9|||%4,%5) EXTRY:%0##%1I"("#%9")"#,%2,%3|||%4|||%5)
#define Float_EXTRO:%0##%1,%2|||%3=%9|||%4,%5) EXTRY:%0##%1F"("#%9")"#,%2,%3|||%4|||%5)
#define player_EXTRO:%0##%1,%2|||%3=%9|||%4,%5) EXTRY:%0##%1U"("#%9")"#,%2,%3|||%4|||%5)
#define string_EXTRO:%0##%1,%2|||%3[%7]=%9|||%4,%5) EXTRY:%0##%1S"("#%9")"#[%7],%2,%3|||%4|||%5)

// Normal specifiers (the double underscore is to work for "_:".
#define __EXTRN:%0##%1,%2|||%3|||%4,%5) EXTRY:%0##%1i,%2,%3|||%4|||%5)
#define Float_EXTRN:%0##%1,%2|||%3|||%4,%5) EXTRY:%0##%1f,%2,%3|||%4|||%5)
#define player_EXTRN:%0##%1,%2|||%3|||%4,%5) EXTRY:%0##%1u,%2,%3|||%4|||%5)
//#define string_EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1s[%7],%2,%3|||%4|||%5)

// Array versions of normal specifiers.
#define __EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1a<i>[%7],%2,%3|||%4|||%5)
#define Float_EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1a<f>[%7],%2,%3|||%4|||%5)
#define player_EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1a<u>[%7],%2,%3|||%4|||%5)
#define string_EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1s[%7],%2,%3|||%4|||%5)

// Get rid of excess leading space which causes warnings.
#define EXTRN%0new%1; new%1;

#if !defined string
    #define string:
#endif

#define player:

#define hex:
#define hex_EXTRO:%0##%1,%2|||%3=%9|||%4,%5) EXTRY:%0##%1H"("#%9")"#,%2,%3|||%4|||%5)
#define hex_EXTRN:%0##%1,%2|||%3|||%4,%5) EXTRY:%0##%1h,%2,%3|||%4|||%5)
#define hex_EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1a<h>[%7],%2,%3|||%4|||%5)

#define bin:
#define bin_EXTRO:%0##%1,%2|||%3=%9|||%4,%5) EXTRY:%0##%1B"("#%9")"#,%2,%3|||%4|||%5)
#define bin_EXTRN:%0##%1,%2|||%3|||%4,%5) EXTRY:%0##%1b,%2,%3|||%4|||%5)
#define bin_EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1a<b>[%7],%2,%3|||%4|||%5)

#define kustom:%0<%1> %0
#define kustom_EXTRO:%0##%1,%2|||%3<%8>=%9|||%4,%5) EXTRY:%0##%1K<%8>"("#%9")"#,%2,%3|||%4|||%5)
#define kustom_EXTRN:%0##%1,%2|||%3<%8>|||%4,%5) EXTRY:%0##%1k<%8>,%2,%3|||%4|||%5)
//#define bin_EXTRW:%0##%1,%2|||%3[%7]|||%4,%5) EXTRY:%0##%1a<b>[%7],%2,%3|||%4|||%5)

SSCANF:weapon(wstring[])
{
	// This function is VERY basic, needs VASTLY improving to detect variations.
	if ('0' <= wstring[0] <= '9')
	{
		new
			ret = strval(wstring);
		if (0 <= ret <= 18 || 22 <= ret <= 46)
		{
			return ret;
		}
	}
	else if (!strcmp(wstring, "Unarmed")) return 0;
	else if (!strcmp(wstring, "Brass Knuckles")) return 1;
	else if (!strcmp(wstring, "Golf Club")) return 2;
	else if (!strcmp(wstring, "Night Stick")) return 3;
	else if (!strcmp(wstring, "Knife")) return 4;
	else if (!strcmp(wstring, "Baseball Bat")) return 5;
	else if (!strcmp(wstring, "Shovel")) return 6;
	else if (!strcmp(wstring, "Pool cue")) return 7;
	else if (!strcmp(wstring, "Katana")) return 8;
	else if (!strcmp(wstring, "Chainsaw")) return 9;
	else if (!strcmp(wstring, "Purple Dildo")) return 10;
	else if (!strcmp(wstring, "White Dildo")) return 11;
	else if (!strcmp(wstring, "Long White Dildo")) return 12;
	else if (!strcmp(wstring, "White Dildo 2")) return 13;
	else if (!strcmp(wstring, "Flowers")) return 14;
	else if (!strcmp(wstring, "Cane")) return 15;
	else if (!strcmp(wstring, "Grenades")) return 16;
	else if (!strcmp(wstring, "Tear Gas")) return 17;
	else if (!strcmp(wstring, "Molotovs")) return 18;
	else if (!strcmp(wstring, "Pistol")) return 22;
	else if (!strcmp(wstring, "Silenced Pistol")) return 23;
	else if (!strcmp(wstring, "Desert Eagle")) return 24;
	else if (!strcmp(wstring, "Shotgun")) return 25;
	else if (!strcmp(wstring, "Sawn Off Shotgun")) return 26;
	else if (!strcmp(wstring, "Combat Shotgun")) return 27;
	else if (!strcmp(wstring, "Micro Uzi")) return 28;
	else if (!strcmp(wstring, "Mac 10")) return 28;
	else if (!strcmp(wstring, "MP5")) return 29;
	else if (!strcmp(wstring, "AK47")) return 30;
	else if (!strcmp(wstring, "M4")) return 31;
	else if (!strcmp(wstring, "Tec9")) return 32;
	else if (!strcmp(wstring, "Rifle")) return 33;
	else if (!strcmp(wstring, "Sniper Rifle")) return 34;
	else if (!strcmp(wstring, "RPG")) return 35;
	else if (!strcmp(wstring, "Missile Launcher")) return 36;
	else if (!strcmp(wstring, "Flame Thrower")) return 37;
	else if (!strcmp(wstring, "Minigun")) return 38;
	else if (!strcmp(wstring, "Sachel Charges")) return 39;
	else if (!strcmp(wstring, "Detonator")) return 40;
	else if (!strcmp(wstring, "Spray Paint")) return 41;
	else if (!strcmp(wstring, "Fire Extinguisher")) return 42;
	else if (!strcmp(wstring, "Camera")) return 43;
	else if (!strcmp(wstring, "Nightvision Goggles")) return 44;
	else if (!strcmp(wstring, "Thermal Goggles")) return 45;
	else if (!strcmp(wstring, "Parachute")) return 46;
	return -1;
}

SSCANF:vehicle(vstring[])
{
	// This function is VERY basic, needs VASTLY improving to detect variations.
	if ('0' <= vstring[0] <= '9')
	{
		new
			ret = strval(vstring);
		if (400 <= ret <= 611)
		{
			return ret;
		}
	}
	else if (!strcmp(vstring, "Landstalker")) return 400;
	else if (!strcmp(vstring, "Bravura")) return 401;
	else if (!strcmp(vstring, "Buffalo")) return 402;
	else if (!strcmp(vstring, "Linerunner")) return 403;
	else if (!strcmp(vstring, "Perenniel")) return 404;
	else if (!strcmp(vstring, "Sentinel")) return 405;
	else if (!strcmp(vstring, "Dumper")) return 406;
	else if (!strcmp(vstring, "Firetruck")) return 407;
	else if (!strcmp(vstring, "Trashmaster")) return 408;
	else if (!strcmp(vstring, "Stretch")) return 409;
	else if (!strcmp(vstring, "Manana")) return 410;
	else if (!strcmp(vstring, "Infernus")) return 411;
	else if (!strcmp(vstring, "Voodoo")) return 412;
	else if (!strcmp(vstring, "Pony")) return 413;
	else if (!strcmp(vstring, "Mule")) return 414;
	else if (!strcmp(vstring, "Cheetah")) return 415;
	else if (!strcmp(vstring, "Ambulance")) return 416;
	else if (!strcmp(vstring, "Leviathan")) return 417;
	else if (!strcmp(vstring, "Moonbeam")) return 418;
	else if (!strcmp(vstring, "Esperanto")) return 419;
	else if (!strcmp(vstring, "Taxi")) return 420;
	else if (!strcmp(vstring, "Washington")) return 421;
	else if (!strcmp(vstring, "Bobcat")) return 422;
	else if (!strcmp(vstring, "Mr Whoopee")) return 423;
	else if (!strcmp(vstring, "BF Injection")) return 424;
	else if (!strcmp(vstring, "Hunter")) return 425;
	else if (!strcmp(vstring, "Premier")) return 426;
	else if (!strcmp(vstring, "Enforcer")) return 427;
	else if (!strcmp(vstring, "Securicar")) return 428;
	else if (!strcmp(vstring, "Banshee")) return 429;
	else if (!strcmp(vstring, "Predator")) return 430;
	else if (!strcmp(vstring, "Bus")) return 431;
	else if (!strcmp(vstring, "Rhino")) return 432;
	else if (!strcmp(vstring, "Barracks")) return 433;
	else if (!strcmp(vstring, "Hotknife")) return 434;
	else if (!strcmp(vstring, "Article Trailer")) return 435;
	else if (!strcmp(vstring, "Previon")) return 436;
	else if (!strcmp(vstring, "Coach")) return 437;
	else if (!strcmp(vstring, "Cabbie")) return 438;
	else if (!strcmp(vstring, "Stallion")) return 439;
	else if (!strcmp(vstring, "Rumpo")) return 440;
	else if (!strcmp(vstring, "RC Bandit")) return 441;
	else if (!strcmp(vstring, "Romero")) return 442;
	else if (!strcmp(vstring, "Packer")) return 443;
	else if (!strcmp(vstring, "Monster")) return 444;
	else if (!strcmp(vstring, "Admiral")) return 445;
	else if (!strcmp(vstring, "Squallo")) return 446;
	else if (!strcmp(vstring, "Seasparrow")) return 447;
	else if (!strcmp(vstring, "Pizzaboy")) return 448;
	else if (!strcmp(vstring, "Tram")) return 449;
	else if (!strcmp(vstring, "Article Trailer 2")) return 450;
	else if (!strcmp(vstring, "Turismo")) return 451;
	else if (!strcmp(vstring, "Speeder")) return 452;
	else if (!strcmp(vstring, "Reefer")) return 453;
	else if (!strcmp(vstring, "Tropic")) return 454;
	else if (!strcmp(vstring, "Flatbed")) return 455;
	else if (!strcmp(vstring, "Yankee")) return 456;
	else if (!strcmp(vstring, "Caddy")) return 457;
	else if (!strcmp(vstring, "Solair")) return 458;
	else if (!strcmp(vstring, "Berkley's RC Van")) return 459;
	else if (!strcmp(vstring, "Skimmer")) return 460;
	else if (!strcmp(vstring, "PCJ-600")) return 461;
	else if (!strcmp(vstring, "Faggio")) return 462;
	else if (!strcmp(vstring, "Freeway")) return 463;
	else if (!strcmp(vstring, "RC Baron")) return 464;
	else if (!strcmp(vstring, "RC Raider")) return 465;
	else if (!strcmp(vstring, "Glendale")) return 466;
	else if (!strcmp(vstring, "Oceanic")) return 467;
	else if (!strcmp(vstring, "Sanchez")) return 468;
	else if (!strcmp(vstring, "Sparrow")) return 469;
	else if (!strcmp(vstring, "Patriot")) return 470;
	else if (!strcmp(vstring, "Quad")) return 471;
	else if (!strcmp(vstring, "Coastguard")) return 472;
	else if (!strcmp(vstring, "Dinghy")) return 473;
	else if (!strcmp(vstring, "Hermes")) return 474;
	else if (!strcmp(vstring, "Sabre")) return 475;
	else if (!strcmp(vstring, "Rustler")) return 476;
	else if (!strcmp(vstring, "ZR-350")) return 477;
	else if (!strcmp(vstring, "Walton")) return 478;
	else if (!strcmp(vstring, "Regina")) return 479;
	else if (!strcmp(vstring, "Comet")) return 480;
	else if (!strcmp(vstring, "BMX")) return 481;
	else if (!strcmp(vstring, "Burrito")) return 482;
	else if (!strcmp(vstring, "Camper")) return 483;
	else if (!strcmp(vstring, "Marquis")) return 484;
	else if (!strcmp(vstring, "Baggage")) return 485;
	else if (!strcmp(vstring, "Dozer")) return 486;
	else if (!strcmp(vstring, "Maverick")) return 487;
	else if (!strcmp(vstring, "SAN News Maverick")) return 488;
	else if (!strcmp(vstring, "Rancher")) return 489;
	else if (!strcmp(vstring, "FBI Rancher")) return 490;
	else if (!strcmp(vstring, "Virgo")) return 491;
	else if (!strcmp(vstring, "Greenwood")) return 492;
	else if (!strcmp(vstring, "Jetmax")) return 493;
	else if (!strcmp(vstring, "Hotring Racer")) return 494;
	else if (!strcmp(vstring, "Sandking")) return 495;
	else if (!strcmp(vstring, "Blista Compact")) return 496;
	else if (!strcmp(vstring, "Police Maverick")) return 497;
	else if (!strcmp(vstring, "Boxville")) return 498;
	else if (!strcmp(vstring, "Benson")) return 499;
	else if (!strcmp(vstring, "Mesa")) return 500;
	else if (!strcmp(vstring, "RC Goblin")) return 501;
	else if (!strcmp(vstring, "Hotring Racer")) return 502;
	else if (!strcmp(vstring, "Hotring Racer")) return 503;
	else if (!strcmp(vstring, "Bloodring Banger")) return 504;
	else if (!strcmp(vstring, "Rancher")) return 505;
	else if (!strcmp(vstring, "Super GT")) return 506;
	else if (!strcmp(vstring, "Elegant")) return 507;
	else if (!strcmp(vstring, "Journey")) return 508;
	else if (!strcmp(vstring, "Bike")) return 509;
	else if (!strcmp(vstring, "Mountain Bike")) return 510;
	else if (!strcmp(vstring, "Beagle")) return 511;
	else if (!strcmp(vstring, "Cropduster")) return 512;
	else if (!strcmp(vstring, "Stuntplane")) return 513;
	else if (!strcmp(vstring, "Tanker")) return 514;
	else if (!strcmp(vstring, "Roadtrain")) return 515;
	else if (!strcmp(vstring, "Nebula")) return 516;
	else if (!strcmp(vstring, "Majestic")) return 517;
	else if (!strcmp(vstring, "Buccaneer")) return 518;
	else if (!strcmp(vstring, "Shamal")) return 519;
	else if (!strcmp(vstring, "Hydra")) return 520;
	else if (!strcmp(vstring, "FCR-900")) return 521;
	else if (!strcmp(vstring, "NRG-500")) return 522;
	else if (!strcmp(vstring, "HPV1000")) return 523;
	else if (!strcmp(vstring, "Cement Truck")) return 524;
	else if (!strcmp(vstring, "Towtruck")) return 525;
	else if (!strcmp(vstring, "Fortune")) return 526;
	else if (!strcmp(vstring, "Cadrona")) return 527;
	else if (!strcmp(vstring, "FBI Truck")) return 528;
	else if (!strcmp(vstring, "Willard")) return 529;
	else if (!strcmp(vstring, "Forklift")) return 530;
	else if (!strcmp(vstring, "Tractor")) return 531;
	else if (!strcmp(vstring, "Combine Harvester")) return 532;
	else if (!strcmp(vstring, "Feltzer")) return 533;
	else if (!strcmp(vstring, "Remington")) return 534;
	else if (!strcmp(vstring, "Slamvan")) return 535;
	else if (!strcmp(vstring, "Blade")) return 536;
	else if (!strcmp(vstring, "Freight (Train)")) return 537;
	else if (!strcmp(vstring, "Brownstreak (Train)")) return 538;
	else if (!strcmp(vstring, "Vortex")) return 539;
	else if (!strcmp(vstring, "Vincent")) return 540;
	else if (!strcmp(vstring, "Bullet")) return 541;
	else if (!strcmp(vstring, "Clover")) return 542;
	else if (!strcmp(vstring, "Sadler")) return 543;
	else if (!strcmp(vstring, "Firetruck LA")) return 544;
	else if (!strcmp(vstring, "Hustler")) return 545;
	else if (!strcmp(vstring, "Intruder")) return 546;
	else if (!strcmp(vstring, "Primo")) return 547;
	else if (!strcmp(vstring, "Cargobob")) return 548;
	else if (!strcmp(vstring, "Tampa")) return 549;
	else if (!strcmp(vstring, "Sunrise")) return 550;
	else if (!strcmp(vstring, "Merit")) return 551;
	else if (!strcmp(vstring, "Utility Van")) return 552;
	else if (!strcmp(vstring, "Nevada")) return 553;
	else if (!strcmp(vstring, "Yosemite")) return 554;
	else if (!strcmp(vstring, "Windsor")) return 555;
	else if (!strcmp(vstring, "Monster \"A\"")) return 556;
	else if (!strcmp(vstring, "Monster \"B\"")) return 557;
	else if (!strcmp(vstring, "Uranus")) return 558;
	else if (!strcmp(vstring, "Jester")) return 559;
	else if (!strcmp(vstring, "Sultan")) return 560;
	else if (!strcmp(vstring, "Stratum")) return 561;
	else if (!strcmp(vstring, "Elegy")) return 562;
	else if (!strcmp(vstring, "Raindance")) return 563;
	else if (!strcmp(vstring, "RC Tiger")) return 564;
	else if (!strcmp(vstring, "Flash")) return 565;
	else if (!strcmp(vstring, "Tahoma")) return 566;
	else if (!strcmp(vstring, "Savanna")) return 567;
	else if (!strcmp(vstring, "Bandito")) return 568;
	else if (!strcmp(vstring, "Freight Flat Trailer (Train)")) return 569;
	else if (!strcmp(vstring, "Streak Trailer (Train)")) return 570;
	else if (!strcmp(vstring, "Kart")) return 571;
	else if (!strcmp(vstring, "Mower")) return 572;
	else if (!strcmp(vstring, "Dune")) return 573;
	else if (!strcmp(vstring, "Sweeper")) return 574;
	else if (!strcmp(vstring, "Broadway")) return 575;
	else if (!strcmp(vstring, "Tornado")) return 576;
	else if (!strcmp(vstring, "AT400")) return 577;
	else if (!strcmp(vstring, "DFT-30")) return 578;
	else if (!strcmp(vstring, "Huntley")) return 579;
	else if (!strcmp(vstring, "Stafford")) return 580;
	else if (!strcmp(vstring, "BF-400")) return 581;
	else if (!strcmp(vstring, "Newsvan")) return 582;
	else if (!strcmp(vstring, "Tug")) return 583;
	else if (!strcmp(vstring, "Petrol Trailer")) return 584;
	else if (!strcmp(vstring, "Emperor")) return 585;
	else if (!strcmp(vstring, "Wayfarer")) return 586;
	else if (!strcmp(vstring, "Euros")) return 587;
	else if (!strcmp(vstring, "Hotdog")) return 588;
	else if (!strcmp(vstring, "Club")) return 589;
	else if (!strcmp(vstring, "Freight Box Trailer (Train)")) return 590;
	else if (!strcmp(vstring, "Article Trailer 3")) return 591;
	else if (!strcmp(vstring, "Andromada")) return 592;
	else if (!strcmp(vstring, "Dodo")) return 593;
	else if (!strcmp(vstring, "RC Cam")) return 594;
	else if (!strcmp(vstring, "Launch")) return 595;
	else if (!strcmp(vstring, "Police Car (LSPD)")) return 596;
	else if (!strcmp(vstring, "Police Car (SFPD)")) return 597;
	else if (!strcmp(vstring, "Police Car (LVPD)")) return 598;
	else if (!strcmp(vstring, "Police Ranger")) return 599;
	else if (!strcmp(vstring, "Picador")) return 600;
	else if (!strcmp(vstring, "S.W.A.T.")) return 601;
	else if (!strcmp(vstring, "Alpha")) return 602;
	else if (!strcmp(vstring, "Phoenix")) return 603;
	else if (!strcmp(vstring, "Glendale Shit")) return 604;
	else if (!strcmp(vstring, "Sadler Shit")) return 605;
	else if (!strcmp(vstring, "Baggage Trailer \"A\"")) return 606;
	else if (!strcmp(vstring, "Baggage Trailer \"B\"")) return 607;
	else if (!strcmp(vstring, "Tug Stairs Trailer")) return 608;
	else if (!strcmp(vstring, "Boxville")) return 609;
	else if (!strcmp(vstring, "Farm Trailer")) return 610;
	else if (!strcmp(vstring, "Utility Trailer")) return 611;
	return -1;
}

// Fix the compiler crash when both the PAWN and Plugin versions of sscanf are
// found by renaming the old version at declaration.  (fixes.inc compatible
// naming scheme: "BAD_Function()").
#define sscanf(%0:...) BAD_sscanf(%0:...)
