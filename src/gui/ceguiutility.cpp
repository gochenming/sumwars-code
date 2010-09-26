#include "ceguiutility.h"
#include "CEGUI/CEGUI.h"
#include <iostream>
#include <algorithm>

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

    std::string::size_type pos = text.find_first_of("\n");
    while (pos != std::string::npos)
    {
        // ignore new line directly at the beginning of the string
        if (pos != 0)
        {
            std::string substr = temp.substr(0, pos-1);
            l.push_back(temp.substr(0, pos-1));
            temp.erase(0, pos+1);
            pos = temp.find("\n");
        }
        else
		{
			temp.erase(0, 1);
			//l.push_back(" ");
			pos = temp.find("\n");
		}
    }

    return l;
}

CEGUI::UVector2 CEGUIUtility::getWindowSizeForText(std::list<std::string> list, CEGUI::Font *font, std::string &added)
{
    using namespace CEGUI;

    float textWidth = 0.0f;
    float textHeight = 0.0f;
    Size screenSize = CEGUI::System::getSingleton().getRenderer()->getDisplaySize();

    added = "";

    std::list<std::string>::iterator iter = list.begin();
    while (iter != list.end())
    {
        std::string s = *iter;
        added += s;
        float tempwidth = font->getTextExtent(stripColours(s).c_str()) ;
        if (tempwidth > textWidth)
            textWidth = tempwidth;

        iter++;
    }

    textHeight = list.size() * font->getFontHeight();

    return UVector2(UDim((textWidth/screenSize.d_width) + 0.05f, 0), UDim(textHeight / screenSize.d_height, 0) );
}

CEGUI::UVector2 CEGUIUtility::getWindowSizeForText(std::string text, CEGUI::Font* font)
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
	if(!afterColour > 0)
		return getStdColourAsString(colour) + text;
	else
		return getStdColourAsString(colour) + text + getStdColourAsString(afterColour);
}

const size_t CEGUIUtility::getNextWord(const CEGUI::String& in_string, size_t start_idx, CEGUI::String& out_string) 
{
	out_string = CEGUI::TextUtils::getNextWord(in_string, start_idx, CEGUI::TextUtils::DefaultWrapDelimiters);
	
	return out_string.length();
}

FormatedText CEGUIUtility::fitTextToWindow(const CEGUI::String& text, const CEGUI::Rect& format_area,TextFormatting fmt, CEGUI::Font *font, float x_scale)
{
/*	if ((fmt == LeftAligned) || (fmt == Centred) || (fmt == RightAligned) || (fmt == Justified))
	{
		return std::count(text.begin(), text.end(), static_cast<CEGUI::utf8>('\n')) + 1;
	}
	*/

	CEGUI::String newText(text.c_str());
	
	// handle wraping cases
	size_t lineStart = 0, lineEnd = 0;
	CEGUI::String	sourceLine;
	
	float	wrap_width = format_area.getWidth();
	CEGUI::String  whitespace = CEGUI::TextUtils::DefaultWhitespace;
	CEGUI::String	thisLine, thisWord;
	size_t	line_count = 0, currpos = 0;
	
	while (lineEnd < text.length())
	{
		if ((lineEnd = text.find_first_of('\n', lineStart)) == CEGUI::String::npos)
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
			
			// if the new word would make the string too long
			if ((font->getTextExtent(thisLine, x_scale) + font->getTextExtent(thisWord, x_scale)) > wrap_width)
			{
				// too long, so that's another line of text
				line_count++;
				
				// remove whitespace from next word - it will form start of next line
				thisWord = thisWord.substr(thisWord.find_first_not_of(whitespace));
				
				newText.insert(currpos - thisWord.size(), "\n");
				// reset for a new line.
				thisLine.clear();
			}
			
			// add the next word to the line
			thisLine += thisWord;
		}
		
		// plus one for final line
		line_count++;
	}
	FormatedText formt;
	formt.text = newText;
	formt.lines = line_count;
	return formt;
}



