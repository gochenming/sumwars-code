/*
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef BENCHMARKTAB_H
#define BENCHMARKTAB_H

#include <debugtab.h>
#include <CEGUI/CEGUIWindow.h>
#include "OgreLog.h"

namespace Ogre
{
	class Root;
	class SceneManager;
	class Camera;
	class RenderTexture;
}

class BenchmarkTab : public CEGUI::Window, public DebugTab, public Ogre::LogListener
{
public:
	BenchmarkTab(const CEGUI::String& type, const CEGUI::String& name);
	
	virtual void update(OIS::Keyboard *keyboard, OIS::Mouse *mouse);

	virtual void messageLogged (const Ogre::String &message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName);

	virtual bool handleStartBenchmark(const CEGUI::EventArgs& e);
private:
	CEGUI::Window *m_tabLayout;
	CEGUI::MultiLineEditbox *m_CapsBox;
	CEGUI::PushButton *m_startBenchmarkButton;
	Ogre::Log *m_log;
	Ogre::Root *m_ogreRoot;
	Ogre::SceneManager *m_sceneManager;
public:
	static CEGUI::String WidgetTypeName;
};

#endif // BENCHMARKTAB_H
