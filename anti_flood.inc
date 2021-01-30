#include <a_samp>
#define IP_LIMIT 10 // = Max connections from one single IP
#define SAME_IP_CONNECT 4 // = The number of connects from the same IP before banning the flooder
new Same_IP=0,Join_Stamp,ban_s[25],exceed=0;
#define Time_Limit 3500 // = The time span between connects, adjust it to your own specifications

stock flood_OnPlayerConnect(playerid)
{
	new ConnIP[16];
	GetPlayerIp(playerid,ConnIP,16);
	new compare_IP[16];
	new number_IP = 0;
	for(new i=0; i<MAX_PLAYERS; i++) {
		if(IsPlayerConnected(i)) {
		    GetPlayerIp(i,compare_IP,16);
		    if(!strcmp(compare_IP,ConnIP)) number_IP++;
		}
	}
	if((GetTickCount() - Join_Stamp) < Time_Limit)
	    exceed=1;
	else
	    exceed=0;
 	if(strcmp(ban_s, ConnIP, false) == 0 && exceed == 1 )
 	{
 	    Same_IP++;
 	    if(Same_IP > SAME_IP_CONNECT)
 	    {
                         new entry[256],
                         file[20]="flood_bans.txt";
      	                 new lday,lmonth,lyear,lhour,lminute,lsecond;
      	                 gettime(lhour,lminute,lsecond);
      	                 getdate(lyear,lmonth,lday);
      	                 new date[128];
      	                 format(date,sizeof(date),"[%02d.%02d.%04d %02d:%02d:%02d]: ",lday,lmonth,lyear,lhour,lminute,lsecond);
      	                 format(entry, sizeof(entry), "%s%s wurde wegen Flooding gebannt. \r\n",date,SpielerName(playerid));
      	                 new File:hFile;
      	                 hFile = fopen(file, io_append);
      	                 fwrite(hFile, entry);
      	                 fclose(hFile);
	   		Ban(playerid);
 			Same_IP=0;
 	    }
 	}
 	else
 	{
 		Same_IP=0;
	}
	if(number_IP > IP_LIMIT)
	    Kick(playerid);
	GetStampIP(playerid);
	return 1;

}
stock GetStampIP(playerid){
	new S_IP[16];
	Join_Stamp=GetTickCount();
	GetPlayerIp(playerid,S_IP,16);
	format(ban_s, 16, "%s", S_IP);
}