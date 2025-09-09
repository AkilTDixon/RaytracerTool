#include "jsonClass.h"

GeometryJ::GeometryJ()
{
	comment = "";
	FloatMemberMap =
	{ {"P1", &GeometryJ::getP1},
	  {"P2", &GeometryJ::getP2},
	  {"P3", &GeometryJ::getP3},
	  {"P4", &GeometryJ::getP4},
	  {"centre", &GeometryJ::getCentre},
	  {"ac", &GeometryJ::getac},
	  {"dc", &GeometryJ::getdc},
	  {"sc", &GeometryJ::getsc},
	};
};


LightJ::LightJ()
{
	comment = "";
	FloatMemberMap =
	{ {"P1", &LightJ::getP1},
	  {"P2", &LightJ::getP2},
	  {"P3", &LightJ::getP3},
	  {"P4", &LightJ::getP4},

	  {"centre", &LightJ::getCentre},

	  {"id", &LightJ::getid},
	  {"is", &LightJ::getis}
	};
};


OutputJ::OutputJ()
{
	globalillum = false;
	maxbounces = 0;
	probterminate = 1;
	speedup = 1;
	antialiasing = false;
	twosiderender = false;
	raysperpixel = Eigen::Vector2d(1,1);

	FloatMemberMap =
	{   {"lookat",&OutputJ::getlookat},
		{"up",&OutputJ::getup},
		{"ai",&OutputJ::getai},
		{"bkc",&OutputJ::getbkc},
		{"centre",&OutputJ::getcentre}
	};
};

void JsonParse::cleanThis()
{

}

JsonParse::JsonParse()
{

}
JsonParse::~JsonParse()
{

}
