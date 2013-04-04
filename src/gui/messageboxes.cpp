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

#include "messageboxes.h"

SaveExitWindow::SaveExitWindow (Document* doc)
	:Window(doc)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::PushButton* btn;
	
	CEGUI::FrameWindow* save_exit = (CEGUI::FrameWindow*) win_mgr.loadWindowLayout("saveexitwindow.layout");
	m_window = save_exit;
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("GameExitConfirmButton"));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SaveExitWindow ::onExitGameConfirmed, this));
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("GameExitAbortButton"));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SaveExitWindow ::onExitGameAborted, this));

	// If the panel also has an auto-close button, connect it to the Cancel/Abort event.
	if (win_mgr.isWindowPresent ("SaveExitWindow__auto_closebutton__"))
	{
		btn = static_cast<CEGUI::PushButton*>( win_mgr.getWindow ("SaveExitWindow__auto_closebutton__"));
		if (btn)
		{
			btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&SaveExitWindow::onExitGameAborted, this));
		}
	}
	
	updateTranslation();
}

void SaveExitWindow::update()
{
}

void SaveExitWindow::updateTranslation()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;
	
	CEGUI::PushButton* btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow( "GameExitConfirmButton"));
	btn->setText((CEGUI::utf8*) gettext("Ok"));
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow( "GameExitAbortButton"));
	btn->setText((CEGUI::utf8*) gettext("Abort"));
	
	if (win_mgr.isWindowPresent ("SaveExitLabel"))
	{
		label = win_mgr.getWindow("SaveExitLabel");
		label->setText((CEGUI::utf8*) gettext("Save and Exit?"));
	}
	else if (win_mgr.isWindowPresent ("SaveExitWindow"))
	{
		label = win_mgr.getWindow ("SaveExitWindow");
		if (label->isPropertyPresent ("Text"))
		{
			label->setProperty ("Text", (CEGUI::utf8*) gettext("Save and Exit?"));
		}
	}

}


bool SaveExitWindow::onExitGameConfirmed(const CEGUI::EventArgs& evt)
{
	m_document->onButtonSaveExitConfirm();
	return true;
}

bool SaveExitWindow::onExitGameAborted(const CEGUI::EventArgs& evt)
{
	m_document->onButtonSaveExitAbort();
	return true;
}



//
// ----------------------------------------- Message Question Window (a window containing a question and two buttons) ----------------------------------------------
//


MessageQuestionWindow::MessageQuestionWindow (Document* doc
							, const std::string& layoutName
							, const std::string& question
							, const std::string& button1Text
							, CEGUI::Event::Subscriber subscriberButton1Callback
							, const std::string& button2Text
							, CEGUI::Event::Subscriber subscriberButton2Callback)
	: Window (doc)
	, m_button1 (button1Text)
	, m_button2 (button2Text)
	, m_layoutName (layoutName)
{
	m_question = question;
	
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::PushButton* btn;
	
	CEGUI::FrameWindow* message = (CEGUI::FrameWindow*) win_mgr.loadWindowLayout(layoutName.c_str ());
	m_window = message;

	if (win_mgr.isWindowPresent ("QuestionAnswerButton1"))
	{
		btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("QuestionAnswerButton1"));
		btn->subscribeEvent(CEGUI::PushButton::EventClicked, subscriberButton1Callback);
	}
	if (win_mgr.isWindowPresent ("QuestionAnswerButton2"))
	{
		btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("QuestionAnswerButton2"));
		btn->subscribeEvent(CEGUI::PushButton::EventClicked, subscriberButton2Callback);
	}

	updateTranslation ();
}


MessageQuestionWindow::~MessageQuestionWindow ()
{
}


void MessageQuestionWindow::updateTranslation()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* widget;

	if (win_mgr.isWindowPresent ("QuestionAnswerButton1"))
	{
		widget = win_mgr.getWindow("QuestionAnswerButton1");
		widget->setText ((CEGUI::utf8*) m_button1.c_str ());
	}
	if (win_mgr.isWindowPresent ("QuestionAnswerButton2"))
	{
		widget = win_mgr.getWindow("QuestionAnswerButton2");
		widget->setText ((CEGUI::utf8*) m_button2.c_str ());
	}
	if (win_mgr.isWindowPresent ("QuestionInfoLabel"))
	{
		widget = win_mgr.getWindow("QuestionInfoLabel");
		widget->setText ((CEGUI::utf8*) m_question.c_str ());
	}
}

void MessageQuestionWindow::setQuestion(std::string question)
{
	m_question = question;
	updateTranslation();
}


//
// ----------------------------------------- Warning Window (a window containing a notification and a button) ----------------------------------------------
//


WarningDialogWindow::WarningDialogWindow (Document* doc)
:Window(doc)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::PushButton* btn;
	
	CEGUI::FrameWindow* warning_dialog = (CEGUI::FrameWindow*) win_mgr.loadWindowLayout("warningdialogwindow.layout");
	m_window = warning_dialog;
	m_warning = "";
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("WarningDialogConfirmButton"));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&WarningDialogWindow ::onConfirmed, this));
	
	updateTranslation();
}

void WarningDialogWindow::update()
{
}

void WarningDialogWindow::updateTranslation()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;
	
	CEGUI::PushButton* btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow( "WarningDialogConfirmButton"));
	btn->setText((CEGUI::utf8*) gettext("Ok"));
	
	label = win_mgr.getWindow("WarningDialogLabel");
	label->setText((CEGUI::utf8*) gettext(m_warning.c_str()));
}


bool WarningDialogWindow::onConfirmed(const CEGUI::EventArgs& evt)
{
	m_document->hideWarning ();
	return true;
}

void WarningDialogWindow::setWarning(std::string warning)
{
	m_warning = warning;
	updateTranslation();
}



//
// ----------------------------------------- Error Dialog Window (a window containing a notification and a button) ----------------------------------------------
//


ErrorDialogWindow::ErrorDialogWindow (Document* doc)
:Window(doc)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::PushButton* btn;

	CEGUI::FrameWindow* error_dialog = (CEGUI::FrameWindow*) win_mgr.loadWindowLayout("errordialogwindow.layout");
	m_window = error_dialog;
	m_error = "Network connection timed out";
	
	btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow("ErrorDialogConfirmButton"));
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&ErrorDialogWindow ::onConfirmed, this));
	
	updateTranslation();
}

void ErrorDialogWindow::update()
{
}

void ErrorDialogWindow::updateTranslation()
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	CEGUI::Window* label;
	
	CEGUI::PushButton* btn = static_cast<CEGUI::PushButton*>(win_mgr.getWindow( "ErrorDialogConfirmButton"));
	btn->setText((CEGUI::utf8*) gettext("Ok"));
	
	label = win_mgr.getWindow("ErrorDialogLabel");
	label->setText((CEGUI::utf8*) gettext(m_error.c_str()));
}


bool ErrorDialogWindow::onConfirmed(const CEGUI::EventArgs& evt)
{
	m_document->onButtonErrorDialogConfirm();
	return true;
}

void ErrorDialogWindow::setError(std::string error)
{
	m_error = error;
	updateTranslation();
}

