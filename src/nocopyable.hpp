#ifndef __SIMPLE_NOCOPYABLE__
#define __SIMPLE_NOCOPYABLE__

namespace simple
{

class nocopyable
{
protected:
    nocopyable(){}
    ~nocopyable(){}
private:
    nocopyable(const nocopyable&);
    const nocopyable& operator=(const nocopyable&);
};

}
#endif
