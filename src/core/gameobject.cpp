#include "gameobject.h"
#include "world.h"
#include "region.h"

GameObject::GameObject( int id)
{
	if (id !=0)
	{
		m_id = id;
	}
	else
	{
		if (World::getWorld() !=0)
		{
			m_id = World::getWorld()->getValidId();
		}
		else
			m_id =0;
	}
	
	m_state = STATE_ACTIVE;
	m_event_mask=0;
	m_region_id = -1;
}


void GameObject::setState(State s, bool throwevent)
{
	m_state = s;
	
	// einige Zustaende werden nicht ueber das Netzwerk angeglichen
	if (throwevent)
	{
		addToNetEventMask(NetEvent::DATA_STATE);
	}
}

void  GameObject::setType(Type type)
{
	m_type = type;
}

void GameObject::setSubtype(Subtype subtype)
{
	m_subtype = subtype;
}

void GameObject::setLayer(short layer)
{
	m_layer = layer;
}

Region* GameObject::getRegion()
{
	if (World::getWorld() == 0)
		return 0;

	return World::getWorld()->getRegion(m_region_id);
}

void GameObject::setSpeed(Vector speed)
{
	m_speed =speed;
	m_event_mask |= NetEvent::DATA_MOVE_INFO;
}

void GameObject::setAngle(float angle)
{
	m_shape.m_angle = angle;
}

void  GameObject::setPosition(Vector pos)
{
	m_shape.m_center = pos;
}

void GameObject::toString(CharConv* cv)
{
	cv->toBuffer(m_type);
	cv->toBuffer(m_subtype);
	cv->toBuffer(m_id);
	cv->toBuffer(m_shape.m_center.m_x);
	cv->toBuffer(m_shape.m_center.m_y);
	cv->toBuffer((char) m_shape.m_type);
	if (m_shape.m_type==Shape::RECT)
	{
		cv->toBuffer(m_shape.m_extent.m_x);
		cv->toBuffer(m_shape.m_extent.m_y);
	}
	else
	{
		cv->toBuffer(m_shape.m_radius);
		cv->toBuffer(m_shape.m_radius);
	}
	cv->toBuffer((char) m_layer);
	cv->toBuffer(m_shape.m_angle);

	cv->toBuffer((char) m_state);
}

void GameObject::fromString(CharConv* cv)
{
	char ctmp;

	// Typ, Subtyp und ID werden extern gelesen
	cv->fromBuffer<float>(m_shape.m_center.m_x) ;
	cv->fromBuffer<float>(m_shape.m_center.m_y);
	cv->fromBuffer<char>(ctmp);
	m_shape.m_type = (Shape::ShapeType) ctmp;
	if (m_shape.m_type==Shape::RECT)
	{
		cv->fromBuffer<float>(m_shape.m_extent.m_x);
		cv->fromBuffer<float>(m_shape.m_extent.m_y);
	}
	else
	{
		cv->fromBuffer<float>(m_shape.m_radius);
		cv->fromBuffer<float>(m_shape.m_radius);
	}
	cv->fromBuffer<char>(ctmp);
	m_layer  = (Layer) ctmp;
	cv->fromBuffer<float>(m_shape.m_angle);

	cv->fromBuffer<char>(ctmp);
	m_state = (State) ctmp;
}

string GameObject::getName()
{
	return ObjectFactory::getObjectName(m_subtype);
}


string GameObject::getNameId()
{
	std::ostringstream out_stream;

	out_stream.str("");
	out_stream << m_subtype << ":" << getId();
	return out_stream.str();
}

