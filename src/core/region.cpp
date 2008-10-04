#include "region.h"
#include "world.h"


Region::Region(short dimx, short dimy, short id, World* world, bool server)
{
	DEBUG5("creating region");
	m_data_grid = new Matrix2d<Gridunit>(dimx,dimy);
	m_dimx = dimx;
	m_dimy = dimy;

	m_tiles = new Matrix2d<char>(dimx*2,dimy*2);
	m_tiles->clear();

	// Binärbaum fuer WorldObjects anlegen
	m_objects = new map<int,WorldObject*>;
	m_static_objects = new map<int,WorldObject*>;

	m_players = new map<int,WorldObject*>;

	// Baum fuer Projektile anlegen
	m_projectiles = new map<int,Projectile*>;

	// Liste der Gegenstaende
	m_drop_items = new map<int,DropItem*>;
	m_drop_item_locations = new map<int,DropItem*>;

	m_id = id;

	m_events = new list<Event>;

	m_server = server;

	m_world = world;
}

Region::~Region()
{
	map<int,WorldObject*>::iterator i;
	for (i=m_objects->begin(); i!=m_objects->end();i++)
	{
		delete i->second;
	}

	for (i=m_static_objects->begin(); i!=m_static_objects->end();i++)
	{
		delete i->second;
	}

	map<int,Projectile*>::iterator j;
	for (j =  m_projectiles->begin(); j != m_projectiles->end(); j++)
	{
		delete (j->second);
	}


	map<int,DropItem*>::iterator k;
			for (k =  m_drop_items->begin(); k != m_drop_items->end(); k++)
	{
		delete k->second;
	}

	delete m_objects;
	delete m_static_objects;
	delete m_players;
	delete m_projectiles;

	delete m_data_grid;
	delete m_tiles;
	delete m_drop_items;
	delete m_drop_item_locations;

	delete m_events;

}

WorldObject* Region::getObject ( int id)
{
	map<int,WorldObject*>::iterator iter;

	// Objekt im Binärbaum suchen
	iter = m_objects->find(id);

	// Testen ob ein Objekt gefunden wurde
	if (iter == m_objects->end())
	{
		// keins gefunden, NULL ausgeben
		return 0;
	}
	else
	{
		// Zeiger auf Objekt ausgeben
		return iter->second;
	}
}

Projectile* Region::getProjectile(int id)
{
	map<int,Projectile*>::iterator iter;

	// Objekt im Binärbaum suchen
	iter = m_projectiles->find(id);

	// Testen ob ein Objekt gefunden wurde
	if (iter == m_projectiles->end())
	{
		// keins gefunden, NULL ausgeben
		return 0;
	}
	else
	{
		// Zeiger auf Objekt ausgeben
		return iter->second;
	}
}

bool Region::getFreePlace(Shape* shape, short layer, float& x, float&y)
{
	// Menge der bereits getesteten Felder
	set<int> tfields;

	// Schlange mit zu testenden Feldern
	queue<int> fields;

	float c = 1.1;
	// Position in 0.5 x 0.5 Feldern
	int sx = (int) (x/c);
	int sy = (int) (y/c);
	int i;
	fields.push(sx*10000+sy);

	// Kopie der Form
	Shape s;
	memcpy(&s,shape,sizeof(Shape));

	list<WorldObject*> res;
	res.clear();

	// eine Stelle suchen an der das Objekt passt
	while (!fields.empty())
	{
		// erstes Feld aus der Queue nehmen
		i = fields.front();
		sx = i /10000;
		sy = i %10000;
		fields.pop();

		DEBUG5("testing field %f %f",sx*c,sy*c);

		// Testen ob dort keine festen Objekte sind
		s.m_coordinate_x = sx*c;
		s.m_coordinate_y = sy*c;

		res.clear();
		getObjectsInShape(&s,&res,layer,WorldObject::FIXED,0,true);

		if (!res.empty())
		{
			// Objekt im Weg
			res.clear();
			continue;
		}

		DEBUG5("no fixed obstacle");
		// Testen, ob dort nicht gerade eine Kreatur steht
		getObjectsInShape(&s,&res,layer,WorldObject::CREATURE,0,true);
		if (res.empty())
		{
			DEBUG5("field is free");
			// Stelle ist frei
			x = sx*c ;
			y = sy*c ;


			return true;

		}
		else
		{
			WorldObject* obs = *(res.begin());
			DEBUG5("obstacle is %s at %f %f",obs->getNameId().c_str(), obs->getGeometry()->m_shape.m_coordinate_x,obs->getGeometry()->m_shape.m_coordinate_y);
			// Stelle ist besetzt
			tfields.insert(i);

			// Nachbarfelder in die Liste der zu untersuchenden Felder einfuegen
			if (sx>0)
			{
				i = (sx-1)*10000+sy;
				if (tfields.find(i)==tfields.end())
				{
					fields.push(i);
				}
			}

			if (sy>0)
			{
				i = (sx)*10000+sy-1;
				if (tfields.find(i)==tfields.end())
				{
					fields.push(i);
				}
			}

			if (sx<m_dimx*4-1)
			{
				i = (sx+1)*10000+sy;
				if (tfields.find(i)==tfields.end())
				{
					fields.push(i);
				}
			}

			if (sy<m_dimy*4-1)
			{
				i = (sx)*10000+sy+1;
				if (tfields.find(i)==tfields.end())
				{
					fields.push(i);
				}
			}

		}

	}

	// keine Stelle gefunden um das Item zu droppen
	return false;

}

