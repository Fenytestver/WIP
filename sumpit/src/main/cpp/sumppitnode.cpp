#include "sumppitnode.h"

SumpPitNode::SumpPitNode(Siren* _siren,
    Display* _display,
    SumpPitSensor* _sensor,
    SumpPitInputs* _inputs)
{
  //ctor
  siren = _siren;
  display = _display;
  sensor = _sensor;
  inputs = _inputs;

  mode = SPN_INITIALIZING;
}

SumpPitNode::~SumpPitNode()
{
  //dtor
}
int SumpPitNode::getMode()
{

}

void SumpPitNode::arm()
{

}

void SumpPitNode::disarm()
{

}

void SumpPitNode::maintenance()
{

}

void SumpPitNode::alarm()
{

}
