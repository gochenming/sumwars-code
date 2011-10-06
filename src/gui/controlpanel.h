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


#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include "itemwindow.h"

/**
 * \class ControlPanel
 * \brief Fenster fuer die Steuerleiste
 */
class ControlPanel : public ItemWindow
{
	public:
		/**
	 * \fn ControlPanel (Document* doc)
	 * \brief Konstruktor
		 */
		ControlPanel (Document* doc);
	
		/**
		 * \fn virtual void update()
		 * \brief aktualisiert den Inhalt des Fensters
		 */
		virtual void update();
		
		/**
		 * \fn virtual void updateTranslation
		 * \brief aktualisiert die Uebersetzungen
		*/
		virtual void updateTranslation();
		
		/**
		 * \fn void onButtonCharInfoClicked(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Klick auf CharakterInfo Button
		 */
		bool onButtonCharInfoClicked(const CEGUI::EventArgs& evt);
		
		/**
		 * \fn void onButtonSkilltreeClicked(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Klick auf Skilltree Button
		 */
		bool onButtonSkilltreeClicked(const CEGUI::EventArgs& evt);
		
	private:
		/**
		* \fn bool onButtonSaveExitClicked(const CEGUI::EventArgs& evt)
		* \brief Behandelt Klick auf Speichern/Beenden
		*/
		bool onButtonSaveExitClicked(const CEGUI::EventArgs& evt);
		
		/**
		 * \fn bool onButtonOptionsClicked(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Klick auf Optionen
		 */
		bool onButtonOptionsClicked(const CEGUI::EventArgs& evt);
		
		/**
		 * \fn void onButtonInventoryClicked(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Klick auf Inventar Button
		 */
		bool onButtonInventoryClicked(const CEGUI::EventArgs& evt);

		/**
		 *\fn bool onButtonPartyClicked(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Klick auf Party Button
		 */
		bool onButtonPartyClicked(const CEGUI::EventArgs& evt);

		/**
		 * \fn void onButtonOpenChatClicked(const CEGUI::EventArgs& evt)
		 * \brief Behandelt Klick auf Chat oeffnen Button
		 */
		bool onButtonOpenChatClicked(const CEGUI::EventArgs& evt);
		
		
		/**
		 * \fn bool onButtonQuestInfoClicked(const CEGUI::EventArgs& evt);
		 * \brief Behandelt Klick auf QuestInfo Button
		 */
		bool onButtonQuestInfoClicked(const CEGUI::EventArgs& evt);
		
		
		
};

#endif