bool  Region::addObjectsInShapeFromGridunit(Shape* shape, Gridunit* gu, list<WorldObject*>* result, short layer, short group,WorldObject* omit, bool empty_test )
{
	WorldObject* wo=0;
	WorldObject** arr=0;
	WorldObject::Geometry* wob=0;
	Shape* s=0;
	arr = gu->getObjects((WorldObject::Group) group);

	int n = gu->getObjectsNr((WorldObject::Group) group);

	DEBUG5("%i objects in layer %i, group %i",n,layer,group);
	for (int k=0;k< n;k++)
	{

		wo = arr[k];
		wob = wo->getGeometry();
		if (shape->m_type == Shape::CIRCLE)
		{
			DEBUG5("testing object %s at %f %f",wo->getNameId().c_str(),wob->m_shape.m_coordinate_x,wob->m_shape.m_coordinate_y);
		}
		s = &(wo->getGeometry()->m_shape);

		if (wob->m_layer & layer)
		{
			if (((shape ==0) || World::intersect(shape,s )))
			{
				if (wo != omit)
				{
					result->push_back(wo);

					if (empty_test)
						return true;
				}
			}
		}
	}
	return true;
}

bool Region::addObjectsOnLineFromGridunit(float xstart, float ystart, float xend,float yend, Gridunit* gu, list<WorldObject*>* result, short layer, short group ,WorldObject* omit, bool empty_test )
{
	WorldObject* wo=0;
	WorldObject** arr=0;
	WorldObject::Geometry* wob=0;
	Shape* s=0;
	arr = gu->getObjects((WorldObject::Group) group);

	int n = gu->getObjectsNr((WorldObject::Group) group);

	DEBUG5("%i objects in layer %i, group %i",n,layer,group);
	float  dir[2];
	dir[0] = xstart-xend;
	dir[1] = ystart-yend;

	for (int k=0;k< n;k++)
	{

		wo = arr[k];
		DEBUG5("testing %s %p",wo->getNameId().c_str(),wo->getNameId().c_str())
		DEBUG5("testing %p",wo);
		wob = wo->getGeometry();
		s = &(wo->getGeometry()->m_shape);
		if (wob->m_layer & layer)
		{
			if (World::lineIntersect(xstart, ystart,  xend,yend ,dir, s))
			{
				if (wo != omit)
				{
					result->push_back(wo);

					if (empty_test)
						return true;
				}
			}
		}
	}
	return true;
}


