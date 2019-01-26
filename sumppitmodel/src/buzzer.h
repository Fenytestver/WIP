#ifndef BUZZER_H
#define BUZZER_H
#include "siren.h"
/**
 * Buzzer is same as a Siren,
 * with an extra option for a short beep.
 */
class Buzzer : public Siren
{
  public:
    /** Default constructor */
    Buzzer();
    /** Default destructor */
    virtual ~Buzzer();
    /** beeps once. */
    virtual void beep();
  protected:

  private:
};

#endif // BUZZER_H
