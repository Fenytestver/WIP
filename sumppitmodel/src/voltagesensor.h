#ifndef VOLTAGESENSOR_H
#define VOLTAGESENSOR_H


class VoltageSensor
{
  public:
    /** Default constructor */
    VoltageSensor();
    /** Default destructor */
    virtual ~VoltageSensor();
    /** returns the measured voltage */
    virtual float getVoltage();
    /** Call at setup phase. */
    virtual void setup();
  protected:

  private:
};

#endif // VOLTAGESENSOR_H