bool Region::getObjectsInShape( Shape* shape,  list<WorldObject*>* result,short layer, short group, WorldObject* omit, bool empty_test)
{

	DEBUG5("shape %f %f %f",shape->m_coordinate_x,shape->m_coordinate_y,shape->m_radius);

	 // Wenn der Resultat Zeiger 0 ist: Fehler ausgeben
	if (result == 0)
	{
		ERRORMSG("no result list given");
		return false;
	}

	if (shape == 0)
	{
		DEBUG("no shape given");
		return false;
	}


	DEBUG5("layer %i group %i",layer,group);


	if (group == WorldObject::PLAYER)
	{
		// Wenn nur nach Spielern gesucht ist, dann nur die Liste der Spieler durchsuchen
		/*list<Player*>::iterator i;
		// TODO: Player umbenennen, einbauen
		for (i=m_players->begin();i!=m_player->end();++i)
		{
		wo= (WorldObject*) *i;
		s = &(wo->getGeometry()->m_shape);
		if (World::intersect(shape,s ))
		{
		result->push_back(wo);
	}
	}
		*/
	}
	else
	{
		int i,j;
		Gridunit* gu=0;
		// durchmustere alle 4*4 Felder die von dem Suchrechteck plus 4 Felder bedeckt werden
		float xe,ye;
		if (shape->m_type == Shape::CIRCLE)
		{
			xe = shape->m_radius;
			ye = shape->m_radius;
		}
		else
		{
			xe = shape->m_extent_x;
			ye = shape->m_extent_y;
		}

		int xmin = (int) floor(0.25*(shape->m_coordinate_x-xe-2));
		int ymin = (int) floor(0.25*(shape->m_coordinate_y-ye-2));
		int xmax = (int) floor(0.25*(shape->m_coordinate_x+xe+2));
		int ymax = (int) floor(0.25*(shape->m_coordinate_y+ye+2));

		DEBUG5("searching square (%i %i) (%i %i)",xmin,ymin,xmax,ymax);
		// Pruefen ob die Suchanfrage nicht aus der Region herauslaeuft
		bool ret = false;
		int is = max (xmin,0);
		int ie = min(xmax,m_dimx-1);
		int js = max(ymin,0);
		int je = min(ymax,m_dimy-1);

		// Alle 4x4 Felder durchmustern
		for (i = is;i<=ie;i++)
		{
			for (j=js;j<=je;j++)
			{

				DEBUG5("searching in Grid Tile %i %i",i,j);
				// Durchmustern der Listen im 4x4-Feld
				gu = (m_data_grid->ind(i,j));

				// Totenebene
				if (group & WorldObject::DEAD)
				{
					DEBUG5("searching dead layer");

					ret =  addObjectsInShapeFromGridunit(shape, gu, result, layer,WorldObject::DEAD, omit, empty_test);
					if (!result->empty() && empty_test)
						return true;

				}

				// feste Objekte
				if (group & WorldObject::FIXED )
				{
					DEBUG5("searching fixed layer");

					ret =  addObjectsInShapeFromGridunit(shape, gu, result, layer,WorldObject::FIXED, omit, empty_test);
					if (!result->empty() && empty_test)
						return true;

				}

				// lebende Objekte
				if (group & WorldObject::CREATURE)
				{

					ret =  addObjectsInShapeFromGridunit(shape, gu, result, layer,group & WorldObject::CREATURE, omit, empty_test);
					if (!result->empty() && empty_test)
						return true;

				}

			}

		}
	}
	return true;
}


WorldObject* Region::getObjectAt(float x_coordinate, float y_coordinate, short layer, short group)
{
	Shape s;
	s.m_type = Shape::CIRCLE;
	s.m_radius =0;
	s.m_coordinate_x=x_coordinate;
	s.m_coordinate_y=y_coordinate;

	list<WorldObject*> l;
	l.clear();
	getObjectsInShape ( &s, &l,layer,group );
	if (l.empty())
	{
		return 0;
	}
	else
	{
		return *(l.begin());
	}
}



void Region::getObjectsOnLine( float xstart, float ystart, float xend, float yend,  list<WorldObject*>* result,short layer, short group , WorldObject* omit)
{
	Gridunit* gu=0;

	int xmin = (short) floor(0.25*(min(xstart,xend)-2));
	int ymin = (short) floor(0.25*(min(ystart,yend)-2));
	int xmax = (short) floor(0.25*(max(xstart,xend)+2));
	int ymax = (short) floor(0.25*(max(ystart,yend)+2));
	int i,j;
	float p[2];
	float proj[2];
	float d;
	float dir[2];

	// Richtung der Linie
	dir[0] = xend-xstart;
	dir[1] = yend-ystart;
	d = sqr(dir[0])+sqr(dir[1]);
	if (d<=0)
		return;

	d = 1/sqrt(d);
	dir[0] *=d;
	dir[1] *=d;



	for (i = max (xmin,0);i<=min(xmax,m_dimx-1);i++)
	{
		for (j=max(ymin,0);j<=min(ymax,m_dimy-1);j++)
		{
			DEBUG5("searching in Grid Tile %i %i",i,j);

			// Herausfiltern jener Felder die zu weit von der Linie entfernt sind
			p[0] = i*4.0+2;
			p[1] = j*4.0+2;
			p[0] -= xstart;
			p[1] -= ystart;

			d=p[0]*dir[0]+p[1]*dir[1];
			proj[0] = dir[0]*d;
			proj[1] = dir[1]*d;

			d = sqr(p[0]-proj[0])+sqr(p[1]-proj[1]);
			if (d>32)
			{
				DEBUG5("aborted");
				continue;
			}

				// Durchmustern der Listen im 4x4-Feld
			gu = (m_data_grid->ind(i,j));

			// Totenebene
			// Totenebene
			if (group & WorldObject::DEAD)
			{
				DEBUG5("searching dead layer");

				addObjectsOnLineFromGridunit(xstart,ystart,xend,yend, gu, result, layer,WorldObject::DEAD, omit);
			}

				// feste Objekte
			if (group & WorldObject::FIXED )
			{
				DEBUG5("searching fixed layer");


				addObjectsOnLineFromGridunit(xstart,ystart,xend,yend, gu, result, layer,WorldObject::FIXED, omit);
			}

				// lebende Objekte
			if (group & WorldObject::CREATURE)
			{


				addObjectsOnLineFromGridunit(xstart,ystart,xend,yend, gu, result, layer,WorldObject::CREATURE, omit);
			}


		}

	}
}

