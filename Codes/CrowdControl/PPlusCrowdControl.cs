using System;
using System.Collections.Generic;
using CrowdControl.Common;
using CrowdControl.Games.Packs;
using ConnectorType = CrowdControl.Common.ConnectorType;

public class PPlus : SimpleTCPPack
{
    public override string Host { get; } = "127.0.0.1";

    public override ushort Port { get; } = 43384;

    public PPlus(IPlayer player, Func<CrowdControlBlock, bool> responseHandler, Action<object> statusUpdateHandler) : base(player, responseHandler, statusUpdateHandler) { }

    public override Game Game { get; } = new Game(90, "Project+", "PPlus", "PC", ConnectorType.SimpleTCPConnector);

    private enum ItemIds : int
    {
        Assist_Trophy = 0x00,
		Franklin_Badge = 0x01,
		Banana_Peel = 0x02,
		Barrel = 0x03,
		Beam_Sword = 0x04,
		Bill_Coin_mode = 0x05,
		Bob_Omb = 0x06,
		Crate = 0x07,
		Bumper = 0x08,
		Capsule = 0x09,
		Rolling_Crate = 0x0A,
		CD = 0x0B,
		Gooey_Bomb = 0x0C,
		Cracker_Launcher = 0x0D,
		Cracker_Launcher_Shot = 0x0E,
		Coin = 0x0F,
		Superspicy_Curry = 0x10,
		Superspice_Curry_Shot = 0x11,
		Deku_Nut = 0x12,
		Mr_Saturn = 0x13,
		Dragoon_Part = 0x14,
		Dragoon_Set = 0x15,
		Dragoon_Sight = 0x16,
		Trophy = 0x17,
		Fire_Flower = 0x18,
		Fire_Flower_Shot = 0x19,
		Freezie = 0x1A,
		Golden_Hammer = 0x1B,
		Green_Shell = 0x1C,
		Hammer = 0x1D,
		Hammer_Head = 0x1E,
		Fan = 0x1F,
		Heart_Container = 0x20,
		Homerun_Bat = 0x21,
		Party_Ball = 0x22,
		Manaphy_Heart = 0x23,
		Maxim_Tomato = 0x24,
		Poison_Mushroom = 0x25,
		Super_Mushroom = 0x26,
		Metal_Box = 0x27,
		Hothead = 0x28,
		Pitfall = 0x29,
		Pokeball = 0x2A,
		Blast_Box = 0x2B,
		Ray_Gun = 0x2C,
		Ray_Gun_Shot = 0x2D,
		Lipstick = 0x2E,
		Lipstick_Flower = 0x2F,
		Lipstick_Shot_Dust_Powder = 0x30,
		Sandbag = 0x31,
		Screw_Attack = 0x32,
		Sticker = 0x33,
		Motion_Sensor_Bomb = 0x34,
		Timer = 0x35,
		Smart_Bomb = 0x36,
		Smash_Ball = 0x37,
		Smoke_Screen = 0x38,
		Spring = 0x39,
		Star_Rod = 0x3A,
		Star_Rod_Shot = 0x3B,
		Soccer_Ball = 0x3C,
		Super_Scope = 0x3D,
		Super_Scope_shot = 0x3E,
		Star = 0x3F,
		Food = 0x40,
		Team_Healer = 0x41,
		Lightning = 0x42,
		Unira = 0x43,
		Bunny_Hood = 0x44,
		Warpstar = 0x45,
		Trophy_SSE = 0x46,
		Key = 0x47,
		Trophy_Stand = 0x48,
		Stock_Ball = 0x49,
		Apple_Green_Greens = 0x4A,
		Sidestepper = 0x4B,
		Shellcreeper = 0x4C,
		Pellet = 0x4D,
		Vegetable_Summit = 0x4E,
		Sandbag_HRC = 0x4F,
		Auroros = 0x50,
		Koopa1 = 0x51,
		Koopa2 = 0x52,
		Snakes_Box = 0x53,
		Diddys_Peanut = 0x54,
		Links_Bomb = 0x55,
		Peachs_Turnup = 0x56,
		ROBs_Gyro = 0x57,
		Seed_edible_peanut = 0x58,
		Snakes_Grenade = 0x59,
		Samus_Armor_piece = 0x5A,
		Toon_Links_Bomb = 0x5B,
		Warios_Bike = 0x5C,
		Warios_Bike_A = 0x5D,
		Warios_Bike_B = 0x5E,
		Warios_Bike_C = 0x5F,
		Warios_Bike_D = 0x60,
		Warios_Bike_E = 0x61,
		Torchic = 0x62,
		Cerebi = 0x63,
		Chickorita = 0x64,
		Chickoritas_Shot = 0x65,
		Entei = 0x66,
		Moltres = 0x67,
		Munchlax = 0x68,
		Deoxys = 0x69,
		Groudon = 0x6A,
		Gulpin = 0x6B,
		Staryu = 0x6C,
		Staryus_Shot = 0x6D,
		Ho_oh = 0x6E,
		Ho_ohs_Shot = 0x6F,
		Jirachi = 0x70,
		Snorlax = 0x71,
		Bellossom = 0x72,
		Kyogre = 0x73,
		Kyogres_Shot = 0x74,
		Latias_and_Latios = 0x75,
		Lugia = 0x76,
		Lugias_Shot = 0x77,
		Manaphy = 0x78,
		Weavile = 0x79,
		Electrode = 0x7A,
		Metagross = 0x7B,
		Mew = 0x7C,
		Meowth = 0x7D,
		Meowths_Shot = 0x7E,
		Piplup = 0x7F,
		Togepi = 0x80,
		Goldeen = 0x81,
		Gardevoir = 0x82,
		Wobuffet = 0x83,
		Suicune = 0x84,
		Bonsly = 0x85,
		Andross = 0x86,
		Andross_Shot = 0x87,
		Barbara = 0x88,
		Gray_Fox = 0x89,
		Ray_MKII_Custom_Robo = 0x8A,
		Ray_MKII_Bomb = 0x8B,
		Ray_MKII_Gun_Shot = 0x8C,
		Samurai_Goroh = 0x8D,
		Devil = 0x8E,
		Excitebike = 0x8F,
		Jeff_Andonuts = 0x90,
		Jeff_Pencil_Bullet = 0x91,
		Jeff_Pencil_Rocket = 0x92,
		Lakitu = 0x93,
		Knuckle_Joe = 0x94,
		Knuckle_Joe_Shot = 0x95,
		Hammer_Bro = 0x96,
		Hammer_Bro_Hammer = 0x97,
		Helirin = 0x98,
		Kat_and_Ana = 0x99,
		Ana = 0x9A,
		Jill_and_Drill_Dozer = 0x9B,
		Lyn = 0x9C,
		Little_Mac = 0x9D,
		Metroid = 0x9E,
		Nintendog = 0x9F,
		NintendogFull = 0xA0,
		Mr_Resetti = 0xA1,
		Isaac = 0xA2,
		Isaac_Shot = 0xA3,
		Saki_Amamiya = 0xA4,
		Saki_Shot_1 = 0xA5,
		Saki_Shot_2 = 0xA6,
		Shadow_the_Hedgehog = 0xA7,
		Infantry = 0xA8,
		Infantry_Shot = 0xA9,
		Stafy = 0xAA,
		Tank_Infantry = 0xAB,
		Tank_Shot = 0xAC,
		Tingle = 0xAD,
		togezo = 0xAE,
		Waluigi = 0xAF,
		Dr_Wright = 0xB0,
		Wright_Buildings = 0xB1,
		Unknown1 = 0x7D1,
		Unknown2 = 0x7D2,
		Unknown3 = 0x7D3,
		Unknown4 = 0x7D4,
		Unknown5 = 0x7D5,
		Random = 0xFF
    }
	
