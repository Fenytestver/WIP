#ifndef RPMSENSOR_H
#define RPMSENSOR_H


class RpmSensor
{
  public:
    /** Default constructor */
    RpmSensor();
    /** Default destructor */
    virtual ~RpmSensor();
    virtual void setup();
    virtual int getRpm();
  protected:

  private:
};

#endif // RPMSENSOR_H