void Region::getProjectilesOnScreen(float center_x,float center_y, list<Projectile*>* result)
{
	map<int,Projectile*>::iterator i;
	float dx,dy,r;
	Projectile* pr;
	for (i=m_projectiles->begin();i!=m_projectiles->end();++i)
	{
		pr = (i->second);

		r=pr->getGeometry()->m_radius;
		// Abstaende, in dem die Effekte von ihrem Zentrum entfernt sichtbar sind

		dx = fabs(pr->getGeometry()->m_coordinate_x-center_x)-r;
		dy = fabs(pr->getGeometry()->m_coordinate_y-center_y)-r;

		if (dx<=12 && dy <=12)
		{
			result->push_back(pr);
		}
		else
		{
			DEBUG5("out of screen");
		}

	}
}

bool Region::insertObject (WorldObject* object, float x, float y)
{
	bool result = true;


	object->getGridLocation()->m_region = m_id;

	 // Einfügen in den Binärbaum
	if (object->getState() != WorldObject::STATE_STATIC)
	{
		result &= (m_objects->insert(make_pair(object->getId(),object))).second;
	}
	else
	{
		result &= (m_static_objects->insert(make_pair(object->getId(),object))).second;
	}


	 // Wenn das Element bereits existiert ist die Antwort false
	if (result==false)
	{
		return result;
	}

	if (object->getTypeInfo()->m_type == WorldObject::TypeInfo::TYPE_PLAYER)
	{
		DEBUG5("player entered Region");
		result &= (m_players->insert(make_pair(object->getId(),object))).second;

		// Daten der Region zum Server senden
		//object->setState(WorldObject::STATE_REGION_ENTERED);
	}
	else
	{
		// fuer Nicht Spieler Event erzeugen, dass das Objekt erschaffen wurde
		Event event;
		event.m_type = Event::OBJECT_CREATED;
		event.m_id = object->getId();
		insertEvent(event);
	}

	 // Koordinaten setzen
	object->getGeometry()->m_shape.m_coordinate_x=x;
	object->getGeometry()->m_shape.m_coordinate_y=y;

	 // Einfuegen in das Grid
	int x_g = (int) floor(0.25*x);
	int y_g = (int) floor(0.25*y);
	object->getGridLocation()->m_grid_x = x_g;
	object->getGridLocation()->m_grid_y = y_g;


	 // Testen ob das Objekt in der Region liegt
	if (x_g<0 || y_g<0 || x_g>=m_dimx || y_g>=m_dimy)
	{
		 // TODO
		 // Region neu setzen
	}
	else
	{
		if (object->getGeometry()->m_layer != WorldObject::Geometry::LAYER_SPECIAL)
		{
			Gridunit *gu = (m_data_grid->ind(x_g,y_g));

			result = gu->insertObject(object);
		}

	}

	return result;
}


bool  Region::insertProjectile(Projectile* object, float x, float y)
{
	m_projectiles->insert(make_pair(object->getId(),object));
	object->getGeometry()->m_coordinate_x = x;
	object->getGeometry()->m_coordinate_y = y;
	object->setRegion( m_id);

	// Event erzeugen: neues Projektil in der Region
	Event event;
	event.m_type = Event::PROJECTILE_CREATED;
	event.m_id = object->getId();
	insertEvent(event);

	return true;
}

bool  Region::deleteObject (WorldObject* object)
{
	bool result = true;

	 // aus dem Grid loeschen
	int x = object->getGridLocation()->m_grid_x;
	int y = object->getGridLocation()->m_grid_y;
	DEBUG5("deleting object in grid tile %i %i",x,y);

	if (object->getGeometry()->m_layer != WorldObject::Geometry::LAYER_SPECIAL)
	{
		Gridunit *gu = (m_data_grid->ind(x,y));
		result = gu->deleteObject(object);
	}

	if (result==false)
	{
		return result;
	}

	 // Aus dem Binärbaum loeschen
	if (object->getState() != WorldObject::STATE_STATIC)
	{
		m_objects->erase(object->getId());
	}
	else
	{
		m_static_objects->erase(object->getId());
	}

	if (object->getTypeInfo()->m_type == WorldObject::TypeInfo::TYPE_PLAYER)
	{
		DEBUG("Player deleted");
		m_players->erase(object->getId());

	}

	return result;
}

