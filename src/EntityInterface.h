#ifndef __ENTITYINTERFACE_H__
#define __ENTITYINTERFACE_H__

class IEntity
{
    public:
        virtual void Draw(void) {}
        virtual void Fall(void) {}
        virtual void Move(void) {}
        virtual int Die(void) = 0;
};

#endif