	private Effect createEffect(string name, string code, int codeID, string[] parameters)
	{
		return new Effect(name, code + "_" + codeID.ToString(), parameters);
	}
	
	private Effect createEffect(string name, string code, int codeID, string[] parameters, string parent)
	{
		return new Effect(name, code + "_" + codeID.ToString(), parameters, parent);
	}

	private Effect createEffect(string name, string code, int codeID)
    {
        return new Effect(name, code + "_" + codeID.ToString());
    }

	private Effect createEffect(string name, string code, int codeID, string parent)
    {
    	return new Effect(name, code + "_" + codeID.ToString(), parent);
    }
	
	private Effect createEffectSubItem(string name, string code, int codeID, string parent)
	{
		return new Effect(name, code + "_" + codeID.ToString(), ItemKind.Usable, parent);
	}
	
	private Effect createEffectFolder(string name, string code)
	{
		return new Effect(name, code, ItemKind.Folder);
	}
	
	private Effect createEffectFolder(string name, string code, string parent)
	{
		return new Effect(name, code, ItemKind.Folder, parent);
	}
	
	
	public override List<Effect> Effects => new List<Effect>
    {	
		
        // General Effects
        createEffect("Trigger the Killswitch", "kill", 3, new[]{"players"}),
        createEffect("Give Stocks", "give_stocks", 4, new[]{"players", "#stocks"}),
		createEffect("Remove Stocks", "remove_stocks", 5, new[]{"players", "#stocks"}),
		
		// Items
		createEffectFolder("Items", "items"),
		
		/// Spawn Items
		createEffectFolder("Spawn", "items_spawn", "items"),
		
		//// Spawn Regular Items
		createEffect("Regular", "items_spawn_reg", 6, new[]{"items_reg", "#items"}, "items_spawn"),
		
		//// Spawn Special Items
		//new Effect("Special Items", "items_spawn_sp", new[]{"items_sp"}, "items_spawn"),
		
		//// Spawn Pokemon 
		createEffect("Pokemon", "items_spawn_pkmn", 7, new[]{"items_pkmn", "#pkmn"}, "items_spawn"),
		
		//// Spawn Assist Trophies
		createEffect("Assist", "items_spawn_assist", 8, new[]{"items_assist", "#assist"}, "items_spawn"),
		
		/// TODO: Item rain selection

		// Status Effects

		createEffectFolder("Status", "status");

		createEffect("Metal", "status_metal", 9, new[]{"players", "%percent"}, "status");
		createEffect("Curry", "status_curry", 10, new[]{"players"}, "status");
		createEffect("Hammer", "status_hammer", 11, new[]{"players", "set"}, "status");
		createEffect("SuperStar", "status_superstar", 12, new[]{"players"}, "status");
		createEffect("Flower", "status_flower", 13, new[]{"players", "flower_rate", "flower_size"}, "status");
		createEffect("Heart", "status_heart", 14, new[]{"players", "players"}, "status");
		createEffect("Slow", "status_slow", 15, new[]{"players", "slow_duration"}, "status");
		createEffect("Mushroom", "status_mushroom", 16, new[]{"players", "mushroom"}, "status");
		createEffect("Bunny Hood", "status_bunnyhood", 17, new[]{"players"}, "status");
		createEffect("Franklin Badge", "status_franklinbadge", 18, new[]{"players"}, "status");
		createEffect("Screw Attack", "status_screwattack", 19, new[]{"players"}, "status");

		// Dropdown List Selections
		
		/// Players
		createEffectSubItem("Player 1", "player", 0, "players"),
		createEffectSubItem("Player 2", "player", 1, "players"),
		createEffectSubItem("Player 3", "player", 2, "players"),
		createEffectSubItem("Player 4", "player", 3, "players"),
		createEffectSubItem("Player Random", "player_random", 4, "players"),
		createEffectSubItem("Player All", "player_all", 5, "players"),

		/// Set
		createEffectSubItem("False", "set_0", 0, "set"),
		createEffectSubItem("True", "set_1", 1, "set"),

		/// Mushroom
        createEffectSubItem("Super", "mushroom_0", 0, "mushroom"),
        createEffectSubItem("Poison", "mushroom_1", 1, "mushroom"),
		
		/// Regular Items
		createEffectSubItem("Assist Trophy", "item_reg", (int)ItemIds.Assist_Trophy, "items_reg"),
		createEffectSubItem("Franklin Badge", "item_reg", (int)ItemIds.Franklin_Badge, "items_reg"),
		createEffectSubItem("Banana Peel", "item_reg", (int)ItemIds.Banana_Peel, "items_reg"),
		createEffectSubItem("Barrel", "item_reg", (int)ItemIds.Barrel, "items_reg"),
		createEffectSubItem("Beam Sword", "item_reg", (int)ItemIds.Beam_Sword, "items_reg"),
		//createEffectSubItem("Bill Coin mode", "item_reg", (int)ItemIds.Bill_Coin_mode, "items_reg"),
		createEffectSubItem("Bob-omb", "item_reg", (int)ItemIds.Bob_Omb, "items_reg"),
		createEffectSubItem("Crate", "item_reg", (int)ItemIds.Crate, "items_reg"),
		createEffectSubItem("Bumper", "item_reg", (int)ItemIds.Bumper, "items_reg"),
		createEffectSubItem("Capsule", "item_reg", (int)ItemIds.Capsule, "items_reg"),
		createEffectSubItem("Rolling Crate", "item_reg", (int)ItemIds.Rolling_Crate, "items_reg"),
		//createEffectSubItem("CD", "item_reg", (int)ItemIds.CD, "items_reg"),
		createEffectSubItem("Gooey Bomb", "item_reg", (int)ItemIds.Gooey_Bomb, "items_reg"),
		createEffectSubItem("Cracker Launcher", "item_reg", (int)ItemIds.Cracker_Launcher, "items_reg"),
		createEffectSubItem("Cracker Launcher Shot", "item_reg", (int)ItemIds.Cracker_Launcher_Shot, "items_reg"),
		//createEffectSubItem("Coin", "item_reg", (int)ItemIds.Coin, "items_reg"),
		createEffectSubItem("Superspicy Curry", "item_reg", (int)ItemIds.Superspicy_Curry, "items_reg"),
		createEffectSubItem("Superspice Curry Shot", "item_reg", (int)ItemIds.Superspice_Curry_Shot, "items_reg"),
		createEffectSubItem("Deku Nut", "item_reg", (int)ItemIds.Deku_Nut, "items_reg"),
		createEffectSubItem("Mr. Saturn", "item_reg", (int)ItemIds.Mr_Saturn, "items_reg"),
		createEffectSubItem("Dragoon Part", "item_reg", (int)ItemIds.Dragoon_Part, "items_reg"),
		//createEffectSubItem("Dragoon Set", "item_reg", (int)ItemIds.Dragoon_Set, "items_reg"),
		//createEffectSubItem("Dragoon Sight", "item_reg", (int)ItemIds.Dragoon_Sight, "items_reg"),
		//createEffectSubItem("Trophy", Trophy, "item_reg", (int)ItemIds.ItemKind.Usable).ToString(), "items_reg"),
		createEffectSubItem("Fire Flower", "item_reg", (int)ItemIds.Fire_Flower, "items_reg"),
		createEffectSubItem("Fire Flower Shot", "item_reg", (int)ItemIds.Fire_Flower_Shot, "items_reg"),
		createEffectSubItem("Freezie", "item_reg", (int)ItemIds.Freezie, "items_reg"),
		createEffectSubItem("Golden Hammer", "item_reg", (int)ItemIds.Golden_Hammer, "items_reg"),
		createEffectSubItem("Green Shell", "item_reg", (int)ItemIds.Green_Shell, "items_reg"),
		createEffectSubItem("Hammer", "item_reg", (int)ItemIds.Hammer, "items_reg"),
		createEffectSubItem("Hammer Head", "item_reg", (int)ItemIds.Hammer_Head, "items_reg"),
		createEffectSubItem("Fan", "item_reg", (int)ItemIds.Fan, "items_reg"),
		createEffectSubItem("Heart Container", "item_reg", (int)ItemIds.Heart_Container, "items_reg"),
		createEffectSubItem("Homerun Bat", "item_reg", (int)ItemIds.Homerun_Bat, "items_reg"),
		createEffectSubItem("Party Ball", "item_reg", (int)ItemIds.Party_Ball, "items_reg"),
		//createEffectSubItem("Manaphy Heart", "item_reg", (int)ItemIds.Manaphy_Heart, "items_reg"),
		createEffectSubItem("Maxim Tomato", "item_reg", (int)ItemIds.Maxim_Tomato, "items_reg"),
		createEffectSubItem("Poison Mushroom", "item_reg", (int)ItemIds.Poison_Mushroom, "items_reg"),
		createEffectSubItem("Super Mushroom", "item_reg", (int)ItemIds.Super_Mushroom, "items_reg"),
		createEffectSubItem("Metal Box", "item_reg", (int)ItemIds.Metal_Box, "items_reg"),
		createEffectSubItem("Hothead", "item_reg", (int)ItemIds.Hothead, "items_reg"),
		createEffectSubItem("Pitfall", "item_reg", (int)ItemIds.Pitfall, "items_reg"),
		createEffectSubItem("Pokeball", "item_reg", (int)ItemIds.Pokeball, "items_reg"),
		createEffectSubItem("Blast Box", "item_reg", (int)ItemIds.Blast_Box, "items_reg"),
		createEffectSubItem("Ray Gun", "item_reg", (int)ItemIds.Ray_Gun, "items_reg"),
		createEffectSubItem("Ray Gun Shot", "item_reg", (int)ItemIds.Ray_Gun_Shot, "items_reg"),
		createEffectSubItem("Lipstick", "item_reg", (int)ItemIds.Lipstick, "items_reg"),
		//createEffectSubItem("Lipstick Flower", "item_reg", (int)ItemIds.Lipstick_Flower, "items_reg"),
		//createEffectSubItem("Lipstick Shot Dust Powder", "item_reg", (int)ItemIds.Lipstick_Shot_Dust_Powder, "items_reg"),
		createEffectSubItem("Sandbag", "item_reg", (int)ItemIds.Sandbag, "items_reg"),
		createEffectSubItem("Screw Attack", "item_reg", (int)ItemIds.Screw_Attack, "items_reg"),
		//createEffectSubItem("Sticker", "item_reg", (int)ItemIds.Sticker, "items_reg"),
		createEffectSubItem("Motion Sensor Bomb", "item_reg", (int)ItemIds.Motion_Sensor_Bomb, "items_reg"),
		createEffectSubItem("Timer", "item_reg", (int)ItemIds.Timer, "items_reg"),
		createEffectSubItem("Smart Bomb", "item_reg", (int)ItemIds.Smart_Bomb, "items_reg"),
		createEffectSubItem("Smash Ball", "item_reg", (int)ItemIds.Smash_Ball, "items_reg"),
		createEffectSubItem("Smoke Screen", "item_reg", (int)ItemIds.Smoke_Screen, "items_reg"),
		createEffectSubItem("Spring", "item_reg", (int)ItemIds.Spring, "items_reg"),
		createEffectSubItem("Star Rod", "item_reg", (int)ItemIds.Star_Rod, "items_reg"),
		createEffectSubItem("Star Rod Shot", "item_reg", (int)ItemIds.Star_Rod_Shot, "items_reg"),
		createEffectSubItem("Soccer Ball", "item_reg", (int)ItemIds.Soccer_Ball, "items_reg"),
		createEffectSubItem("Super Scope", "item_reg", (int)ItemIds.Super_Scope, "items_reg"),
		createEffectSubItem("Super Scope Shot", "item_reg", (int)ItemIds.Super_Scope_shot, "items_reg"),
		createEffectSubItem("Star", "item_reg", (int)ItemIds.Star, "items_reg"),
		createEffectSubItem("Food", "item_reg", (int)ItemIds.Food, "items_reg"),
		createEffectSubItem("Team Healer", "item_reg", (int)ItemIds.Team_Healer, "items_reg"),
		createEffectSubItem("Lightning", "item_reg", (int)ItemIds.Lightning, "items_reg"),
		createEffectSubItem("Unira", "item_reg", (int)ItemIds.Unira, "items_reg"),
		createEffectSubItem("Bunny Hood", "item_reg", (int)ItemIds.Bunny_Hood, "items_reg"),
		createEffectSubItem("Warpstar", "item_reg", (int)ItemIds.Warpstar, "items_reg"),
		createEffectSubItem("Random", "item_reg", (int)ItemIds.Random, "items_reg"),
		
		/// Special Items
		//createEffectSubItem("Trophy SSE", "item_sp", (int)ItemIds.Trophy_SSE, "items_sp"),
		//createEffectSubItem("Key", "item_sp", (int)ItemIds.Key, "items_sp"),
		//createEffectSubItem("Trophy Stand", "item_sp", (int)ItemIds.Trophy_Stand, "items_sp"),
		//createEffectSubItem("Stock Ball", "item_sp", (int)ItemIds.Stock_Ball, "items_sp"),
		//createEffectSubItem("Apple Green Greens", "item_sp", (int)ItemIds.Apple_Green_Greens, "items_sp"),
		//createEffectSubItem("Sidestepper", "item_sp", (int)ItemIds.Sidestepper, "items_sp"),
		//createEffectSubItem("Shellcreeper", "item_sp", (int)ItemIds.Shellcreeper, "items_sp"),
		//createEffectSubItem("Pellet", "item_sp", (int)ItemIds.Pellet, "items_sp"),
		//createEffectSubItem("Vegetable Summit", "item_sp", (int)ItemIds.Vegetable_Summit, "items_sp"),
		//createEffectSubItem("Sandbag HRC", "item_sp", (int)ItemIds.Sandbag_HRC, "items_sp"),
		//createEffectSubItem("Auroros", "item_sp", (int)ItemIds.Auroros, "items_sp"),
		//createEffectSubItem("Koopa1", "item_sp", (int)ItemIds.Koopa1, "items_sp"),
		//createEffectSubItem("Koopa2", "item_sp", (int)ItemIds.Koopa2, "items_sp"),
		//createEffectSubItem("Snake's Box", "item_sp", (int)ItemIds.Snakes_Box, "items_sp"),
		//createEffectSubItem("Diddy's Peanut", "item_sp", (int)ItemIds.Diddys_Peanut, "items_sp"),
		//createEffectSubItem("Link's Bomb", "item_sp", (int)ItemIds.Links_Bomb, "items_sp"),
		//createEffectSubItem("Peach's Turnup", "item_sp", (int)ItemIds.Peachs_Turnup, "items_sp"),
		//createEffectSubItem("ROB's Gyro", "item_sp", (int)ItemIds.ROBs_Gyro, "items_sp"),
		//createEffectSubItem("Seed Edible "item_sp", (int)ItemIds.Peanut).ToString()", Seed_edible_peanut, ItemKind.Usable, "items_sp"),
		//createEffectSubItem("Snakes' Grenade", "item_sp", (int)ItemIds.Snakes_Grenade, "items_sp"),
		//createEffectSubItem("Samus' Armor Piece", "item_sp", (int)ItemIds.Samus_Armor_piece, "items_sp"),
		//createEffectSubItem("Toon Link's Bomb", "item_sp", (int)ItemIds.Toon_Links_Bomb, "items_sp"),
		//createEffectSubItem("Wario's Bike", "item_sp", (int)ItemIds.Warios_Bike, "items_sp"),
		//createEffectSubItem("Wario's Bike A", "item_sp", (int)ItemIds.Warios_Bike_A, "items_sp"),
		//createEffectSubItem("Wario's Bike B", "item_sp", (int)ItemIds.Warios_Bike_B, "items_sp"),
		//createEffectSubItem("Wario's Bike C", "item_sp", (int)ItemIds.Warios_Bike_C, "items_sp"),
		//createEffectSubItem("Wario's Bike D", "item_sp", (int)ItemIds.Warios_Bike_D, "items_sp"),
		//createEffectSubItem("Wario's Bike E", "item_sp", (int)ItemIds.Warios_Bike_E, "items_sp"),
		//createEffectSubItem("Unknown1", "item_sp", (int)ItemIds.Unknown1, "items_sp"),
		//createEffectSubItem("Unknown2", "item_sp", (int)ItemIds.Unknown2, "items_sp"),
		//createEffectSubItem("Unknown3", "item_sp", (int)ItemIds.Unknown3, "items_sp"),
		//createEffectSubItem("Unknown4", "item_sp", (int)ItemIds.Unknown4, "items_sp"),
		//createEffectSubItem("Unknown5", "item_sp", (int)ItemIds.Unknown5, "items_sp"),
		
		/// Pokemon
		createEffectSubItem("Torchic", "item_pkmn", (int)ItemIds.Torchic, "items_pkmn"),
		//createEffectSubItem("Celebi", "item_pkmn", (int)ItemIds.Cerebi, "items_pkmn"),
		createEffectSubItem("Chickorita", "item_pkmn", (int)ItemIds.Chickorita, "items_pkmn"),
		createEffectSubItem("Chickorita's Shot", "item_pkmn", (int)ItemIds.Chickoritas_Shot, "items_pkmn"),
		createEffectSubItem("Entei", "item_pkmn", (int)ItemIds.Entei, "items_pkmn"),
		createEffectSubItem("Moltres", "item_pkmn", (int)ItemIds.Moltres, "items_pkmn"),
		createEffectSubItem("Munchlax", "item_pkmn", (int)ItemIds.Munchlax, "items_pkmn"),
		createEffectSubItem("Deoxys", "item_pkmn", (int)ItemIds.Deoxys, "items_pkmn"),
		createEffectSubItem("Groudon", "item_pkmn", (int)ItemIds.Groudon, "items_pkmn"),
		createEffectSubItem("Gulpin", "item_pkmn", (int)ItemIds.Gulpin, "items_pkmn"),
		createEffectSubItem("Staryu", "item_pkmn", (int)ItemIds.Staryu, "items_pkmn"),
		createEffectSubItem("Staryu's Shot", "item_pkmn", (int)ItemIds.Staryus_Shot, "items_pkmn"),
		createEffectSubItem("Ho-oh", "item_pkmn", (int)ItemIds.Ho_oh, "items_pkmn"),
		createEffectSubItem("Ho-oh's Shot", "item_pkmn", (int)ItemIds.Ho_ohs_Shot, "items_pkmn"),
		//createEffectSubItem("Jirachi", "item_pkmn", (int)ItemIds.Jirachi, "items_pkmn"),
		createEffectSubItem("Snorlax", "item_pkmn", (int)ItemIds.Snorlax, "items_pkmn"),
		createEffectSubItem("Bellossom", "item_pkmn", (int)ItemIds.Bellossom, "items_pkmn"),
		createEffectSubItem("Kyogre", "item_pkmn", (int)ItemIds.Kyogre, "items_pkmn"),
		createEffectSubItem("Kyogre's Shot", "item_pkmn", (int)ItemIds.Kyogres_Shot, "items_pkmn"),
		createEffectSubItem("Latias and Latios", "item_pkmn", (int)ItemIds.Latias_and_Latios, "items_pkmn"),
		createEffectSubItem("Lugia", "item_pkmn", (int)ItemIds.Lugia, "items_pkmn"),
		createEffectSubItem("Lugia's Shot", "item_pkmn", (int)ItemIds.Lugias_Shot, "items_pkmn"),
		createEffectSubItem("Manaphy", "item_pkmn", (int)ItemIds.Manaphy, "items_pkmn"),
		createEffectSubItem("Weavile", "item_pkmn", (int)ItemIds.Weavile, "items_pkmn"),
		createEffectSubItem("Electrode", "item_pkmn", (int)ItemIds.Electrode, "items_pkmn"),
		createEffectSubItem("Metagross", "item_pkmn", (int)ItemIds.Metagross, "items_pkmn"),
		//createEffectSubItem("Mew", "item_pkmn", (int)ItemIds.Mew, "items_pkmn"),
		createEffectSubItem("Meowth", "item_pkmn", (int)ItemIds.Meowth, "items_pkmn"),
		createEffectSubItem("Meowth's Shot", "item_pkmn", (int)ItemIds.Meowths_Shot, "items_pkmn"),
		createEffectSubItem("Piplup", "item_pkmn", (int)ItemIds.Piplup, "items_pkmn"),
		createEffectSubItem("Togepi", "item_pkmn", (int)ItemIds.Togepi, "items_pkmn"),
		createEffectSubItem("Goldeen", "item_pkmn", (int)ItemIds.Goldeen, "items_pkmn"),
		createEffectSubItem("Gardevoir", "item_pkmn", (int)ItemIds.Gardevoir, "items_pkmn"),
		createEffectSubItem("Wobuffet", "item_pkmn", (int)ItemIds.Wobuffet, "items_pkmn"),
		createEffectSubItem("Suicune", "item_pkmn", (int)ItemIds.Suicune, "items_pkmn"),
		createEffectSubItem("Bonsly", "item_pkmn", (int)ItemIds.Bonsly, "items_pkmn"),
		createEffectSubItem("Random", "item_pkmn", (int)ItemIds.Random, "items_pkmn"),
		
		/// Assist Trophies
		createEffectSubItem("Andross", "item_assist", (int)ItemIds.Andross, "items_assist"),
		createEffectSubItem("Andross' Shot", "item_assist", (int)ItemIds.Andross_Shot, "items_assist"),
		createEffectSubItem("Barbara", "item_assist", (int)ItemIds.Barbara, "items_assist"),
		createEffectSubItem("Gray Fox", "item_assist", (int)ItemIds.Gray_Fox, "items_assist"),
		createEffectSubItem("Ray MKII Custom Robo", "item_assist", (int)ItemIds.Ray_MKII_Custom_Robo, "items_assist"),
		createEffectSubItem("Ray MKII Bomb", "item_assist", (int)ItemIds.Ray_MKII_Bomb, "items_assist"),
		createEffectSubItem("Ray MKII Gun Shot", "item_assist", (int)ItemIds.Ray_MKII_Gun_Shot, "items_assist"),
		createEffectSubItem("Samurai Goroh", "item_assist", (int)ItemIds.Samurai_Goroh, "items_assist"),
		createEffectSubItem("Devil", "item_assist", (int)ItemIds.Devil, "items_assist"),
		createEffectSubItem("Excitebike", "item_assist", (int)ItemIds.Excitebike, "items_assist"),
		createEffectSubItem("Jeff Andonuts", "item_assist", (int)ItemIds.Jeff_Andonuts, "items_assist"),
		createEffectSubItem("Jeff Pencil Bullet", "item_assist", (int)ItemIds.Jeff_Pencil_Bullet, "items_assist"),
		createEffectSubItem("Jeff Pencil Rocket", "item_assist", (int)ItemIds.Jeff_Pencil_Rocket, "items_assist"),
		createEffectSubItem("Lakitu", "item_assist", (int)ItemIds.Lakitu, "items_assist"),
		createEffectSubItem("Knuckle Joe", "item_assist", (int)ItemIds.Knuckle_Joe, "items_assist"),
		createEffectSubItem("Knuckle Joe_Shot", "item_assist", (int)ItemIds.Knuckle_Joe_Shot, "items_assist"),
		createEffectSubItem("Hammer Bro", "item_assist", (int)ItemIds.Hammer_Bro, "items_assist"),
		createEffectSubItem("Hammer Bro Hammer", "item_assist", (int)ItemIds.Hammer_Bro_Hammer, "items_assist"),
		createEffectSubItem("Helirin", "item_assist", (int)ItemIds.Helirin, "items_assist"),
		createEffectSubItem("Kat and Ana", "item_assist", (int)ItemIds.Kat_and_Ana, "items_assist"),
		createEffectSubItem("Ana", "item_assist", (int)ItemIds.Ana, "items_assist"),
		createEffectSubItem("Jill and Drill Dozer", "item_assist", (int)ItemIds.Jill_and_Drill_Dozer, "items_assist"),
		createEffectSubItem("Lyn", "item_assist", (int)ItemIds.Lyn, "items_assist"),
		createEffectSubItem("Little Mac", "item_assist", (int)ItemIds.Little_Mac, "items_assist"),
		createEffectSubItem("Metroid", "item_assist", (int)ItemIds.Metroid, "items_assist"),
		createEffectSubItem("Nintendog", "item_assist", (int)ItemIds.Nintendog, "items_assist"),
		createEffectSubItem("Nintendog Full", "item_assist", (int)ItemIds.NintendogFull, "items_assist"),
		createEffectSubItem("Mr. Resetti", "item_assist", (int)ItemIds.Mr_Resetti, "items_assist"),
		createEffectSubItem("Isaac", "item_assist", (int)ItemIds.Isaac, "items_assist"),
		createEffectSubItem("Isaac Shot", "item_assist", (int)ItemIds.Isaac_Shot, "items_assist"),
		createEffectSubItem("Saki Amamiya", "item_assist", (int)ItemIds.Saki_Amamiya, "items_assist"),
		createEffectSubItem("Saki Shot 1", "item_assist", (int)ItemIds.Saki_Shot_1, "items_assist"),
		createEffectSubItem("Saki Shot 2", "item_assist", (int)ItemIds.Saki_Shot_2, "items_assist"),
		createEffectSubItem("Shadow the Hedgehog", "item_assist", (int)ItemIds.Shadow_the_Hedgehog, "items_assist"),
		createEffectSubItem("Infantry", "item_assist", (int)ItemIds.Infantry, "items_assist"),
		createEffectSubItem("Infantry Shot", "item_assist", (int)ItemIds.Infantry_Shot, "items_assist"),
		createEffectSubItem("Stafy", "item_assist", (int)ItemIds.Stafy, "items_assist"),
		createEffectSubItem("Tank Infantry", "item_assist", (int)ItemIds.Tank_Infantry, "items_assist"),
		createEffectSubItem("Tank Shot", "item_assist", (int)ItemIds.Tank_Shot, "items_assist"),
		createEffectSubItem("Tingle", "item_assist", (int)ItemIds.Tingle, "items_assist"),
		createEffectSubItem("Togezo", "item_assist", (int)ItemIds.togezo, "items_assist"),
		createEffectSubItem("Waluigi", "item_assist", (int)ItemIds.Waluigi, "items_assist"),
		createEffectSubItem("Dr. Wright", "item_assist", (int)ItemIds.Dr_Wright, "items_assist"),
		createEffectSubItem("Wright Buildings", "item_assist", (int)ItemIds.Wright_Buildings, "items_assist"),
		createEffectSubItem("Random", "item_assist", (int)ItemIds.Random, "items_assist")
		
    };

