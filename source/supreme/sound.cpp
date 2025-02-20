#include "winpch.h"
#include "sound.h"
#include "display.h"
#include "repair.h"
#include "progress.h"
#include "music.h"
#include "shop.h"
#include "appdata.h"
#include "string_extras.h"

static soundDesc_t soundInfo[MAX_SOUNDS]={
	{SND_NONE,"No Sound At All!!",ST_EFFECT},
	{SND_MENUSELECT,"Menu Select",ST_INTFACE},
	{SND_PAUSE,"Pause Game",ST_INTFACE},
	{SND_SAVEGAME,"Save Game",ST_INTFACE|ST_VOCAL},
	{SND_LOADGAME,"Load Game",ST_INTFACE|ST_VOCAL},
	{SND_HAMMERBONK,"Hammer Bonk",ST_EFFECT},
	{SND_HAMMERREFLECT,"Hammer Reflect",ST_EFFECT},
	{SND_BULLETREFLECT,"Bullet Reflect",ST_EFFECT},
	{SND_BOMBREFLECT,"Bomb Bounce",ST_EFFECT},
	{SND_MISSILEBOOM,"Missile Boom",ST_EFFECT},
	{SND_ACIDSPLAT,"Acid Splat",ST_EFFECT},
	{SND_ENERGYBONK,"Red Bullet Hit",ST_EFFECT},
	{SND_BOMBBOOM,"Bomb Boom",ST_EFFECT},
	{SND_BULLETHIT,"Bullet Hit",ST_EFFECT},
	{SND_MEGABEAMHIT,"Death Beam Hit",ST_EFFECT},
	{SND_BOMBTHROW,"Bomb Throw",ST_PLAYER|ST_VOCAL},
	{SND_MISSILELAUNCH,"Missile Launch",ST_EFFECT|ST_PLAYER},
	{SND_FLAMEGO,"Flame",ST_EFFECT},
	{SND_BULLETFIRE,"Bullet Fire",ST_EFFECT},
	{SND_KOOLKAT,"Kool Kat Bad",ST_VOCAL|ST_INTFACE|ST_PLAYER},
	{SND_MEGABEAM,"Death Beam",ST_EFFECT},
	{SND_HAMMERTOSS,"Throw Hammer",ST_PLAYER},
	{SND_CHEATWIN,"Cheat!!!",ST_INTFACE|ST_VOCAL},
	{SND_HEALTHCHEAT,"Aaahhh",ST_INTFACE|ST_VOCAL},
	{SND_INVINCCHEAT,"Sickly Yay",ST_VOCAL},
	{SND_DROWN,"Drown",ST_PLAYER|ST_VOCAL},
	{SND_ITEMDROP,"Item Drop",ST_INTFACE},
	{SND_GOTOMAP,"Goto Map",ST_INTFACE},
	{SND_WINLEVEL,"Win Level",ST_INTFACE|ST_VOCAL},
	{SND_WALLDOWN,"Zap Wall",ST_EFFECT},
	{SND_WALLUP,"Make Wall",ST_EFFECT},
	{SND_LIGHTSON,"Lights On",ST_EFFECT},
	{SND_TELEPORT,"Teleport",ST_EFFECT},
	{SND_MESSAGE,"Message Appears",ST_INTFACE},
	{SND_SKELKICK,"Bonehead Kick",ST_MONSTER},
	{SND_SKELSHOOT,"Bonehead Shoot",ST_MONSTER},
	{SND_SKELOUCH,"Bonehead Ouch",ST_MONSTER},
	{SND_SKELDIE,"Bonehead Die",ST_MONSTER},
	{SND_BATEYES,"Bat Attack",ST_MONSTER},
	{SND_BATDIVE,"Bat Dive",ST_MONSTER},
	{SND_BATOUCH,"Bat Ouch",ST_MONSTER},
	{SND_BATDIE,"Bat Die",ST_MONSTER},
	{SND_SPD1OUCH,"",0},
	{SND_SPD1DIE,"Spider Die",ST_MONSTER},
	{SND_SPD1GNAW,"Eensy Bite",ST_MONSTER},
	{SND_SPD2OUCH,"Spider Ouch",ST_MONSTER},
	{SND_SPD2DIE,"Spider Die",ST_MONSTER},
	{SND_SPD2SPIT,"Spider Spit",ST_MONSTER},
	{SND_ZOMBIEOUCH,"Zombie Ouch",ST_MONSTER},
	{SND_ZOMBIEDIE,"Zombie Die",ST_MONSTER},
	{SND_ZOMBIELEAP,"Zombie Leap",ST_MONSTER|ST_VOCAL},
	{SND_MENUCLICK,"Menu Click",ST_INTFACE},
	{SND_EGGSACDIE,"Egg Sac Die",ST_MONSTER},
	{SND_EGGSACBIRTH,"Egg Sac Hatch",ST_MONSTER},
	{SND_SPD3OUCH,"Mama Spider Ouch",ST_MONSTER},
	{SND_SPD3DIE,"Mama Spider Die",ST_MONSTER},
	{SND_SPD3CHOMP,"Mama Spider Bite",ST_MONSTER},
	{SND_SPD3SPEW,"Mama Spider Spit",ST_MONSTER},
	{SND_PYGMYOUCH,"Pygmy Ouch",ST_MONSTER},
	{SND_PYGMYDIE,"Pygmy Die",ST_MONSTER},
	{SND_PYGMYYAWN,"Pygmy Yawn",ST_MONSTER|ST_VOCAL},
	{SND_PYGMYSPIN,"Pygmy Spin",ST_MONSTER},
	{SND_PYGMYSTAB,"Pygmy Stab",ST_MONSTER},
	{SND_PYGMYANGRY,"Unga Bunga",ST_MONSTER|ST_VOCAL},
	{SND_SERPENTOUCH,"Zoid Ouch",ST_MONSTER},
	{SND_SERPENTDIE,"Zoid Die",ST_MONSTER},
	{SND_SERPENTSPIT,"Zoid Spit",ST_MONSTER},
	{SND_MATTIEOUCH,"Matilda Ouch",ST_MONSTER},
	{SND_MATTIETRUEDIE,"",0},
	{SND_MATTIEDIE,"Matilda Die",ST_MONSTER},
	{SND_MATTIECLAW,"Matilda Claw",ST_MONSTER},
	{SND_MATTIESHOOT,"Matilda Shoot",ST_MONSTER},
	{SND_MATTIESTOMP,"Matilda Stomp",ST_MONSTER},
	{SND_GINGEROUCH,"Ninjabread Ouch",ST_MONSTER},
	{SND_GINGERDIE,"Ninjabread Die",ST_MONSTER},
	{SND_GINGERJKICK,"Ninjabread Jumpkick",ST_MONSTER},
	{SND_GINGERCOMBO,"Ninjabread Combo",ST_MONSTER},
	{SND_PUMPKINDIE,"Pumpkin Pop",ST_MONSTER},
	{SND_HAMMERUP,"Hammer Up",ST_INTFACE|ST_VOCAL|ST_PLAYER},
	{SND_PANTS,"Pants!",ST_INTFACE|ST_VOCAL|ST_PLAYER},
	{SND_REVERSE,"",0},
	{SND_REFLECT,"",0},
	{SND_WEAPON,"Get Weapon",ST_INTFACE|ST_PLAYER},
	{SND_FOOD,"Get Food",ST_INTFACE|ST_PLAYER},
	{SND_SHIELD,"Energy Shield",ST_INTFACE|ST_PLAYER},
	{SND_GETBRAIN,"Get Brain",ST_INTFACE|ST_PLAYER},
	{SND_GETKEY,"Get Key",ST_INTFACE|ST_PLAYER},
	{SND_GETKEYCHAIN,"Bingo!",ST_INTFACE|ST_VOCAL|ST_PLAYER},
	{SND_LOONYKEY,"Get Loonykey",ST_INTFACE|ST_PLAYER|ST_VOCAL},
	{SND_BOUAPHAOUCH,"Bouapha Ouch",ST_PLAYER},
	{SND_BOUAPHADIE,"Bouapha Die",ST_PLAYER},
	{SND_BOUAPHABORED,"Ho Hum",ST_PLAYER|ST_VOCAL},
	{SND_DOOROPEN,"Door Open",ST_EFFECT},
	{SND_WORLDTURN,"World Select Turn",ST_INTFACE},
	{SND_WORLDPICK,"Select A World",ST_INTFACE},
	{SND_BLOCKPUSH,"Push A Block",ST_PLAYER},
	{SND_MUSHMAD,"Mush Angry",ST_MONSTER|ST_VOCAL},
	{SND_MUSHMISSILE,"Mush Missile",ST_MONSTER},
	{SND_MUSHSPORES,"Mush Spores",ST_MONSTER},
	{SND_MUSHOUCH,"Mush Ouch",ST_MONSTER},
	{SND_MUSHDIE,"Mush Die",ST_MONSTER},
	{SND_SZGRAB,"Super Zombie BRAINS",ST_MONSTER|ST_VOCAL},
	{SND_BABYSNORE,"Thingie Snore",ST_MONSTER|ST_VOCAL},
	{SND_HAPPYFLEX,"Happy Stick Flex",ST_MONSTER|ST_VOCAL},
	{SND_HAPPYOUCH,"Happy Stick Ouch",ST_MONSTER},
	{SND_HAPPYDIE,"Happy Stick Die",ST_MONSTER},
	{SND_HAPPYCHOMP,"Happy Stick Chomp",ST_MONSTER},
	{SND_THINGVOMIT,"Thing Tongue",ST_MONSTER},
	{SND_ZOMBONIZOOM,"Zomboni Engine",ST_MONSTER},
	{SND_SEALOUCH,"Seal Ouch",ST_MONSTER},
	{SND_SEALDIE,"Seal Die",ST_MONSTER},
	{SND_PENGDIE,"Pengulon Die",ST_MONSTER},
	{SND_PENGPECK,"Pengulon Peck",ST_MONSTER},
	{SND_SNOWOUCH,"Snowguy Ouch",ST_MONSTER},
	{SND_SNOWDIE,"Snowguy Die",ST_MONSTER},
	{SND_SNOWTHROW,"Snowguy Throw",ST_MONSTER},
	{SND_SVENHI,"Yeti: Sven Hello",ST_MONSTER|ST_VOCAL},
	{SND_BJORNHI,"Yeti: Bjorn Hello",ST_MONSTER|ST_VOCAL},
	{SND_YETIOUCH,"Yeti Ouch",ST_MONSTER},
	{SND_YETIDIE,"Yeti Die",ST_MONSTER},
	{SND_YETIYODEL,"Yeti Yodel",ST_MONSTER|ST_VOCAL},
	{SND_CACTONSHOOT,"Cacton Shoot",ST_MONSTER},
	{SND_CACTONDIE,"Cacton Die",ST_MONSTER},
	{SND_DJINNIOUCH,"Djinni Ouch",ST_MONSTER},
	{SND_DJINNIDIE,"Djinni Die",ST_MONSTER},
	{SND_DJINNIPUNCH,"Djinni Punch",ST_MONSTER},
	{SND_DJINNIBOO,"Djinni Boo",ST_MONSTER|ST_VOCAL},
	{SND_LAMPPOOF,"Magic Lamp Steam",ST_MONSTER},
	{SND_LAMPDIE,"Magic Lamp Vanish",ST_MONSTER},
	{SND_MUMBLECRUSH,"Mumble Smack",ST_MONSTER},
	{SND_ROLYPOLYWALL,"Roly Poly Clonk",ST_MONSTER|ST_EFFECT},
	{SND_ROCKBOUNCE,"Rock Bounce",ST_EFFECT},
	{SND_SPHINXSNEEZE,"Sphinxter Sneeze",ST_MONSTER|ST_VOCAL},
	{SND_SPHINXSTOMP,"Sphinxter Stomp",ST_MONSTER},
	{SND_CRAZYSTAB,"Crazy Stab",ST_MONSTER},
	{SND_WACKOOUCH,"Wacko Ouch",ST_MONSTER},
	{SND_WACKODIE,"Wacko Die",ST_MONSTER|ST_VOCAL},
	{SND_BOILERHIT,"Boiler Ouch",ST_MONSTER},
	{SND_GREATPKOUCH,"Great Pumpkin Ouch",ST_MONSTER},
	{SND_GREATPKDIE,"Great Pumpkin Die",ST_MONSTER},
	{SND_GREATPKVOMIT,"Great Pumpkin Spew",ST_MONSTER},
	{SND_DRLLAUGH,"Dr. Lunatic Get 'Em",ST_MONSTER|ST_VOCAL},
	{SND_DRLFEAR,"Dr. Lunatic Scared",ST_MONSTER|ST_VOCAL},
	{SND_DRLOUCH,"Dr. Lunatic Ouch",ST_MONSTER},
	{SND_DRLDIE,"Dr. Lunatic Die",ST_MONSTER},
	{SND_SDZLOUCH,"SDZL Ouch",ST_MONSTER},
	{SND_SDZLDIE,"SDZL Die",ST_MONSTER},
	{SND_SDZLSLASH,"SDZL Slash",ST_MONSTER},
	{SND_SDZLPOUND,"SDZL Ground Pound",ST_MONSTER},
	{SND_SDZLSPIT,"SDZL Spit",ST_MONSTER},
	{SND_SANTAOUCH,"Santa Ouch",ST_MONSTER},
	{SND_SANTADIE,"Santa Die",ST_MONSTER},
	{SND_SANTAHIT,"Santa Smack",ST_MONSTER},
	{SND_MINECART,"Mine Cart Roll",ST_MONSTER|ST_EFFECT},
	{SND_HAMUMU,"Hamumu Theme",ST_INTFACE},
	{SND_VAMPATTK,"Vampire Attack",ST_MONSTER},
	{SND_VAMPFEAR,"Vampire Garlic",ST_MONSTER|ST_VOCAL},
	{SND_VAMPOUCH,"Vampire Ouch",ST_MONSTER},
	{SND_VAMPDIE,"Vampire Die",ST_MONSTER},
	{SND_ARMORSHOOT,"Power Armor Shoot",ST_PLAYER},
	{SND_TRICERAROAR,"Triceroid Roar",ST_MONSTER},
	{SND_CHESTPOUND,"Kongor Chest Pound",ST_MONSTER},
	{SND_ROBOOUCH,"Robot Ouch",ST_MONSTER},
	{SND_ROBODIE,"Robot Die",ST_MONSTER},
	{SND_FACTORYDIE,"Robofactory Die",ST_MONSTER},
	{SND_COFFIN,"Coffin Open",ST_MONSTER|ST_EFFECT|ST_VOCAL},
	{SND_GHOSTOUCH,"Ghost Ouch",ST_MONSTER},
	{SND_GHOSTDIE,"Ghost Die",ST_MONSTER},
	{SND_GHOSTTPORT,"Ghost Teleport",ST_MONSTER},
	{SND_GHOSTYELL,"Ghost Scream",ST_MONSTER},
	{SND_LEFTYOUCH,"Lefty Ouch",ST_MONSTER},
	{SND_LEFTYDIE,"Lefty Die",ST_MONSTER},
	{SND_LEFTYWAVE,"Lefty Hello",ST_MONSTER|ST_VOCAL},
	{SND_UNGADANCE,"Unga Bunga Slow",ST_MONSTER|ST_VOCAL},
	{SND_SHAMANOUCH,"Shaman Ouch",ST_MONSTER},
	{SND_SHAMANDIE,"Shaman Die",ST_MONSTER},
	{SND_SHAMANZAP,"Shaman Zap",ST_MONSTER},
	{SND_SHAMANDANCE,"Unga Bunga High",ST_MONSTER|ST_VOCAL},
	{SND_PKSTEINBUMP,"Pumpkinstein Bump",ST_MONSTER},
	{SND_GLASSBREAK,"Glass Shatter",ST_EFFECT},
	{SND_GLASSHIT,"Stan Glass Ouch",ST_MONSTER},
	{SND_GLASSDIE,"Stan Glass Die",ST_MONSTER|ST_VOCAL},
	{SND_GLASSBLOCK,"Stan Glass Block",ST_MONSTER},
	{SND_TRICEROUCH,"Triceroid Ouch",ST_MONSTER},
	{SND_TRICERDIE,"Triceroid Die",ST_MONSTER},
	{SND_TRICERBUMP,"Triceroid Bump",ST_MONSTER},
	{SND_COUNTOUCH,"Countess Ouch",ST_MONSTER},
	{SND_COUNTDIE,"Countess Die",ST_MONSTER},
	{SND_COUNTHELLO,"Countess Good Eve",ST_MONSTER|ST_VOCAL},
	{SND_COUNTSUMMON,"Countess Arise",ST_MONSTER|ST_VOCAL},
	{SND_COUNTSHOOT,"Countess Shoot",ST_MONSTER},
	{SND_COUNTCHARGE,"Countess Charge",ST_MONSTER},
	{SND_EGGOUCH,"Xeno Egg Ouch",ST_MONSTER},
	{SND_EGGDIE,"Xeno Egg Die",ST_MONSTER},
	{SND_EGGOPEN,"Xeno Egg Hatch",ST_MONSTER},
	{SND_ALIENOUCH,"Xeno Ouch",ST_MONSTER},
	{SND_ALIENDIE,"Xeno Die",ST_MONSTER},
	{SND_ALIENSPIT,"Xeno Spit",ST_MONSTER},
	{SND_INVIDCHARGE,"Shock Trooper Dash",ST_MONSTER},
	{SND_ROBOSHOOT,"Robot Shoot",ST_MONSTER},
	{SND_FACTORYGO,"Robofactory Create",ST_MONSTER},
	{SND_KONGOUCH,"Kongor Ouch",ST_MONSTER},
	{SND_KONGDIE,"Kongor Die",ST_MONSTER},
	{SND_KONGHELLO,"Kongor KONGOR!",ST_MONSTER|ST_VOCAL},
	{SND_LIGHTNING,"Lightning Zap",ST_EFFECT},
	{SND_SLASH,"Machete Slash",ST_EFFECT},
	{SND_CLANG,"Machete Clang",ST_EFFECT},
	{SND_MINELAY,"Mine Drop",ST_PLAYER},
	{SND_RAGE,"RAGE!",ST_PLAYER|ST_INTFACE|ST_VOCAL},
	{SND_SPEEDUP,"Accelerate",ST_EFFECT|ST_INTFACE},
	{SND_ROBOBOUAPHAOUCH,"Power Armor Ouch",ST_PLAYER},
	{SND_ROBOBOUAPHAON,"Power Armor Activate",ST_PLAYER},
	{SND_ULTRAPYGMYOUCH,"Ultrapygmy Ouch",ST_MONSTER},
	{SND_ULTRAPYGMYDIE,"Ultrapygmy Die",ST_MONSTER},
	{SND_ULTRAPYGMYYAWN,"Ultrapygmy Yawn",ST_MONSTER},
	{SND_ULTRAPYGMYSTAB,"Ultrapygmy Stab",ST_MONSTER},
	{SND_ULTRAPYGMYANGRY,"Unga Bunga Quick",ST_MONSTER|ST_VOCAL},
	{SND_ROBOSTOMP,"Loonybot Stomp",ST_MONSTER},
	{SND_SHAMANANGRY,"Shaman Hey",ST_MONSTER|ST_VOCAL},
	{SND_ZAP,"Electrocute",ST_EFFECT},
	{SND_BALLLIGHTNING,"Ball Lightning",ST_MONSTER},
	{SND_VICTORY,"Victory Song",ST_INTFACE},
	{SND_COINGET,"Coin Collect",ST_INTFACE},
	{SND_BATSTARE,"Bat Stare",ST_MONSTER},
	{SND_CANDLEGET,"Candle Get",ST_INTFACE},
	{SND_ALLCANDLE,"Candle All Get",ST_INTFACE},
	{SND_CRAZYDASH,"Crazypants Dash",ST_VOCAL|ST_MONSTER},
	{SND_CRAZYSPIN,"Crazypants Spin",ST_VOCAL|ST_MONSTER},
	{SND_PYGMYCASE,"Pygmy Briefcase",ST_MONSTER},
	{SND_CRAZYMAD,"Crazypants Mad",ST_VOCAL|ST_MONSTER},
	{SND_YERFDOG,"Yerfdog Yerf",ST_MONSTER},
	{SND_SCUBA,"Scuba Tank",ST_INTFACE},
	{SND_BUBBLE,"Bubble Bloop",ST_EFFECT},
	{SND_DIVERANGRY,"Pygmy Diver Unga",ST_VOCAL|ST_MONSTER},
	{SND_DIVERDIE,"Pygmy Diver Die",ST_VOCAL|ST_MONSTER},
	{SND_GLUP,"Drown A Bit",ST_VOCAL|ST_MONSTER},
	{SND_OLAFHI,"Yeti: Olaf Hello",ST_VOCAL|ST_MONSTER},
	{SND_FOLLOWHI,"Bunny Awaken",ST_MONSTER},
	{SND_CARSTART,"Car Start",ST_EFFECT},
	{SND_CARGO,"Car Running",ST_EFFECT},
	{SND_JELLOOUCH,"Jellofish Ouch",ST_MONSTER},
	{SND_KINGDIE,"Zis Is Unacceptable",ST_VOCAL|ST_MONSTER},
	{SND_KINGCONES,"King Cone: Cones",ST_VOCAL|ST_MONSTER},
	{SND_KINGSCOFF,"King Cone: Scoff",ST_VOCAL|ST_MONSTER},
	{SND_FISHDIE,"Glurk!",ST_MONSTER},
	{SND_SUBSTART,"Mini-Sub Startup",ST_EFFECT},
	{SND_SUBSHOOT,"Mini-Sub Torpedo",ST_EFFECT},
	{SND_SUBMINE,"Mini-Sub Depth Charge",ST_EFFECT},
	{SND_OYOYOY,"Punkin Oy Oy Oy!",ST_VOCAL|ST_MONSTER},
	{SND_CUKEDANCE,"Sea Cuke Dance",ST_MONSTER},
	{SND_RUFFIAN,"Sea Cuke Ruffian!",ST_VOCAL|ST_MONSTER},
	{SND_HAVEATYOU,"Sea Cuke Have At You!",ST_VOCAL|ST_MONSTER},
	{SND_CUKEDIE,"Sea Cuke Oh My Heavens",ST_VOCAL|ST_MONSTER},
	{SND_CUKEOUCH,"Sea Cuke Ouch",ST_MONSTER},
	{SND_WILLYIDLE,"Wet Willy Bored",ST_VOCAL|ST_MONSTER},
	{SND_WILLYIDLE2,"Wet Willy Bored 2",ST_VOCAL|ST_MONSTER},
	{SND_BUNNYDIE,"Bunny Die",ST_MONSTER},
	{SND_MINDWIPE,"Mind Control",ST_EFFECT},
	{SND_SHARKATTACK,"Shark Attack",ST_MONSTER},
	{SND_SHARKOUCH,"Shark Ouch",ST_MONSTER},
	{SND_SHARKDIE,"Shark Die",ST_MONSTER},
	{SND_WIZHIT,"Wacky Wiz Alakazam",ST_MONSTER|ST_VOCAL},
	{SND_WIZOUCH,"Wacky Wizard Oy",ST_MONSTER|ST_VOCAL},
	{SND_WIZDIE,"Wacky Wizard Die",ST_MONSTER|ST_VOCAL},
	{SND_HAMMERDN,"Hammer Down",ST_PLAYER|ST_VOCAL|ST_INTFACE},
	{SND_PANTSDN,"Pants Of Futility",ST_PLAYER|ST_VOCAL|ST_INTFACE},
	{SND_LAYEGG,"Xeno Egg Lay",ST_MONSTER},
	{SND_GNOMESEE,"Gnome There He Is",ST_MONSTER|ST_VOCAL},
	{SND_GNOMELAUGH,"Gnome Ha Ha",ST_MONSTER|ST_VOCAL},
	{SND_GENERATE,"Generator",ST_MONSTER},
	{SND_CLONEOUCH,"Evil Clone Ouch",ST_MONSTER},
	{SND_CLONEDIE,"Evil Clone Die",ST_MONSTER},
	{SND_TURRETBZZT,"BZZT",ST_INTFACE},
	{SND_SKELALIVE,"Bonehead Revive",ST_MONSTER},
	{SND_WILLYOUCH,"Wet Willy Ouch",ST_MONSTER},
	{SND_WILLYDIE,"Wet Willy Die",ST_MONSTER},
	{SND_CRABCLICK,"Crab Puff Clack",ST_MONSTER},
	{SND_PATCHDIE,"Pumpkin Patch Die",ST_MONSTER},
	{SND_KINGTICKLE,"King Cone: Tickling",ST_VOCAL|ST_MONSTER},
	{SND_PATTYOUCH,"Crabby Patty Ouch",ST_MONSTER},
	{SND_PATTYDIE,"Crabby Patty Die",ST_MONSTER},
	{SND_BLOWBUBBLE,"Blow A Bubble",ST_EFFECT},
	{SND_PATTYSUMMON,"Crabby Patty: Get Em!",ST_VOCAL|ST_MONSTER},
	{SND_DOZERSPIN,"Dozer Saw Spin",ST_MONSTER},
	{SND_DOZEROUCH,"Dozer Ouch",ST_MONSTER},
	{SND_AMMOCRATE,"Ammo Supreme!",ST_VOCAL|ST_EFFECT},
	{SND_BUBBLEPOP,"Bubble Pop",ST_EFFECT},
	{SND_SCARIEROUCH,"Scarier Bat Ouch",ST_MONSTER},
	{SND_SCARIERDIE,"Scarier Bat Die",ST_MONSTER},
	{SND_SCARIERSPIT,"Scarier Bat Spit",ST_MONSTER},
	{SND_UBEROUCH,"Uberbat Ouch",ST_MONSTER},
	{SND_UBERDIE,"Uberbat Die",ST_MONSTER},
	{SND_UBERSPIT,"Uberbat Spit",ST_MONSTER},
	{SND_WOLFOUCH,"Werepuppy Ouch",ST_MONSTER},
	{SND_WOLFDIE,"Werepuppy Die",ST_MONSTER},
	{SND_WOLFHOWL,"Werepuppy Howl",ST_MONSTER},
	{SND_WOLFPOUNCE,"Werepuppy Pounce",ST_MONSTER},
	{SND_WOLFCHARGE,"Weremutt Charge",ST_MONSTER},
	{SND_CLAP,"Clap",ST_EFFECT},
	{SND_MORONRIGHT,"That's Right!",ST_VOCAL},
	{SND_MORONRIGHT2,"Correct!",ST_VOCAL},
	{SND_MORONWRONG,"Good Try But Wrong",ST_VOCAL},
	{SND_MORONHELLO,"Welcome To Game Show",ST_VOCAL},
	{SND_MORONWIN,"Congratulations!",ST_VOCAL},
	{SND_FREEZE,"Freeze Ray",ST_EFFECT},
	{SND_LUNASHOOT,"LunaChick Shoot",ST_PLAYER},
	{SND_LUNAOUCH,"LunaChick Ouch",ST_PLAYER},
	{SND_LUNADIE,"LunaChick Die",ST_PLAYER},
	{SND_LUNADROWN,"LunaChick Drown",ST_PLAYER},
	{SND_LUNABORED,"LunaChick Bored",ST_PLAYER|ST_VOCAL},
	{SND_LUNABRAINS,"LunaChick Whoohoo!",ST_PLAYER|ST_VOCAL},
	{SND_BUNNYTHEME,"Bunny The Slayer Theme",ST_INTFACE},
	{SND_HAPPYWEEN,"Happy Halloween!",ST_VOCAL|ST_INTFACE},
	{SND_MARKMOVIE,"Mushroom Movie Soundtrack",ST_INTFACE},
	{SND_TZONE,"Spooky Tune",ST_INTFACE},
	{SND_BOING,"Boing",ST_EFFECT},
	{SND_BOING2,"Boing 2",ST_EFFECT},
	{SND_BOING3,"Boing 3",ST_EFFECT},
	{SND_TALLY,"Win Level",ST_INTFACE},
	{SND_SHOPBELL,"Shop Bell",ST_EFFECT|ST_INTFACE},
	{SND_LOCKER,"Open A Locker",ST_INTFACE},
	{SND_BUYITEM,"Buy An Item",ST_INTFACE},
	{SND_TURNEVIL,"Turn Evil",ST_INTFACE},
	{SND_TURNGOOD,"Turn Good",ST_INTFACE},
	{SND_ALLKEYCHAIN,"Bingo-o-o-o",ST_VOCAL|ST_INTFACE},
	{SND_MINIZOIDSPIT,"Microzoid Spit",ST_MONSTER},
	{SND_MINIZOIDDIE,"Microzoid Die",ST_MONSTER},
	{SND_MINIWACKOOUCH,"Little Crazy Ouch",ST_MONSTER},
	{SND_MINIWACKODIE,"Little Crazy Die",ST_MONSTER|ST_VOCAL},
	{SND_JACKATTACK,"Jack Frost Attack",ST_MONSTER},
	{SND_JACKDIE,"Jack Frost Die",ST_MONSTER},
};

