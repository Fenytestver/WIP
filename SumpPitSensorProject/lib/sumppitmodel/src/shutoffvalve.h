#ifndef SHUTOFFVALVE_H
#define SHUTOFFVALVE_H


class ShutoffValve
{
  public:
    ShutoffValve();
    virtual ~ShutoffValve();
    virtual void activate();
    virtual void deactivate();
    virtual bool isActive();
  protected:

  private:
};

#endif // SHUTOFFVALVE_H
