/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ceguiutility.h"
#include "CEGUI/CEGUI.h"
#include <iostream>
#include <algorithm>
#include "debug.h"


const CEGUI::String CEGUIUtility::EventMouseEntersWindowArea ()
{
#ifdef CEGUI_07
	return CEGUI::Window::EventMouseEnters;
#else
	return CEGUI::Window::EventMouseEntersArea;
#endif
}

const CEGUI::String CEGUIUtility::EventMouseEntersPushButtonArea ()
{
#ifdef CEGUI_07
	return CEGUI::PushButton::EventMouseEnters;
#else
	return CEGUI::PushButton::EventMouseEntersArea;
#endif
}

const CEGUI::String CEGUIUtility::EventMouseEntersListBoxArea ()
{
#ifdef CEGUI_07
	return CEGUI::Listbox::EventMouseEnters;
#else
	return CEGUI::Listbox::EventMouseEntersArea;
#endif
}

const CEGUI::String CEGUIUtility::EventMouseEntersScrollbarArea ()
{
#ifdef CEGUI_07
	return CEGUI::Scrollbar::EventMouseEnters;
#else
	return CEGUI::Scrollbar::EventMouseEntersArea;
#endif
}


const CEGUI::String CEGUIUtility::EventMouseEntersComboboxArea ()
{
#ifdef CEGUI_07
	return CEGUI::Combobox::EventMouseEnters;
#else
	return CEGUI::Combobox::EventMouseEntersArea;
#endif
}

const CEGUI::String CEGUIUtility::EventMouseEntersComboDroplistArea ()
{
#ifdef CEGUI_07
	return CEGUI::ComboDropList::EventMouseEnters;
#else
	return CEGUI::ComboDropList::EventMouseEntersArea;
#endif
}

const CEGUI::String CEGUIUtility::EventMouseEntersEditboxArea ()
{
#ifdef CEGUI_07
	return CEGUI::Editbox::EventMouseEnters;
#else
	return CEGUI::Editbox::EventMouseEntersArea;
#endif
}

const CEGUI::String CEGUIUtility::EventMouseEntersToggleButtonArea ()
{
#ifdef CEGUI_07
	return CEGUI::Checkbox::EventMouseEnters;
#else
	return CEGUI::ToggleButton::EventMouseEntersArea;
#endif
}

const CEGUI::String CEGUIUtility::EventToggleButtonStateChanged ()
{
#ifdef CEGUI_07
	return CEGUI::Checkbox::EventCheckStateChanged;
#else
	return CEGUIUtility::ToggleButton::EventSelectStateChanged;
#endif
}

const CEGUI::String CEGUIUtility::EventMouseLeavesWindowArea ()
{
#ifdef CEGUI_07
	return CEGUI::Window::EventMouseLeaves;
#else
	return CEGUI::Window::EventMouseLeavesArea;
#endif
}


std::string CEGUIUtility::stripColours(const std::string &input)
{
    std::string output = input;

    std::string::size_type pos = output.find("[");
    while (pos != std::string::npos)
    {
        output.erase(pos,pos+19);
        pos = output.find("[");
    }

    return output;
}

std::list< std::string > CEGUIUtility::getTextAsList(const std::string &text)
{
    std::list<std::string> l;
    std::string temp = text;
	
    std::string::size_type pos = text.find_first_of(LINE_ENDING);
    while (pos != std::string::npos)
    {
        // ignore new line directly at the beginning of the string
        if (pos != 0)
        {
            l.push_back(temp.substr(0, pos));
            temp.erase(0, pos+ERASE_CNT);
            pos = temp.find(LINE_ENDING);
        }
        else
		{
			temp.erase(0, ERASE_CNT);
			//l.push_back(" ");
			pos = temp.find(LINE_ENDING);
		}
    }
    
    if (temp != "")
	{
		l.push_back(temp);
	}
	
    return l;
}