static int numSounds,numCustom;
static byte *customSound[MAX_CUSTOM_SOUNDS];
static int32_t customLength[MAX_CUSTOM_SOUNDS];

void InitSound(void)
{
	JamulSoundPurge();
	numSounds=NUM_ORIG_SOUNDS;
	memset(customSound,0,sizeof(byte *)*MAX_CUSTOM_SOUNDS);
	numCustom=0;
}

void ExitSound(void)
{
	ClearCustomSounds();
	// don't need to do nothing, it's handled by jamulsoundexit
}

void ClearCustomSounds(void)
{
	int i;

	for(i=0;i<MAX_CUSTOM_SOUNDS;i++)
	{
		soundInfo[i+CUSTOM_SND_START].theme=0;
		if(customSound[i])
			free(customSound[i]);
		customSound[i]=NULL;
	}
	numCustom=0;
}

int GlobalFlags()
{
	int result = 0;
	if (profile.progress.purchase[modeShopNum[MODE_REVERSE]]&SIF_ACTIVE)
		result |= SND_BACKWARDS;
	if (profile.progress.purchase[modeShopNum[MODE_MANIC]]&SIF_ACTIVE)
		result |= SND_DOUBLESPEED;
	return result;
}

void MakeSound(int snd,int x,int y,int flags,int priority)
{
	long pan,vol;
	int cx,cy;

	if(snd==0)
		return;

	if(!SoundIsAvailable())
		return;
	if(profile.sound==0)
		return;

	GetCamera(&cx,&cy);
	x>>=FIXSHIFT;
	y>>=FIXSHIFT;
	pan=(x-cx)*128/320;
	vol=-(abs(y-cy)*128/240);
	if(pan<-127)
	{
		vol+=(pan+127);
		pan=-127;
	}
	if(pan>127)
	{
		vol-=(pan-127);
		pan=127;
	}
	if(vol<-255)
		return;
	GoPlaySound(snd,pan,vol,flags|GlobalFlags(),priority);
}