bool Region::moveObject(WorldObject* object, float x, float y)
{
	bool result = true;
	// Wenn NULL Zeiger übergeben -> Fehler anzeigen
	if (object == 0)
		return false;

	// Testen ob das Objekt innerhalb des 4*4 Grid in ein neues Feld kommt
	int x_old = object->getGridLocation()->m_grid_x;
	int y_old = object->getGridLocation()->m_grid_y;
	int x_new = (int) floor(0.25*x);
	int y_new = (int) floor(0.25*y);

	if (x_old == x_new && y_old == y_new)
	{
		object->getGeometry()->m_shape.m_coordinate_x=x;
		object->getGeometry()->m_shape.m_coordinate_y=y;
		return true;
	}
	// Testen ob das Objekt in der Region liegt
	if (x_new<0 || y_new<0 || x_new>=m_dimx || y_new>=m_dimy)
	{
		 // TODO
		 // Region neu setzen
	}
	else
	{
		Gridunit *gu = (m_data_grid->ind(x_old,y_old));
		result =gu->deleteObject(object);
		gu = (m_data_grid->ind(x_new,y_new));
		result = gu->insertObject(object);

		object->getGridLocation()->m_grid_x=x_new;
		object->getGridLocation()->m_grid_y=y_new;

		object->getGeometry()->m_shape.m_coordinate_x=x;
		object->getGeometry()->m_shape.m_coordinate_y=y;
	}

	return result;
}


bool Region::changeObjectGroup(WorldObject* object,WorldObject::Group group )
{
	bool result = true;

	int x = object->getGridLocation()->m_grid_x;
	int y = object->getGridLocation()->m_grid_y;
	DEBUG5("changing object in grid tile %i %i",x,y);

	Gridunit *gu = (m_data_grid->ind(x,y));
	result = gu->moveObject(object,group);


	return result;

}

void Region::deleteProjectile(Projectile* proj)
{
	int id = proj->getId();

	if (m_projectiles->count(id)!=0)
	{
		m_projectiles->erase(m_projectiles->find(id));
	}
}

void Region::update(float time)
{

	DEBUG5("\nUpdate aller WeltObjekte starten\n");
	//DEBUG("m_players %p",m_players);
	// Iterator zum durchmustern einer solchen Liste
	map<int,WorldObject*>::iterator iter;
	WorldObject* object;
	WorldObject::Geometry* wob;
	map<int,Projectile*>::iterator it3;


	// alle Eventsmasken loeschen
	for (iter =m_objects->begin(); iter!=m_objects->end(); ++iter)
	{
		iter->second->clearEventMask();
	}

	for (it3 = m_projectiles->begin(); it3 !=m_projectiles->end();++it3)
	{
		it3->second->clearEventMask();
	}

	// Durchmustern aller WorldObjects
	for (iter =m_objects->begin(); iter!=m_objects->end(); )
	{
		object = iter->second;
		wob = object->getGeometry();
		DEBUG5("\nObjekt: %f %f key: %i layer %x",wob->m_shape.m_coordinate_x,wob->m_shape.m_coordinate_y ,object->getId(),object->getGeometry()->m_layer);
		if (object->getDestroyed()==true)
		{
			// Objekte selbststaendig loeschen darf nur der Server
			if (m_server)
			{
				DEBUG5("Objekt gelöscht: %i \n",object->getId());
				Event event;
				event.m_type = Event::OBJECT_DESTROYED;
				event.m_id = object->getId();
				insertEvent(event);

				++iter;
				object->destroy();
				deleteObject(object);
				delete object;
				continue;
			}
			else
			{
				++iter;
			}
		}
		else
		{
			// Polymorpher Funktionsaufruf
			object->update(time);
			++iter;

		}

	}
	DEBUG5("Update aller WeltObjekte abgeschlossen\n\n");

	// alle Projektile updaten
	Projectile* pr =0;

	for (it3 = m_projectiles->begin(); it3 !=m_projectiles->end();)
	{
		pr = (it3->second);
		if (pr->getState() == Projectile::DESTROYED)
		{
			// Projektile selbststaendig loeschen darf nur der Server
			if (m_server)
			{
				Event event;
				event.m_type = Event::PROJECTILE_DESTROYED;
				event.m_id = pr->getId();
				insertEvent(event);

				DEBUG5("deleting projectile %p",pr);
				m_projectiles->erase(it3++);
				delete pr;
				DEBUG5("loesche projektil");
			}
			else
			{
			 	++it3;
			}

		}
		else
		{
			pr->update(time);
			++it3;
		}
	}
	DEBUG5("update projektile abgeschlossen");

	if (m_server)
	{
		// Events fuer geaenderte Objekte / Projektile erzeugen
		for (iter =m_objects->begin(); iter!=m_objects->end(); ++iter)
		{
			object = iter->second;

			// Events durch Spieler werden global behandelt, daher hier nicht beruecksichtigen
			if (object->getTypeInfo()->m_type == WorldObject::TypeInfo::TYPE_PLAYER)
			{
				continue;
			}

			if (object->getEventMask() !=0)
			{
				Event event;
				event.m_type = Event::OBJECT_STAT_CHANGED;
				event.m_data = object->getEventMask();
				event.m_id = object->getId();
				insertEvent(event);
			}
		}

		for (it3 = m_projectiles->begin(); it3 !=m_projectiles->end();++it3)
		{
			pr = (it3->second);
			if (pr->getEventMask() !=0)
			{
				Event event;
				event.m_type = Event::PROJECTILE_STAT_CHANGED;
				event.m_data = pr->getEventMask();
				event.m_id = pr->getId();
				insertEvent(event);
			}
		}
	}
}

