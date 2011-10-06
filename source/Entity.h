#ifndef Entity_h
#define Entity_h

#include <G3D/G3DAll.h>

class Entity : public GEntity {
private:

    /** Prevent instantiation */
    Entity() {}

protected:

    Entity
    (const std::string& name,
     AnyTableReader&    propertyTable, 
     const ModelTable&  modelTable);

public:

    typedef ReferenceCountedPointer<Entity> Ref;

    static Entity::Ref create
    (const std::string& name,
     AnyTableReader&    propertyTable,
     const ModelTable&  modelTable);

    void onSimulation(GameTime absoluteTime, GameTime deltaTime);
};

#endif