void MakeNormalSound(int snd)
{
	if(!SoundIsAvailable())
		return;

	if(profile.sound==0)
		return;

	GoPlaySound(snd,0,0,SND_MAXPRIORITY|SND_CUTOFF|SND_ONE|GlobalFlags(),MAX_SNDPRIORITY);
}

int DescIndexToSound(int descIndex)
{
	// Like a hardcoded version of old soundDesc[customSound].num lookup.
	if (descIndex == 1)
		return 51;
	else if (2 <= descIndex && descIndex <= 51)
		return descIndex - 1;
	else
		return descIndex;
}

int SoundToDescIndex(int sound)
{
	// Reverse of DescIndexToSound.
	if (sound == 51)
		return 1;
	else if (1 <= sound && sound <= 50)
		return sound + 1;
	else
		return sound;
}

soundDesc_t *GetSoundInfo(int snd)
{
	return &soundInfo[snd];
}

int GetNumSounds(void)
{
	return MAX_SOUNDS;
}

int GetNumCustomSounds(void)
{
	return numCustom;
}

byte *GetCustomSound(int n)
{
	return customSound[n];
}

long GetCustomLength(int n)
{
	return customLength[n];
}

byte AddCustomSound(const char *fname)
{
	if(numCustom==MAX_CUSTOM_SOUNDS)
		return 0;

	auto f = AppdataOpen(fname);
	if(!f)
		return 0;
	customLength[numCustom]=SDL_RWsize(f);
	if(customLength[numCustom]<=0)
	{
		return 0;
	}
	customSound[numCustom]=(byte *)malloc(customLength[numCustom]);
	if(!customSound[numCustom])
	{
		return 0;
	}
	SDL_RWread(f,customSound[numCustom],sizeof(byte),customLength[numCustom]);
	f.reset();

	std::string_view fname2 = fname;
	size_t slash = fname2.rfind('/');
	if (slash != std::string_view::npos)
		fname2 = fname2.substr(slash + 1);
	size_t dot = fname2.rfind('.');
	if (dot != std::string_view::npos)
		fname2 = fname2.substr(0, dot);

	ham_strcpy(soundInfo[CUSTOM_SND_START+numCustom].name, fname2);
	soundInfo[CUSTOM_SND_START+numCustom].num=CUSTOM_SND_START+numCustom;
	soundInfo[CUSTOM_SND_START+numCustom].theme=ST_CUSTOM;

	numCustom++;
	return 1;
}

