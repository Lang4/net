#include "Connection.h"
#include "MyClient.h"
#include "Info.h"
#include "MyTick.h"
#include "start.h"
void LogicConnection::beInit()
{
	printf("��������\n");
	this->registerLogics(&theLogics);

	theTick.addTick(this);

}
bool LogicConnection::beFinal()
{
	theTick.removeTick(this);
	return false;
}