CEGUI::UVector2 CEGUIUtility::getWindowSizeForText(std::list<std::string> list, const CEGUI::Font *font, std::string &added)
{
    using namespace CEGUI;

    float textWidth = 0.0f;
    float textHeight = 0.0f;

#ifdef CEGUI_07
	CEGUI::Size screenSize = CEGUI::System::getSingleton().getRenderer()->getDisplaySize();
#else
    CEGUI::Sizef screenSize (CEGUI::System::getSingleton().getRenderer()->getDisplaySize ());
#endif

    added.clear();
	size_t count = 0;
	
    for (std::list<std::string>::iterator iter = list.begin();
		iter != list.end();
		++iter)
    {
		std::string & s = *iter;
		
		if (!s.empty())
		{
			std::string::size_type word_pos = 0;
			while ( word_pos!=std::string::npos )
			{
				word_pos = s.find ( LINE_ENDING, word_pos );
				if ( word_pos != std::string::npos )
				{
					++count;

					// start next search after this word
					word_pos += 2;
				}
			}
			
			added += s + '\n';

			float tempwidth = font->getTextExtent(stripColours(s).c_str()) ;
			if (tempwidth > textWidth)
				textWidth = tempwidth;
		}
    }

	// increase the counter intil we have at least one line break more than we have text
	if (count <= list.size())  
		count = list.size() + 1;

	textHeight = count * (font->getLineSpacing());

    return UVector2(UDim((textWidth/screenSize.d_width) + 0.05f, 0), UDim(textHeight / screenSize.d_height, 0) );
}

CEGUI::UVector2 CEGUIUtility::getWindowSizeForText (std::string text, const CEGUI::Font* font)
{
    std::list<std::string> l = getTextAsList(text);
    std::string s;
    return getWindowSizeForText(l, font, s);
}

std::string CEGUIUtility::getStdColourAsString(int col)
{
	switch(col)
	{
		case Red:
			return "[colour='FFFF0000']";
		case Green:
			return "[colour='FF00FF00']";
		case Blue:
			return "[colour='FF0000FF']";
		case White:
			return "[colour='FFFFFFFF']";
		case Black:
			return "[colour='FF000000']";
		default:
			return "";
	}		
}

std::string CEGUIUtility::getColourizedString(int colour, std::string text, int afterColour)
{
	if(afterColour == 0)
		return getStdColourAsString(colour) + text;
	else
		return getStdColourAsString(colour) + text + getStdColourAsString(afterColour);
}

const size_t CEGUIUtility::getNextWord(const CEGUI::String& in_string, size_t start_idx, CEGUI::String& out_string) 
{
	out_string = CEGUI::TextUtils::getNextWord(in_string, start_idx, CEGUI::TextUtils::DefaultWrapDelimiters);
	
	return out_string.length();
}

FormatedText CEGUIUtility::fitTextToWindow(const CEGUI::String& text, float maxWidth, TextFormatting fmt, const CEGUI::Font *font, float x_scale)
{
/*	if ((fmt == LeftAligned) || (fmt == Centred) || (fmt == RightAligned) || (fmt == Justified))
	{
		return std::count(text.begin(), text.end(), static_cast<CEGUI::utf8>('\n')) + 1;
	}
	*/

	CEGUI::String newText((CEGUI::utf8*)text.c_str());
	
	// handle wraping cases
	size_t lineStart = 0, lineEnd = 0;
	CEGUI::String	sourceLine;

	CEGUI::String  whitespace = CEGUI::TextUtils::DefaultWhitespace;
	CEGUI::String	thisLine, thisWord;
	size_t	line_count = 0, currpos = 0;
	
	size_t fullPosCtr = 0;
	
	while (lineEnd < text.length())
	{
		if ((lineEnd = text.find_first_of(LINE_ENDING, lineStart)) == CEGUI::String::npos)
		{
			lineEnd = text.length();
		}
		
		sourceLine = text.substr(lineStart, lineEnd - lineStart);
		lineStart = lineEnd + 1;
		
		// get first word.
		currpos = getNextWord(sourceLine, 0, thisLine);
		
		// while there are words left in the string...
		while (CEGUI::String::npos != sourceLine.find_first_not_of(whitespace, currpos))
		{
			// get next word of the string...
			currpos += getNextWord(sourceLine, currpos, thisWord);
			
			//float davor = (font->getTextExtent(thisLine, x_scale));
			//float danach = (font->getTextExtent(thisLine, x_scale) + font->getTextExtent(thisWord, x_scale));
			
			// if the new word would make the string too long
			if ((font->getTextExtent(thisLine, x_scale) + font->getTextExtent(thisWord, x_scale)) > maxWidth)
			{
				// too long, so that's another line of text
				fullPosCtr += thisLine.size();
				line_count++;
				
				// remove whitespace from next word - it will form start of next line
				thisWord = thisWord.substr(thisWord.find_first_not_of(whitespace));
				
				newText.insert(fullPosCtr+1, LINE_ENDING);
				fullPosCtr += 2;
				
				// reset for a new line.
				thisLine.clear();
			}
			
			// add the next word to the line
			thisLine += thisWord;
		}
		
		fullPosCtr += thisLine.size();
		// plus one for final line
		line_count++;
	}
	FormatedText formt;
	formt.text = newText;
	formt.lines = line_count;
	return formt;
}