void Region::getRegionData(CharConv* cv)
{
	// Dimension des Feldes angeben
	cv->toBuffer((short) m_dimx);
	cv->toBuffer((short) m_dimy);

	// Tiles eintragen
	int i,j;
	for (i =0;i<m_dimx*2;i++)
	{
		for (j=0;j<m_dimy*2;j++)
		{
			cv->toBuffer(*(m_tiles->ind(i,j)));
		}
	}

	// Anzahl der statischen Objekte eintragen
	DEBUG("static objects: %i",m_static_objects->size());
	cv->toBuffer<short>((short) m_static_objects->size());

	// statische Objekte in den Puffer eintragen
	map<int,WorldObject*>::iterator it;
	for (it = m_static_objects->begin();it!=m_static_objects->end();++it)
	{
		(it->second)->toString(cv);
		DEBUG5("static object: %s",(it->second)->getNameId().c_str());
	}


	// Anzahl der nicht  statischen Objekte eintragen
	DEBUG("nonstatic objects: %i",m_objects->size());
	cv->toBuffer<short>((short) m_objects->size());

	// nicht statische Objekte in den Puffer eintragen
	map<int,WorldObject*>::iterator jt;
	for (jt = m_objects->begin();jt!=m_objects->end();++jt)
	{
		DEBUG5("write offset: %i",cv->getBitStream()->GetNumberOfBitsUsed());
		(jt->second)->toString(cv);

		DEBUG5("object: %s",(jt->second)->getNameId().c_str());
	}

	// Anzahl der Projektile eintragen
	cv->toBuffer<short>((short) m_projectiles->size());
	DEBUG("projectiles: %i",m_projectiles->size());

	// Projektile in den Puffer eintragen
	map<int,Projectile*>::iterator kt;
	for (kt = m_projectiles->begin(); kt != m_projectiles->end(); ++kt)
	{
		kt->second->toString(cv);
	}

	cv->toBuffer<short>((short) m_drop_items->size());
	DEBUG("dropped items: %i",m_drop_items->size());

	//  Items in den Puffer eintragen
	map<int,DropItem*>::iterator lt;
	for (lt = m_drop_items->begin(); lt != m_drop_items->end(); ++lt)
	{
		cv->toBuffer(lt->second->m_x);
		cv->toBuffer(lt->second->m_y);
		lt->second->m_item->toString(cv);
	}
}


void Region::createObjectFromString(CharConv* cv, map<int,WorldObject*>* players)
{
	char type;
	char subt[11];
	subt[10] ='\0';
	int id;

	WorldObject* obj;
	float x,y;


	DEBUG5("read offset: %i",cv->getBitStream()->GetReadOffset());

	cv->fromBuffer(type);
	cv->fromBuffer(subt,10);
	cv->fromBuffer(id);

	DEBUG5("object %s id %i",subt,id);

		// alle Objekte ausser den Spielern werden neu angelegt
		// die Spieler existieren schon
	if (type != WorldObject::TypeInfo::TYPE_PLAYER)
	{
		obj = ObjectFactory::createObject((WorldObject::TypeInfo::ObjectType) type, std::string(subt),id);
	}
	else
	{
		if (players->count(id) ==0)
		{
			ERRORMSG("player (%s) with id %i does not exist",subt,id);
		}
		obj = (*players)[id];
	}

	obj->fromString(cv);

	x = obj->getGeometry()->m_shape.m_coordinate_x;
	y = obj->getGeometry()->m_shape.m_coordinate_y;


	insertObject(obj,x,y);
}


