#ifndef TEMPLATELOADER_H
#define TEMPLATELOADER_H


#include <map>
#include <list>
#include "region.h"
#include "objectfactory.h"

#include "../tinyxml/tinyxml.h"
#include <string>
#include <cstring>



/**
 * \class TemplateLoader
 * \brief Laedt verschiedene Templates aus einer XML-Datei
 */
class TemplateLoader
{
	
	public:
		/**
	 * \fn static bool loadObjectTemplateData(const char* pFilename)
		 * \brief Liest Daten zu Objekt Templates aus der angegebenen Datei
		 * \parampFilename Name der XML Datei
		 */
		static bool loadObjectTemplateData(const char* pFilename);
		
		/**
		 * \fn static bool loadObjectTemplate(TiXmlNode* node)
		 * \brief Liest die Daten zu einem Objekt Template aus dem Knoten
		 * \param node XML Knoten
		 */
		static bool loadObjectTemplate(TiXmlNode* node);
		
		/**
		 * \fn static bool loadObjectGroupTemplateData(const char* pFilename)
		 * \brief Liest Daten zu Objektgruppen Templates aus der angegebenen Datei
		 * \parampFilename Name der XML Datei
		 */
		static bool loadObjectGroupTemplateData(const char* pFilename);

		
		/**
		 * \fn static bool loadObjectGroupTemplate(TiXmlNode* node)
		 * \brief Liest die Daten zu einem Objektgruppen Template aus dem Knoten
		 * \param node XML Knoten
		 */
		static bool loadObjectGroupTemplate(TiXmlNode* node);
		

};


#endif