//
//	Other utility static functions
//


		/**
			Add a child widget to a CEGUI widget
			@param parentPtr The parent pointer to the CEGUI Window
			@param childPtr The child pointer to the CEGUI Window.
		*/
		void CEGUIUtility::addChildWidget (CEGUI::Window* parentPtr, CEGUI::Window* childPtr)
		{
#ifdef CEGUI_07
			parentPtr->addChildWindow (childPtr);
#else
			parentPtr->addChild (childPtr);
#endif
		}


		/**
			Remove a child widget from a CEGUI widget
			@param parentPtr The parent pointer to the CEGUI Window
			@param childPtr The child pointer to the CEGUI Window.
		*/
		void CEGUIUtility::removeChildWidget (CEGUI::Window* parentPtr, CEGUI::Window* childPtr)
		{
#ifdef CEGUI_07
			parentPtr->removeChildWindow (childPtr);
#else
			parentPtr->removeChild (childPtr);
#endif
		}


#ifdef CEGUI_07
	CEGUI::ImagesetManager& CEGUIUtility::getImageManager ()
	{
		return CEGUI::ImagesetManager::getSingleton();
	}
#else
	CEGUI::ImageManager& CEGUIUtility::getImageManager ()
	{
		return CEGUI::ImageManager::getSingleton();
	}
#endif

	void CEGUIUtility::loadImageset (const CEGUI::String& name)
	{
#ifdef CEGUI_07
		CEGUI::ImagesetManager::getSingleton().create(name);
#else
		CEGUI::ImageManager::getSingleton().loadImageset(name);
#endif
	}

	void CEGUIUtility::addManagedImageFromFile (const CEGUI::String& name, const CEGUI::String& fileName, const CEGUI::String& resourceGroup)
	{
#ifdef CEGUI_07
		CEGUI::ImagesetManager::getSingleton().createFromImageFile (name, fileName, resourceGroup);
#else
		CEGUI::ImageManager::getSingleton().addFromImageFile (name, fileName, resourceGroup);
#endif
	}



CEGUI::Window* CEGUIUtility::getWindow (const CEGUI::String& name)
{
	CEGUI::System* ceguiSysPtr_ = CEGUI::System::getSingletonPtr ();
	return getWindowForSystem (ceguiSysPtr_, name);
}

// static
CEGUI::Window* CEGUIUtility::getWindowForSystem (CEGUI::System* sys, const CEGUI::String& name)
{
#ifdef CEGUI_07
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton ();
	if (win_mgr.isWindowPresent (name))
	{
		return win_mgr.getWindow (name);
	}
#else
	CEGUI::Window* root = sys->getDefaultGUIContext().getRootWindow();
	if (root->isChild (name))
	{
		return root->getChild (name);
	}
	else
	{
	}
#endif
	return 0;
}


	CEGUI::Window* CEGUIUtility::getWindowForLoadedLayout (CEGUI::Window* parentWnd, const CEGUI::String& name)
	{
#ifdef CEGUI_07
		CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton ();
		if (win_mgr.isWindowPresent (name))
		{
			return win_mgr.getWindow (name);
		}
#else
		if (0 == parentWnd)
		{
			return 0;
		}

		if (parentWnd->isChild (name))
		{
			return parentWnd->getChild (name);
		}
		else
		{
			if (parentWnd->getName () == name)
			{
				return parentWnd;
			}
			else
			{
				DEBUG ("Could not find child [%s] within %d children", name.c_str (), parentWnd->getChildCount ());
			}
		}
#endif
		return 0;
	}


	CEGUI::Window* CEGUIUtility::getWindowContainingMouse (CEGUI::System* sys)
	{
#ifdef CEGUI_07
		return sys->getWindowContainingMouse ();
#else
		return sys->getDefaultGUIContext ().getWindowContainingMouse ();
#endif
	}

	bool CEGUIUtility::isWindowPresent (const CEGUI::String& name)
	{
		return (0 != getWindow (name));
	}

	bool CEGUIUtility::isWindowPresentForSystem (CEGUI::System* sys, const CEGUI::String& name)
	{
		return (0 != getWindowForSystem (sys, name));
	}

	CEGUI::Window* CEGUIUtility::loadLayoutFromFile (const CEGUI::String& name, const CEGUI::String& resGroup)
	{
#ifdef CEGUI_07
		CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton ();
		return win_mgr.loadWindowLayout (name, resGroup);
#else
		CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton ();
		return win_mgr.loadLayoutFromFile (name, resGroup);
#endif
	}
		