void Region::createProjectileFromString(CharConv* cv)
{

	char type,frac;
	int id;
	Projectile* proj;
	float x,y;

	cv->fromBuffer(type);
	cv->fromBuffer(frac);
	cv->fromBuffer(id);

	DEBUG5("new projectile %i frac %i id %i",type,frac,id);

	proj = new Projectile(m_world, (Projectile::ProjectileType) type, (WorldObject::TypeInfo::Fraction) frac, id);

	proj->fromString(cv);

	x = proj->getGeometry()->m_coordinate_x;
	y = proj->getGeometry()->m_coordinate_y;

	insertProjectile(proj,x,y);
}

void Region::createItemFromString(CharConv* cv)
{
	char type;
	char subtype[11];
	subtype[10] ='\0';
	Item* item;
	int id;
	short sx,sy;
	cv->fromBuffer(sx);
	cv->fromBuffer(sy);

	cv->fromBuffer<char>(type);
	cv->fromBuffer(subtype,10);
	cv->fromBuffer(id);

	item = ItemFactory::createItem((Item::Type) type, std::string(subtype),id);
	item->fromString(cv);

	DropItem* di = new DropItem;
	di->m_item = item;
	di->m_x = sx;
	di->m_y = sy;
	DEBUG("dropped item %i at %i %i", id,sx,sy);
	di->m_time = 0;

	m_drop_items->insert(make_pair(id,di));
	m_drop_item_locations->insert(make_pair(sx*10000+sy,di));

}


void Region::setRegionData(CharConv* cv,map<int,WorldObject*>* players)
{
	// Groesse der Region wird schon vorher eingelesen
	// Tiles eintragen
	int i,j;
	for (i =0;i<m_dimx*2;i++)
	{
		for (j=0;j<m_dimy*2;j++)
		{
			cv->fromBuffer(*(m_tiles->ind(i,j)));
		}
	}


	// alle bisherigen statischen Objekte entfernen
	map<int,WorldObject*>::iterator it;
	for (it = m_static_objects->begin();it!=m_static_objects->end();it++)
	{
		it->second->destroy();
		deleteObject(it->second);
		delete it->second;
	}
	m_static_objects->clear();

	// alle bisherigen nichtstatischen Objekte entfernen
	// die SpielerObjekte bleiben erhalten, alle anderen werden geloescht
	map<int,WorldObject*>::iterator jt;
	for (jt = m_objects->begin();jt!=m_objects->end();jt++)
	{
		if (jt->second->getTypeInfo()->m_type != WorldObject::TypeInfo::TYPE_PLAYER)
		{
			jt->second->destroy();
			deleteObject(jt->second);
			delete jt->second;
		}
	}
	m_objects->clear();
	m_players->clear();

	// statische Objekte einlesen
	short nr_stat;
	cv->fromBuffer<short>(nr_stat);
	DEBUG("static objects: %i",nr_stat);

	for (int i=0; i<nr_stat;i++)
	{
		createObjectFromString(cv,players);
	}



	// neue Objekte einlesen
	short nr_nonstat;
	cv->fromBuffer<short>(nr_nonstat);
	DEBUG("nonstatic objects: %i",nr_nonstat);

	for (int i=0; i<nr_nonstat;i++)
	{
		createObjectFromString(cv,players);
	}


	// Anzahl der Projektile einlesen
	short nr_proj;
	cv->fromBuffer<short>(nr_proj);
	DEBUG("projectiles: %i",nr_proj);
	// Projektile einlesen
	for (int i=0; i<nr_proj;i++)
	{
		createProjectileFromString(cv);
	}

	// Anzahl Gegenstaende einlesen
	short nr_items;
	cv->fromBuffer<short>(nr_items);
	DEBUG("items: %i",nr_items);
	// Gegenstaende einlesen
	for (int i=0; i<nr_items;i++)
	{
		createItemFromString(cv);

	}

}



void Region::setTile(Tile tile,short x, short y)
{
	*(m_tiles->ind(x,y)) = (char) tile;
}