soundDesc_t *AddCustomSound(byte *data, size_t length)
{
	if(numCustom==MAX_CUSTOM_SOUNDS)
		return nullptr;

	customSound[numCustom] = data;
	customLength[numCustom] = length;

	strcpy(soundInfo[CUSTOM_SND_START+numCustom].name,"New Sound");
	soundInfo[CUSTOM_SND_START+numCustom].num=CUSTOM_SND_START+numCustom;
	soundInfo[CUSTOM_SND_START+numCustom].theme=ST_CUSTOM;
	return &soundInfo[CUSTOM_SND_START + (numCustom++)];
}

byte ReplaceCustomSound(int n,const char *fname)
{
	if(customSound[n])
		free(customSound[n]);
	customSound[n] = nullptr;

	auto f = AppdataOpen(fname);
	if(!f)
		return 0;
	customLength[n]=SDL_RWsize(f);
	if(customLength[n]<=0)
	{
		return 0;
	}
	customSound[n]=(byte *)malloc(customLength[n]);
	if(!customSound[n])
	{
		return 0;
	}
	SDL_RWread(f,customSound[n],sizeof(byte),customLength[n]);
	f.reset();

	return 1;
}

void DeleteCustomSound(int n)
{
	int i;

	free(customSound[n]);

	for(i=n;i<MAX_CUSTOM_SOUNDS-1;i++)
	{
		customSound[i]=customSound[i+1];
		customLength[i]=customLength[i+1];
		soundInfo[CUSTOM_SND_START+i]=soundInfo[CUSTOM_SND_START+i+1];
		soundInfo[CUSTOM_SND_START+i].num=CUSTOM_SND_START+i;
	}
	customSound[MAX_CUSTOM_SOUNDS-1]=NULL;
	soundInfo[CUSTOM_SND_START+MAX_CUSTOM_SOUNDS-1].theme=0;
	numCustom--;

	JamulSoundPurge();
	RepairCustomSounds(CUSTOM_SND_START+n);	// all references to sound N become null references, and all
											// references above N move down one
}

