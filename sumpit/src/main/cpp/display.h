#ifndef DISPLAY_H
#define DISPLAY_H


class Display
{
  public:
    /** Default constructor */
    Display();
    /** Default destructor */
    virtual ~Display();

    virtual void displayMessage(char* message);
    virtual void clear();
  protected:

  private:
};

#endif // DISPLAY_H
