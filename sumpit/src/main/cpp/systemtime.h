#ifndef SYSTEMTIME_H
#define SYSTEMTIME_H


class SystemTime
{
  public:
    /** Default constructor */
    SystemTime();
    /** Default destructor */
    virtual ~SystemTime();
    virtual unsigned long nowMillis();

  protected:

  private:
};

#endif // SYSTEMTIME_H