    //Slider ranges need to be defined
    public override List<ItemType> ItemTypes => new List<ItemType>(new[]
    {
        new ItemType("Percent", "%percent", ItemType.Subtype.Slider, "{\"min\":1,\"max\":999}"),
        new ItemType("#Stocks", "#stocks", ItemType.Subtype.Slider, "{\"min\":1,\"max\":99}"),
		new ItemType("#Items", "#items", ItemType.Subtype.Slider, "{\"min\":1,\"max\":5}"),
		new ItemType("#Pkmn", "#pkmn", ItemType.Subtype.Slider, "{\"min\":1,\"max\":2}"),
		new ItemType("#Assist", "#assist", ItemType.Subtype.Slider, "{\"min\":1,\"max\":2}"),
		new ItemType("Flower Rate", "flower_rate", ItemType.Subtype.Slider, "{\"min\":1,\"max\":1000}"),
		new ItemType("Flower Size", "flower_size", ItemType.Subtype.Slider, "{\"min\":1,\"max\":10}"),
		new ItemType("Slow Duration", "slow_duration", ItemType.Subtype.Slider, "{\"min\":1,\"max\":720}"),

		new ItemType("Players", "players", ItemType.Subtype.ItemList),
		new ItemType("Regular Items", "items_reg", ItemType.Subtype.ItemList),
		//new ItemType("Special Items", "items_sp", ItemType.Subtype.ItemList),
		new ItemType("Pokemon", "items_pkmn", ItemType.Subtype.ItemList),
		new ItemType("Assist", "items_assist", ItemType.Subtype.ItemList),
		new ItemType("Set", "set", ItemType.Subtype.ItemList),
		new ItemType("Mushroom", "mushroom", ItemType.Subtype.ItemList)
    });
}
