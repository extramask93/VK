#include "MouseAdapter.h"
#include "maps.h"


MouseAdapter::MouseAdapter():mouseReport{}
{
	mouseReport.id = 2;
}


MouseAdapter::~MouseAdapter()
{
}

size_t MouseAdapter::mousePush(std::string l)
{
	unsigned int key;
	if (l == "LPM")
		key = LPM;
	else if (l == "PPM")
		key = PPM;
	else
		return 1;
	mouseReport.buttons |= key;
	mouseMove("0,0,0");
	return 0;
}

void MouseAdapter::mouseReleaseAll(std::string l)
{
	mouseReport.buttons = 0;
	mouseMove("0,0,0");
}

size_t MouseAdapter::mouseRelease(std::string l)
{
	unsigned int key;
	if (l == "LPM")
		key = LPM;
	else if (l == "PPM")
		key = PPM;
	else
		return 1;
	mouseReport.buttons &= ~key;
	mouseMove("0,0,0");
	return key;
}

void MouseAdapter::mouseMove(std::string l)
{
	try {
		std::istringstream str{ l };
		int8_t X, Y, angle;
		std::vector<std::string> tokens;
		boost::split(tokens, l, boost::is_any_of(","), boost::token_compress_on);
		mouseReport.X = boost::numeric_cast<int8_t>(boost::lexical_cast<int>(tokens[0]));
		mouseReport.Y = boost::numeric_cast<int8_t>(boost::lexical_cast<int>(tokens[1]));
		mouseReport.Wheel = boost::numeric_cast<int8_t>(boost::lexical_cast<int>(tokens[2]));
	}
	catch (std::exception const &ex)
	{
		throw ParseException{ "Error: Wrong parameters" };
	}
	auto tp = std::make_shared<MouseReport>(mouseReport);
	bque.push(tp);
}

void MouseAdapter::mouseClick(std::string params)
{
	mousePush(params);
	mouseRelease(params);
}
