#include "charinfo.h"




CharInfo::CharInfo (Document* doc)
	: Window(doc)
{
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();
	
	// Rahmen fuer CharInfo Fenster
	CEGUI::FrameWindow* char_info = (CEGUI::FrameWindow*) win_mgr.createWindow("TaharezLook/FrameWindow", "CharInfo");
	m_window = char_info;
	
	
	char_info->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim( 0.0f)));
	char_info->setSize(CEGUI::UVector2(cegui_reldim(0.48f), cegui_reldim( 0.77f)));
	char_info->setProperty("FrameEnabled","false");
	char_info->setProperty("TitlebarEnabled","false");
	char_info->setProperty("CloseButtonEnabled","false");
	char_info->subscribeEvent(CEGUI::Window::EventMouseButtonDown, CEGUI::Event::Subscriber(&Window::consumeEvent, (Window*) this));
	char_info->setWantsMultiClickEvents(false);
	char_info->setAlpha(0.7);

	// Bestandteile des Charakterfensters hinzufuegen
	CEGUI::PushButton* btn;
	CEGUI::Window* label;

	// Label Name
	label = win_mgr.createWindow("TaharezLook/StaticText", "NameLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim( 0.0f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.4f), cegui_reldim( 0.1f)));
	label->setText("Name");

	// Label Klasse
	label = win_mgr.createWindow("TaharezLook/StaticText", "ClassLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.4f), cegui_reldim( 0.0f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.4f), cegui_reldim( 0.1f)));
	label->setText("Klasse");

	// Label Level
	label = win_mgr.createWindow("TaharezLook/StaticText", "LevelLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.8f), cegui_reldim( 0.0f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.2f), cegui_reldim( 0.1f)));
	label->setText("Level 1");

	// Label Staerke
	label = win_mgr.createWindow("TaharezLook/StaticText", "StrengthLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.15f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	label->setText(gettext("main_power"));

	// Label Staerke (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "StrengthValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.15f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.06f)));
	label->setText("20");

	// Button Staerke erhoehen
	btn = static_cast<CEGUI::PushButton*>( win_mgr.createWindow("TaharezLook/Button", "StrengthAddButton"));
	char_info->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.40f), cegui_reldim( 0.15f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.06f)));
	btn->setText("+");
	btn->setID(CreatureBaseAttr::STRENGTH);
	btn->setWantsMultiClickEvents(false);
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CharInfo::onIncreaseAttributeButtonClicked, this));


	// Label Geschick
	label = win_mgr.createWindow("TaharezLook/StaticText", "DexterityLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.23f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	label->setText(gettext("main_dexterity"));

	// Label Geschick (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "DexterityValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.23f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.06f)));
	label->setText("20");

	// Button Geschick erhoehen
	btn = static_cast<CEGUI::PushButton*>( win_mgr.createWindow("TaharezLook/Button", "DexterityAddButton"));
	char_info->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.40f), cegui_reldim( 0.23f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.06f)));
	btn->setText("+");
	btn->setID(CreatureBaseAttr::DEXTERITY);
	btn->setWantsMultiClickEvents(false);
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CharInfo::onIncreaseAttributeButtonClicked, this));

	// Label Willenskraft
	label = win_mgr.createWindow("TaharezLook/StaticText", "WillpowerLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.31f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	label->setText(gettext("main_willpower"));

	// Label Willenskraft (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "WillpowerValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.31f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.06f)));
	label->setText("20");

	// Button Willenskraft erhoehen
	btn = static_cast<CEGUI::PushButton*>( win_mgr.createWindow("TaharezLook/Button", "WillpowerAddButton"));
	char_info->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.40f), cegui_reldim( 0.31f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.06f)));
	btn->setText("+");
	btn->setID(CreatureBaseAttr::WILLPOWER);
	btn->setWantsMultiClickEvents(false);
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CharInfo::onIncreaseAttributeButtonClicked, this));

	// Label Zauberkraft
	label = win_mgr.createWindow("TaharezLook/StaticText", "MagicpowerLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.39f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	label->setText(gettext("main_magic_power"));

	// Label Zauberkraft (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "MagicpowerValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.39f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.06f)));
	label->setText("20");

	// Button Zauberkraft erhoehen
	btn = static_cast<CEGUI::PushButton*>( win_mgr.createWindow("TaharezLook/Button", "MagicpowerAddButton"));
	char_info->addChildWindow(btn);
	btn->setPosition(CEGUI::UVector2(cegui_reldim(0.40f), cegui_reldim( 0.39f)));
	btn->setSize(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.06f)));
	btn->setText("+");
	btn->setID(CreatureBaseAttr::MAGIC_POWER);
	btn->setWantsMultiClickEvents(false);
	btn->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CharInfo::onIncreaseAttributeButtonClicked, this));

	// Label Attributpunkte
	label = win_mgr.createWindow("TaharezLook/StaticText", "AttrPointsLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.47f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	label->setText(gettext("main_credits"));

	// Label  Attributpunkte (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "AttrPointsValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.47f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.1f), cegui_reldim( 0.06f)));
	label->setText("5");

	// Label Exp
	label = win_mgr.createWindow("TaharezLook/StaticText", "ExperienceLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.50f), cegui_reldim( 0.15f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	label->setText(gettext("main_experience"));

	// Label Exp (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "ExperienceValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.70f), cegui_reldim( 0.15f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.06f)));
	label->setText("0/100");

	// Label HP
	label = win_mgr.createWindow("TaharezLook/StaticText", "HitpointsLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.50f), cegui_reldim( 0.23f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	label->setText(gettext("main_healthpoints"));

	// Label HP (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "HitpointsValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.70f), cegui_reldim( 0.23f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.06f)));
	label->setText("0/100");

	// Label Attacke
	label = win_mgr.createWindow("TaharezLook/StaticText", "AttackLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.55f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	label->setText(gettext("main_attack"));

	// Label Attacke (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "AttackValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.55f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.10f), cegui_reldim( 0.06f)));
	label->setText("20");

	// Label Attackegeschwindigkeit
	label = win_mgr.createWindow("TaharezLook/StaticText", "AttackSpeedLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.62f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	label->setText(gettext("main_attacks_per_second"));

	// Label Attacke (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "AttackSpeedValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.62f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.10f), cegui_reldim( 0.06f)));
	label->setText("20");

	// Label Reichweite
	label = win_mgr.createWindow("TaharezLook/StaticText", "RangeLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.69f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	label->setText(gettext("main_range"));

	// Label Reichweite (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "RangeValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.69f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.10f), cegui_reldim( 0.06f)));
	label->setText("2");

	// Label Durchschlagskraft
	label = win_mgr.createWindow("TaharezLook/StaticText", "PowerLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.76f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	label->setText(gettext("main_penetration_power"));

	// Label Durchschlagskraft
	label = win_mgr.createWindow("TaharezLook/StaticText", "PowerValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.76f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.10f), cegui_reldim( 0.06f)));
	label->setText("30");

	// Label Ruestung
	label = win_mgr.createWindow("TaharezLook/StaticText", "ArmorLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.83f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	label->setText(gettext("main_armour"));

	// Label Ruestung
	label = win_mgr.createWindow("TaharezLook/StaticText", "ArmorValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.83f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.10f), cegui_reldim( 0.06f)));
	label->setText("40");

	// Label Block
	label = win_mgr.createWindow("TaharezLook/StaticText", "BlockLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim( 0.90f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	label->setText(gettext("main_block"));

	// Label Block
	label = win_mgr.createWindow("TaharezLook/StaticText", "BlockValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.90f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.10f), cegui_reldim( 0.06f)));
	label->setText("20");

	// Label Basisschaden
	label = win_mgr.createWindow("TaharezLook/StaticText", "BaseDmgLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.50f), cegui_reldim( 0.48f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	label->setText(gettext("main_base_damage"));

	// Label Basisschaden (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "BaseDmgValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.70f), cegui_reldim( 0.48f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.06f)));
	label->setText("5-10");

	// Label Skill1 Schaden
	label = win_mgr.createWindow("TaharezLook/StaticText", "Skill1DmgLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.50f), cegui_reldim( 0.55f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	label->setText("Skill1");

	// Label Skill1 Schaden (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "Skill1DmgValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.70f), cegui_reldim( 0.55f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.06f)));
	label->setText("10-20");

	// Label Skill2 Schaden
	label = win_mgr.createWindow("TaharezLook/StaticText", "Skill2DmgLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.50f), cegui_reldim( 0.62f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	label->setText("Skill2");

	// Label Skill2 Schaden (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "Skill2DmgValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.70f), cegui_reldim( 0.62f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.06f)));
	label->setText("20-30");

	// Label physisches Resistenz
	label = win_mgr.createWindow("TaharezLook/StaticText", "ResistPhysLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.50f), cegui_reldim( 0.69f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	label->setText(gettext("main_physical_resistance"));

	// Label physische Resistenz (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "ResistPhysValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.70f), cegui_reldim( 0.69f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.06f)));
	label->setText("0");

	// Label Feuerresistenz
	label = win_mgr.createWindow("TaharezLook/StaticText", "ResistFireLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.50f), cegui_reldim( 0.76f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	label->setText(gettext("main_fire_resistance"));

	// Label Feuerresistenz (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "ResistFireValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.70f), cegui_reldim( 0.76f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.06f)));
	label->setText("0");

	// Label Luftresistenz
	label = win_mgr.createWindow("TaharezLook/StaticText", "ResistAirLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.50f), cegui_reldim( 0.83f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	label->setText(gettext("main_air_resistance"));

	// Label Feuerresistenz (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "ResistAirValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.70f), cegui_reldim( 0.83f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.06f)));
	label->setText("0");

	// Label Eisresistenz
	label = win_mgr.createWindow("TaharezLook/StaticText", "ResistIceLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.50f), cegui_reldim( 0.90f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.20f), cegui_reldim( 0.06f)));
	label->setText(gettext("main_ice_resistance"));

	// Label Eisresistenz (Wert)
	label = win_mgr.createWindow("TaharezLook/StaticText", "ResistIceValueLabel");
	char_info->addChildWindow(label);
	label->setProperty("FrameEnabled", "true");
	label->setProperty("BackgroundEnabled", "true");
	label->setPosition(CEGUI::UVector2(cegui_reldim(0.70f), cegui_reldim( 0.90f)));
	label->setSize(CEGUI::UVector2(cegui_reldim(0.25f), cegui_reldim( 0.06f)));
	label->setText("0");
}

 void CharInfo::update()
{
	// Spielerobjekt
	Player* player = m_document->getLocalPlayer();

	std::string tooltip;

	// Fenstermanager
	CEGUI::WindowManager& win_mgr = CEGUI::WindowManager::getSingleton();

	CEGUI::PushButton* btn;
	CEGUI::Window* label;
	std::ostringstream out_stream;

	// Label Name
	label =  win_mgr.getWindow("NameLabel");
	out_stream.str("");
	out_stream.str(player->getName());
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Klasse
	label =  win_mgr.getWindow("ClassLabel");
	out_stream.str("");
	if (player->getTypeInfo()->m_subtype == "warrior")
	{
		out_stream.str(gettext("main_warrior"));
	}
	else if (player->getTypeInfo()->m_subtype == "mage")
	{
		out_stream.str(gettext("main_magician"));
	}
	else if (player->getTypeInfo()->m_subtype == "archer")
	{
		out_stream.str(gettext("main_archer"));
	}
	else if (player->getTypeInfo()->m_subtype == "priest")
	{
		out_stream.str(gettext("main_priest"));
	}
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Level
	label =  win_mgr.getWindow("LevelLabel");
	out_stream.str("");
	out_stream << "Level "<<(int) player->getBaseAttr()->m_level;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Staerke
	label =  win_mgr.getWindow("StrengthValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_strength;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Zauberkraft
	label =  win_mgr.getWindow("MagicpowerValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_magic_power;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Willenskraft
	label =  win_mgr.getWindow("WillpowerValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_willpower;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Geschick
	label =  win_mgr.getWindow("DexterityValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_dexterity;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Buttons zum erhoehen von Attributen sind nur sichtbar wenn Attributpunkt zu verteilen sind
	bool add_but_vis = false;
	if ( player->getAttributePoints()>0)
	{
		add_but_vis=true;
	}

	// Sichtbarkeit der Buttons setzen
	btn =   static_cast<CEGUI::PushButton*>(win_mgr.getWindow("StrengthAddButton"));
	btn->setVisible(add_but_vis);
	btn =  static_cast<CEGUI::PushButton*>(win_mgr.getWindow("DexterityAddButton"));
	btn->setVisible(add_but_vis);
	btn =  static_cast<CEGUI::PushButton*>(win_mgr.getWindow("WillpowerAddButton"));
	btn->setVisible(add_but_vis);
	btn =  static_cast<CEGUI::PushButton*>(win_mgr.getWindow("MagicpowerAddButton"));
	btn->setVisible(add_but_vis);

	// Label freie Attributspunkte
	label =  win_mgr.getWindow("AttrPointsValueLabel");
	out_stream.str("");
	out_stream << player->getAttributePoints();
	//label->setVisible(add_but_vis);
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Ruestung
	label =  win_mgr.getWindow("ArmorValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_armor;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Attacke
	label =  win_mgr.getWindow("AttackValueLabel");
	out_stream.str("");
	out_stream << player->getBaseDamage().m_attack;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Block
	label =  win_mgr.getWindow("BlockValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_block;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label HP
	label =  win_mgr.getWindow("HitpointsValueLabel");
	out_stream.str("");
	out_stream << (int) player->getDynAttr()->m_health <<"/" <<(int) player->getBaseAttrMod()->m_max_health;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Exp
	label =  win_mgr.getWindow("ExperienceValueLabel");
	out_stream.str("");
	out_stream << (int) player->getDynAttr()->m_experience <<"/" <<(int) player->getBaseAttr()->m_max_experience;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Phys Resistenz
	label =  win_mgr.getWindow("ResistPhysValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_resistances[Damage::PHYSICAL];
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Feuer Resistenz
	label =  win_mgr.getWindow("ResistFireValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_resistances[Damage::FIRE];
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Eis Resistenz
	label =  win_mgr.getWindow("ResistIceValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_resistances[Damage::ICE];
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Luft Resistenz
	label =  win_mgr.getWindow("ResistAirValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_resistances[Damage::AIR];
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Angriffsgeschwindigkeit
	label =  win_mgr.getWindow("AttackSpeedValueLabel");
	out_stream.str("");
	out_stream << 0.001*std::min((short) 5000, player->getBaseAttrMod()->m_attack_speed);
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Reichweite
	label =  win_mgr.getWindow("RangeValueLabel");
	out_stream.str("");
	out_stream << player->getBaseAttrMod()->m_attack_range;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Durchschlagskraft
	label =  win_mgr.getWindow("PowerValueLabel");
	out_stream.str("");
	out_stream << player->getBaseDamage().m_power;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Schaden Basisattacke
	float minb=player->getBaseDamage().getSumMinDamage();
	float maxb=player->getBaseDamage().getSumMaxDamage();
	// Schaden Attacke links
	float minl=player->getLeftDamage().getSumMinDamage();
	float maxl=player->getLeftDamage().getSumMaxDamage();
	// Schaden Attacke rechts
	float minr=player->getRightDamage().getSumMinDamage();
	float maxr=player->getRightDamage().getSumMaxDamage();



	// Label Schaden Basisattacke
	label =  win_mgr.getWindow( "BaseDmgLabel");
	out_stream.str("");
	if (player->getTypeInfo()->m_subtype == "mage")
		out_stream << Action::getName(Action::MAGIC_ATTACK);
	if (player->getTypeInfo()->m_subtype == "archer")
		out_stream << Action::getName(Action::RANGE_ATTACK);
	if (player->getTypeInfo()->m_subtype == "warrior")
		out_stream << Action::getName(Action::ATTACK);
	if (player->getTypeInfo()->m_subtype == "priest")
		out_stream << Action::getName(Action::HOLY_ATTACK);

	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}

	// Label Basisschaden
	label =  win_mgr.getWindow("BaseDmgValueLabel");
	out_stream.str("");
	out_stream << (int) minb << "-" << (int) maxb;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}
	tooltip = player->getBaseDamage().getDamageString();
	if (tooltip != label->getTooltipText())
	{
		label->setTooltipText(tooltip);
	}

	// Label Attacke links
	label =  win_mgr.getWindow( "Skill1DmgLabel");
	out_stream.str("");
	out_stream << Action::getName(player->getLeftAction());
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());

	}

	// Label Schaden Attacke links
	label =  win_mgr.getWindow( "Skill1DmgValueLabel");
	out_stream.str("");
	out_stream << (int) minl << "-" << (int) maxl;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}
	tooltip = player->getLeftDamage().getDamageString();
	if (tooltip != label->getTooltipText())
	{
		label->setTooltipText(tooltip);
	}



	// Label Attacke rechts
	label =  win_mgr.getWindow( "Skill2DmgLabel");
	out_stream.str("");
	out_stream << Action::getName(player->getRightAction());
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());

	}

	// Label Schaden Attacke rechts
	label =  win_mgr.getWindow( "Skill2DmgValueLabel");
	out_stream.str("");
	out_stream << (int) minr << "-" << (int) maxr;
	if (label->getText()!=out_stream.str())
	{
		label->setText(out_stream.str());
	}
	tooltip = player->getRightDamage().getDamageString();
	if (tooltip != label->getTooltipText())
	{
		label->setTooltipText(tooltip);
	}
}

bool CharInfo::onIncreaseAttributeButtonClicked(const CEGUI::EventArgs& evt)
{
	const CEGUI::MouseEventArgs& we =
			static_cast<const CEGUI::MouseEventArgs&>(evt);
	unsigned int id = we.window->getID();
	DEBUG("left button pressed on skill %i",id);
	m_document->increaseAttribute((CreatureBaseAttr::Attribute) id);
	return true;
}