bool CEGUIUtility::injectKeyDown (OIS::KeyCode myKey)
{
	return injectKeyDown (CEGUI::System::getSingletonPtr(), myKey);
}

bool CEGUIUtility::injectKeyDown (CEGUI::System* sys, OIS::KeyCode myKey)
{
	if (!sys)
	{
		return false;
	}
#ifdef CEGUI_07
	return sys->injectKeyDown (myKey);
#else
	return sys->getDefaultGUIContext ().injectKeyDown (static_cast<CEGUI::Key::Scan> (myKey));
#endif
}

bool CEGUIUtility::injectKeyUp (OIS::KeyCode myKey)
{
	return injectKeyUp (CEGUI::System::getSingletonPtr(), myKey);
}


bool CEGUIUtility::injectKeyUp (CEGUI::System* sys, OIS::KeyCode myKey)
{
	if (!sys)
	{
		return false;
	}
#ifdef CEGUI_07
	return sys->injectKeyUp (myKey);
#else
	return sys->getDefaultGUIContext ().injectKeyUp (static_cast<CEGUI::Key::Scan> (myKey));
#endif
}


bool CEGUIUtility::injectChar (int myKey)
{
	return injectChar (CEGUI::System::getSingletonPtr(), myKey);
}


bool CEGUIUtility::injectChar (CEGUI::System* sys, int myKey)
{
	if (!sys)
	{
		return false;
	}
#ifdef CEGUI_07
	return sys->injectChar (myKey);
#else
	return sys->getDefaultGUIContext ().injectChar (static_cast<CEGUI::Key::Scan> (myKey));
#endif
}


bool CEGUIUtility::injectMousePosition (float x, float y)
{
	return injectMousePosition (CEGUI::System::getSingletonPtr(), x, y);
}

bool CEGUIUtility::injectMousePosition (CEGUI::System* sys, float x, float y)
{
	if (!sys)
	{
		return false;
	}
#ifdef CEGUI_07
	return sys->injectMousePosition (x, y);
#else
	return sys->getDefaultGUIContext ().injectMousePosition (x, y);
#endif
}

bool CEGUIUtility::injectMouseWheelChange (float pos)
{
	return injectMouseWheelChange (CEGUI::System::getSingletonPtr(), pos);
}

bool CEGUIUtility::injectMouseWheelChange (CEGUI::System* sys, float pos)
{
	if (!sys)
	{
		return false;
	}
#ifdef CEGUI_07
	return sys->injectMouseWheelChange (pos);
#else
	return sys->getDefaultGUIContext ().injectMouseWheelChange (pos);
#endif
}

bool CEGUIUtility::injectMouseButtonDown (OIS::MouseButtonID btn)
{
	return injectMouseButtonDown (CEGUI::System::getSingletonPtr(), btn);
}

bool CEGUIUtility::injectMouseButtonDown (CEGUI::System* sys, OIS::MouseButtonID btn)
{
	if (!sys)
	{
		return false;
	}
#ifdef CEGUI_07
	return sys->injectMouseButtonDown (convertOISButtonToCegui (btn));
#else
	return sys->getDefaultGUIContext ().injectMouseButtonDown (convertOISButtonToCegui (btn));
#endif
}

bool CEGUIUtility::injectMouseButtonUp (OIS::MouseButtonID btn)
{
	return injectMouseButtonUp (CEGUI::System::getSingletonPtr(), btn);
}

