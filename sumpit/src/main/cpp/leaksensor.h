#ifndef LEAKSENSOR_H
#define LEAKSENSOR_H


class LeakSensor
{
  public:
    /** Default constructor */
    LeakSensor();
    /** Default destructor */
    virtual ~LeakSensor();
    /** gives true if there is a leak. */
    virtual bool isLeaking();

  protected:

  private:
};

#endif // LEAKSENSOR_H