void SaveCustomSounds(SDL_RWops *f)
{
	int i;

	SDL_RWwrite(f,&numCustom,1,sizeof(int));
	for(i=0;i<numCustom;i++)
	{
		SDL_RWwrite(f,&soundInfo[CUSTOM_SND_START+i],1,sizeof(soundDesc_t));	// write out the descriptor
		SDL_RWwrite(f,&customLength[i],sizeof(int32_t),1);	// write out the data length
		SDL_RWwrite(f,customSound[i],sizeof(byte),customLength[i]);	// write out the data
	}
}

void LoadCustomSounds(SDL_RWops *f)
{
	int i;

	ClearCustomSounds();

	SDL_RWread(f,&numCustom,1,sizeof(int));
	for(i=0;i<numCustom;i++)
	{
		SDL_RWread(f,&soundInfo[CUSTOM_SND_START+i],1,sizeof(soundDesc_t));
		SDL_RWread(f,&customLength[i],sizeof(int32_t),1);
		customSound[i]=(byte *)malloc(customLength[i]);
		SDL_RWread(f,customSound[i],sizeof(byte),customLength[i]);
	}
}

int AppendCustomSounds(SDL_RWops *f)
{
	int i,more,start;

	SDL_RWread(f,&more,1,sizeof(int));

	if(numCustom+more>MAX_CUSTOM_SOUNDS)
		return -1;

	start=numCustom;
	numCustom+=more;
	for(i=start;i<numCustom;i++)
	{
		SDL_RWread(f,&soundInfo[CUSTOM_SND_START+i],1,sizeof(soundDesc_t));
		soundInfo[CUSTOM_SND_START+i].num=CUSTOM_SND_START+i;
		SDL_RWread(f,&customLength[i],sizeof(int32_t),1);
		customSound[i]=(byte *)malloc(customLength[i]);
		SDL_RWread(f,customSound[i],sizeof(byte),customLength[i]);
	}

	return (start);
}

int GetCustomSoundByName(const char *name)
{
	int i;

	for(i=0;i<numCustom;i++)
	{
		if(!strcmp(soundInfo[CUSTOM_SND_START+i].name,name))
			return i+CUSTOM_SND_START;
	}

	return -1;
}

void MakeSpaceSound(int snd,int priority)
{
	if(!SoundIsAvailable())
		return;
	if(profile.sound==0)
		return;

	GoPlaySound(snd,0,0,SND_CUTOFF|GlobalFlags(),priority);
}

owned::SDL_RWops SoundLoadOverride(int num)
{
	if (num >= CUSTOM_SND_START && num < CUSTOM_SND_START+GetNumCustomSounds())
	{
		byte* buf = GetCustomSound(num - CUSTOM_SND_START);
		if (!buf)
			return nullptr;

		return owned::SDL_RWFromConstMem(buf, GetCustomLength(num - CUSTOM_SND_START));
	}

	return nullptr;
}