bool CEGUIUtility::injectMouseButtonUp (CEGUI::System* sys, OIS::MouseButtonID btn)
{
	if (!sys)
	{
		return false;
	}
#ifdef CEGUI_07
	return sys->injectMouseButtonUp (convertOISButtonToCegui (btn));
#else
	return sys->getDefaultGUIContext ().injectMouseButtonUp (convertOISButtonToCegui (btn));
#endif
}

	void CEGUIUtility::injectTimePulse (float timeElapsed)
	{
		injectTimePulse (CEGUI::System::getSingletonPtr(), timeElapsed);
	}


	void CEGUIUtility::injectTimePulse (CEGUI::System* sys, float timeElapsed)
	{
		if (!sys)
		{
			return;
		}
#ifdef CEGUI_07
		sys->injectTimePulse (timeElapsed);
#else
		sys->injectTimePulse (timeElapsed);
		sys->getDefaultGUIContext ().injectTimePulse (timeElapsed);
#endif

	}

	void CEGUIUtility::loadScheme (const CEGUI::String& schemeName, const CEGUI::String& resGroup)
	{
#ifdef CEGUI_07
		CEGUI::SchemeManager::getSingleton().create ((CEGUI::utf8*)schemeName.c_str (), (CEGUI::utf8*)resGroup.c_str ());
#else
		CEGUI::SchemeManager::getSingleton().createFromFile ((CEGUI::utf8*)schemeName.c_str (), (CEGUI::utf8*)resGroup.c_str ());
#endif
	}

	void CEGUIUtility::addFont (const CEGUI::String& fontName, const CEGUI::String& resGroup)
	{
#ifdef CEGUI_07
		CEGUI::FontManager::getSingleton ().create ((CEGUI::utf8*)fontName.c_str (), (CEGUI::utf8*)resGroup.c_str ());
#else
		CEGUI::FontManager::getSingleton ().createFromFile ((CEGUI::utf8*)fontName.c_str (), (CEGUI::utf8*)resGroup.c_str ());
#endif
	}



CEGUI::MouseButton CEGUIUtility::convertOISButtonToCegui (int buttonID)
{
	switch (buttonID)
	{
	case OIS::MB_Left:
		return CEGUI::LeftButton;
	case OIS::MB_Right:
		return CEGUI::RightButton;
	case OIS::MB_Middle:
		return CEGUI::MiddleButton;
	}
	// default:
	return CEGUI::LeftButton;
}


std::string CEGUIUtility::getNameForWidget (const std::string& name)
{
#ifdef CEGUI_07
	int nPos = name.rfind('/');
	if (nPos != CEGUI::String::npos)
	{
		return name.substr (nPos + 1);
	}
#endif
	return name;
}


// Retrieve the widget with a given skin.
std::string CEGUIUtility::getWidgetWithSkin (const std::string& skin, const std::string& name)
{
	std::stringstream ss;
	ss << skin << "/" << name;
	return ss.str ();
}


std::string CEGUIUtility::getImageNameWithSkin (const std::string& skinName, const std::string& imageName)
{
	std::stringstream ss;
#ifdef CEGUI_07
	ss << "set:" << skinName << " image:" << imageName;
#else
	ss << skinName << "/" << imageName;
#endif
	return ss.str ();
}


void CEGUIUtility::setScrollPositionForWidget (const CEGUI::String& widgetName, float newScrollPosition)
{
	CEGUI::Scrollbar* scroller;
	scroller = static_cast<CEGUI::Scrollbar*> (CEGUIUtility::getWindow (widgetName));
	if (scroller)
	{
#ifdef CEGUI_07
		scroller->setScrollPosition (0.5f);
#else
		scroller->setUnitIntervalScrollPosition (0.5f);
#endif
	}
}



void CEGUIUtility::setDefaultFont (const CEGUI::String& fontName)
{
	if (CEGUI::FontManager::getSingleton ().isDefined (fontName))
	{
#ifdef CEGUI_07
		CEGUI::System::getSingletonPtr ()->setDefaultFont ((CEGUI::utf8*)fontName.c_str ());
#else
		CEGUI::System::getSingletonPtr ()->getDefaultGUIContext ().setDefaultFont ((CEGUI::utf8*)fontName.c_str ());
#endif
	}
}

