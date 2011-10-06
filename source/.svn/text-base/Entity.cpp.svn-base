#include "Entity.h"

Entity::Entity
(const std::string& name,
 AnyTableReader&    propertyTable,
 const ModelTable&  modelTable) : 
    GEntity(name, propertyTable, modelTable) {
}


Entity::Ref Entity::create
(const std::string& name,
 AnyTableReader&    propertyTable,
 const ModelTable&  modelTable) {

    return new Entity(name, propertyTable, modelTable);
}

void Entity::onSimulation(GameTime absoluteTime, GameTime deltaTime){
    (void)deltaTime;
    PhysicsFrame prevFrame(m_frame);
    CoordinateFrame c = CFrame(prevFrame);
    m_frame = m_frameSpline.evaluate(float(absoluteTime));
	
    switch (m_modelType) {
    case ARTICULATED_MODEL:
	m_artPoseSpline.get(float(absoluteTime), m_artPose);
	break;
	
    case MD2_MODEL:
	{
	    int q = int(absoluteTime);
	    MD2Model::Pose::Action a;
	    if(m_name == "mod"){

		if(q % 8 == 0)
		    a.flip = true;
	    }

	    else if(m_name == "mod1"){
		if(q % 5 == 0)
		    a.jump = true;
	    }
	    m_md2Pose.onSimulation(deltaTime, a);

	    break;
	}
	
    case MD3_MODEL:
	m_md3Model->simulatePose(m_md3Pose, deltaTime);
	break;
    }
}	
