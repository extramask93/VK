#include "Printer.h"



Printer::Printer(BlockingQueue<std::shared_ptr<IMessage>> &bq): bque{bq}
{
}


Printer::~Printer()
{
}
void Printer::verbosePrinter(const Report &rep) const
{
	char buffer[56];
	auto tp = std::make_shared<Report>(rep);
	bque.push(tp);
	sprintf_s(buffer, 56, "id: %i modifiers: %02x [%02x,%02x,%02x,%02x,%02x,%02x]\n", rep.id, rep.modifiers, rep.keys[0], rep.keys[1], rep.keys[2], rep.keys[3], rep.keys[4], rep.keys[5]);
	PrintThread{} << buffer;
}

void Printer::printer(const Report &rep) const
{
	auto tp = std::make_shared<Report>(rep);
	bque.push(tp);
}
void Printer::verboseMousePrinter(const MouseReport &rep) const
{
	auto tp = std::make_shared<MouseReport>(rep);
	bque.push(tp);
	PrintThread{} << "id: " << std::to_string(rep.id) << " buttons: " << std::to_string(rep.buttons) << " X: " << std::to_string(rep.X) << " Y: " << std::to_string(rep.Y) << " Wheel: " << std::to_string(rep.Wheel) << '\n';
}
void Printer::mousePrinter(const MouseReport &rep) const
{
	auto tp = std::make_shared<MouseReport>(rep);
	bque.push(tp);
}
