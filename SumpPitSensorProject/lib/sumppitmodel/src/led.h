#ifndef LED_H
#define LED_H

/** Just a led */
class Led {
  public:
    /** Default constructor */
    Led();
    /** Default destructor */
    virtual ~Led();

    virtual void setState(bool on);
  protected:

  private:
};

#endif // LED_H
