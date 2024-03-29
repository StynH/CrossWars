#include "FlagManager.h"

std::map<std::string, GameFlag*> FlagManager::GameFlags = std::map<std::string, GameFlag*>();

void FlagManager::LoadFlags()
{
	std::vector<std::string> FlagList = {
		"0xF1",
		"0xF2",
		"0xF3",
		"0xF4",
		"0xF5",
		"0xF6",
		"0xF7",
		"0xF8",
		"0xF9",
		"0xF10",
		"0xF11",
		"0xF12",
		"0xF13",
		"0xF14",
		"0xF15",
		"0xF16",
		"0xF17",
		"0xF18",
		"0xF19",
		"0xF20",
		"0xF21",
		"0xF22",
		"0xF23",
		"0xF24",
		"0xF25",
		"0xF26",
		"0xF27",
		"0xF28",
		"0xF29",
		"0xF30",
		"0xF31",
		"0xF32",
		"0xF33",
		"0xF34",
		"0xF35",
		"0xF36",
		"0xF37",
		"0xF38",
		"0xF39",
		"0xF40",
		"0xF41",
		"0xF42",
		"0xF43",
		"0xF44",
		"0xF45",
		"0xF46",
		"0xF47",
		"0xF48",
		"0xF49",
		"0xF50",
		"0xF51",
		"0xF52",
		"0xF53",
		"0xF54",
		"0xF55",
		"0xF56",
		"0xF57",
		"0xF58",
		"0xF59",
		"0xF60",
		"0xF61",
		"0xF62",
		"0xF63",
		"0xF64",
		"0xF65",
		"0xF66",
		"0xF67",
		"0xF68",
		"0xF69",
		"0xF70",
		"0xF71",
		"0xF72",
		"0xF73",
		"0xF74",
		"0xF75",
		"0xF76",
		"0xF77",
		"0xF78",
		"0xF79",
		"0xF80",
		"0xF81",
		"0xF82",
		"0xF83",
		"0xF84",
		"0xF85",
		"0xF86",
		"0xF87",
		"0xF88",
		"0xF89",
		"0xF90",
		"0xF91",
		"0xF92",
		"0xF93",
		"0xF94",
		"0xF95",
		"0xF96",
		"0xF97",
		"0xF98",
		"0xF99",
		"0xF100",
		"0xF101",
		"0xF102",
		"0xF103",
		"0xF104",
		"0xF105",
		"0xF106",
		"0xF107",
		"0xF108",
		"0xF109",
		"0xF110",
		"0xF111",
		"0xF112",
		"0xF113",
		"0xF114",
		"0xF115",
		"0xF116",
		"0xF117",
		"0xF118",
		"0xF119",
		"0xF120",
		"0xF121",
		"0xF122",
		"0xF123",
		"0xF124",
		"0xF125",
		"0xF126",
		"0xF127",
		"0xF128",
		"0xF129",
		"0xF130",
		"0xF131",
		"0xF132",
		"0xF133",
		"0xF134",
		"0xF135",
		"0xF136",
		"0xF137",
		"0xF138",
		"0xF139",
		"0xF140",
		"0xF141",
		"0xF142",
		"0xF143",
		"0xF144",
		"0xF145",
		"0xF146",
		"0xF147",
		"0xF148",
		"0xF149",
		"0xF150",
		"0xF151",
		"0xF152",
		"0xF153",
		"0xF154",
		"0xF155",
		"0xF156",
		"0xF157",
		"0xF158",
		"0xF159",
		"0xF160",
		"0xF161",
		"0xF162",
		"0xF163",
		"0xF164",
		"0xF165",
		"0xF166",
		"0xF167",
		"0xF168",
		"0xF169",
		"0xF170",
		"0xF171",
		"0xF172",
		"0xF173",
		"0xF174",
		"0xF175",
		"0xF176",
		"0xF177",
		"0xF178",
		"0xF179",
		"0xF180",
		"0xF181",
		"0xF182",
		"0xF183",
		"0xF184",
		"0xF185",
		"0xF186",
		"0xF187",
		"0xF188",
		"0xF189",
		"0xF190",
		"0xF191",
		"0xF192",
		"0xF193",
		"0xF194",
		"0xF195",
		"0xF196",
		"0xF197",
		"0xF198",
		"0xF199",
		"0xF200",
		"0xF201",
		"0xF202",
		"0xF203",
		"0xF204",
		"0xF205",
		"0xF206",
		"0xF207",
		"0xF208",
		"0xF209",
		"0xF210",
		"0xF211",
		"0xF212",
		"0xF213",
		"0xF214",
		"0xF215",
		"0xF216",
		"0xF217",
		"0xF218",
		"0xF219",
		"0xF220",
		"0xF221",
		"0xF222",
		"0xF223",
		"0xF224",
		"0xF225",
		"0xF226",
		"0xF227",
		"0xF228",
		"0xF229",
		"0xF230",
		"0xF231",
		"0xF232",
		"0xF233",
		"0xF234",
		"0xF235",
		"0xF236",
		"0xF237",
		"0xF238",
		"0xF239",
		"0xF240",
		"0xF241",
		"0xF242",
		"0xF243",
		"0xF244",
		"0xF245",
		"0xF246",
		"0xF247",
		"0xF248",
		"0xF249",
		"0xF250",
		"0xF251",
		"0xF252",
		"0xF253",
		"0xF254",
		"0xF255"
	};

	for (std::string Adress : FlagList)
	{
		GameFlags.insert(std::pair<std::string, GameFlag*>(Adress, new GameFlag(Adress)));
	}
}

bool FlagManager::FlagSet(std::string _Adress)
{
	return GameFlags.at(_Adress)->IsFlagSet();
}

void FlagManager::SetFlag(std::string _Adress)
{
	GameFlags.at(_Adress)->Set();
}

void FlagManager::UnsetFlag(std::string _Adress)
{
	GameFlags.at(_Adress)->Unset();
}

std::map<std::string, GameFlag*> FlagManager::GetFlags()
{
	return GameFlags;
}
