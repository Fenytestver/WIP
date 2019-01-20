#ifndef SIREN_H
#define SIREN_H

/** The big loud siren */
class Siren
{
  public:
    /** Default constructor */
    Siren();
    /** Default destructor */
    virtual ~Siren();
    virtual void setup();
    /** Turn on the alarm. */
    virtual void on();
    /** Turn off the alarm. */
    virtual void off();
    virtual void update();
  protected:

  private:
};

#endif // SIREN_H