CEGUI::Font* CEGUIUtility::getDefaultFont ()
{
#ifdef CEGUI_07
	return CEGUI::System::getSingleton().getDefaultFont ();
#else
	return CEGUI::System::getSingletonPtr ()->getDefaultGUIContext ().getDefaultFont ();
#endif
}


void CEGUIUtility::setDefaultMouseCursor (CEGUI::System* sys, const std::string& skinName, const std::string& cursorName)
{
#ifdef CEGUI_07
	sys->setDefaultMouseCursor ((CEGUI::utf8*)skinName.c_str (), (CEGUI::utf8*)cursorName.c_str ());
#else
	std::stringstream ss;
	ss << skinName << "/" << cursorName;
	sys->getDefaultGUIContext ().getMouseCursor ().setDefaultImage (ss.str ());
	sys->getDefaultGUIContext ().getMouseCursor ().setImage (ss.str ());
#endif
}


void CEGUIUtility::setDefaultTooltip (CEGUI::System* sys, const std::string& skinName, const std::string& cursorName)
{
#ifdef CEGUI_07
	std::stringstream ss;
	ss << skinName << "/" << cursorName;
	sys->setDefaultTooltip(ss.str ().c_str ());
	//sys->setDefaultTooltip (skinName.c_str (), cursorName.c_str ());
#else
	std::stringstream ss;
	ss << skinName << "/" << cursorName;
	sys->getDefaultGUIContext().setDefaultTooltipType(ss.str ().c_str ());
#endif
}


CEGUIUtility::Vector2f CEGUIUtility::getMouseCursorPosition (CEGUI::System* sys)
{
#ifdef CEGUI_07
	return CEGUI::MouseCursor::getSingleton().getPosition();
#else
	return sys->getDefaultGUIContext ().getMouseCursor ().getPosition ();
#endif
}

void CEGUIUtility::setWidgetSizeRel (CEGUI::Window* widget, float width, float height)
{
#ifdef CEGUI_07
	widget->setSize (CEGUI::UVector2 (cegui_reldim (width), cegui_reldim (height)));
	
#else
	CEGUI::UDim horz (width, 0);
	CEGUI::UDim vert (height, 0);
	CEGUI::USize mySize (horz, vert);
	widget->setSize (mySize);
#endif
}

CEGUI::UVector2 CEGUIUtility::getWidgetSize (CEGUI::Window* widget)
{
#ifdef CEGUI_07
	return widget->getSize ();
#else
	CEGUI::UVector2 result;
	result.d_x = widget->getSize ().d_width;
	result.d_y = widget->getSize ().d_height;
	return result;
#endif
}


void CEGUIUtility::setWidgetSizeAbs (CEGUI::Window* widget, float width, float height)
{
#ifdef CEGUI_07
	widget->setSize (CEGUI::UVector2 (cegui_reldim (width), cegui_reldim (height)));
	
#else
	CEGUI::UDim horz (width, 0);
	CEGUI::UDim vert (height, 0);
	CEGUI::USize mySize (horz, vert);
	widget->setSize (mySize);
#endif
}


void CEGUIUtility::setWidgetSize (CEGUI::Window* widget, CEGUI::UVector2 size)
{
#ifdef CEGUI_07
	widget->setSize (size);
#else
	CEGUI::UDim horz (size.d_x);
	CEGUI::UDim vert (size.d_y);
	CEGUI::USize mySize (horz, vert);
	widget->setSize (mySize);
#endif
}

void CEGUIUtility::setWidgetMaxSize (CEGUI::Window* widget, CEGUI::UVector2 size)
{
#ifdef CEGUI_07
	widget->setSize (size);
#else
	CEGUI::UDim horz (size.d_x);
	CEGUI::UDim vert (size.d_y);
	CEGUI::USize mySize (horz, vert);
	widget->setMaxSize (mySize);
#endif
}


void CEGUIUtility::setRootSheet (CEGUI::System* sys, CEGUI::Window* widget)
{
#ifdef CEGUI_07
	sys->setGUISheet (widget);
#else
	sys->getDefaultGUIContext ().setRootWindow (widget);
#endif
}

CEGUI::Window* CEGUIUtility::getRootSheet (CEGUI::System* sys)
{
#ifdef CEGUI_07
	return 	sys->getGUISheet ();
#else
	return 	sys->getDefaultGUIContext ().getRootWindow ();
#endif
}