bool  Region::dropItem(Item* item, float x, float y)
{
	// Menge der bereits getesteten Felder
	set<int> tfields;

	// Schlange mit zu testenden Feldern
	queue<int> fields;

	// Position in 0.5 x 0.5 Feldern
	int sx = (int) (x*2);
	int sy = (int) (y*2);
	int i;
	fields.push(sx*10000+sy);


	// Flaeche auf die das Item gedroppt wird
	Shape s;
	s.m_type= Shape::RECT;
	s.m_extent_x = 0.5;
	s.m_extent_y = 0.5;

	list<WorldObject*> res;

	// eine Stelle suchen an der das Item fallen gelassen werden kann
	while (!fields.empty())
	{
		// erstes Feld aus der Queue nehmen
		i = fields.front();
		sx = i /10000;
		sy = i %10000;
		fields.pop();

		DEBUG5("testing field %i %i",sx,sy);

		// Testen ob dort keine festen Objekte sind
		s.m_coordinate_x = sx*0.5;
		s.m_coordinate_y = sy*0.5;

		res.clear();
		getObjectsInShape(&s,&res,WorldObject::Geometry::LAYER_BASE,WorldObject::FIXED,0,true);

		if (!res.empty())
		{
			// Objekt im Weg
			res.clear();
			continue;
		}

		// Testen, ob dort nicht schon ein Item liegt
		if (m_drop_item_locations->find(i) == m_drop_item_locations->end())
		{
			DEBUG5("field is free");
			// Stelle ist frei
			// Item einfuegen
			DropItem* di = new DropItem;
			di->m_item = item;
			di->m_x = (short) sx;
			di->m_y = (short) sy;
			DEBUG5("dropped item %i", sx*10000+sy);
			di->m_time = 0;

			m_drop_items->insert(make_pair(di->m_item->m_id,di));
			m_drop_item_locations->insert(make_pair(i,di));


			DEBUG5("items dropped at %i %i",sx,sy);

			if (m_world->isServer())
			{
				Event event;
				event.m_type = Event::ITEM_DROPPED;
				event.m_id = di->m_item->m_id;

				insertEvent(event);
			}

			return true;

		}
		else
		{
			// an der Stelle liegt bereits ein Item
			tfields.insert(i);

			// Nachbarfelder in die Liste der zu untersuchenden Felder einfuegen
			if (sx>0)
			{
				i = (sx-1)*10000+sy;
				if (tfields.find(i)==tfields.end())
				{
					fields.push(i);
				}
			}

			if (sy>0)
			{
				i = (sx)*10000+sy-1;
				if (tfields.find(i)==tfields.end())
				{
					fields.push(i);
				}
			}

			if (sx<m_dimx*8-1)
			{
				i = (sx+1)*10000+sy;
				if (tfields.find(i)==tfields.end())
				{
					fields.push(i);
				}
			}

			if (sy<m_dimy*8-1)
			{
				i = (sx)*10000+sy+1;
				if (tfields.find(i)==tfields.end())
				{
					fields.push(i);
				}
			}

		}

	}

	// keine Stelle gefunden um das Item zu droppen
	return false;

}

Item*  Region::getItemAt(float x, float y)
{
	map<int,DropItem*>::iterator it;
	short sx = (int) (x*2);
	short sy = (int) (y*2);
	int pos = sx*10000 + sy;

	it = m_drop_item_locations->find(pos);
	if (it == m_drop_items->end())
	{
		return 0;
	}
	else
	{
		return it->second->m_item;
	}
}

Item* Region::getItem(int id)
{
	map<int,DropItem*>::iterator it;
	it = m_drop_items->find(id);
	if (it == m_drop_items->end())
	{
		return 0;
	}
	else
	{
		return it->second->m_item;
	}
}

DropItem* Region::getDropItem(int id)
{
	map<int,DropItem*>::iterator it;
	it = m_drop_items->find(id);
	if (it == m_drop_items->end())
	{
		return 0;
	}
	else
	{
		return it->second;
	}
}


bool Region::deleteItem(int id, bool delitem)
{

	map<int,DropItem*>::iterator it;
	map<int,DropItem*>::iterator it2;
	it = m_drop_items->find(id);
	if (it == m_drop_items->end())
	{
		return false;
	}
	else
	{
		// Item Wrapper loeschen
		int pos = 10000* it->second->m_x + it->second->m_y;
		it2 = m_drop_item_locations->find(pos);

		Event event;
		event.m_type = Event::ITEM_REMOVED;
		event.m_id = it->second->m_item->m_id;
		insertEvent(event);

		if (delitem)
		{
			delete it->second->m_item;
		}
		delete (it->second);

		m_drop_items->erase(it);
		m_drop_item_locations->erase(it2);



		return true;
	}
}




