#include "onlineservicesmanager.h"
#include "config.h"
#include "debug.h"
#include "tinyxml.h"

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/String.h>
#include <Poco/StringTokenizer.h>
#include <Poco/StreamCopier.h>
#include <Poco/Path.h>
#include <Poco/File.h>
#include <Poco/URI.h>
#include <Poco/Exception.h>
#include <Poco/Task.h>
#include <Poco/TaskManager.h>
#include <Poco/TaskNotification.h>
#include <Poco/Observer.h>

#include <string>
#include <iostream>
#include <fstream>

#include <OgreResourceGroupManager.h>

class LoginTask: public Poco::Task
{
public:
    std::string username;
    std::string pw;
    std::string sw_token;
    std::string dataPath;
    std::string userDataPath;
    std::vector<OnlineServicesManager::CharacterLight*> characters;

    LoginTask(const std::string& name, std::string username, std::string pw, std::string dataPath): Task(name)
    {
        sw_token = "";
        this->dataPath = dataPath;
        characters.clear();

        this->username = username;
        this->pw = pw;
    }

    void runTask()
    {
        sw_token = "";
        characters.clear();

        try
        {
            // prepare session
            //Poco::URI uri("http://localhost:8081/api/remote_login?username=" + this->username + "&password=" + this->pw);
			Poco::URI uri("http://sumwars-backend.appspot.com/api/remote_login?username=" + this->username + "&password=" + this->pw);
            Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());

            // prepare path
            std::string path(uri.getPathAndQuery());
            if (path.empty()) path = "/";

            // send request
            Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_GET, path, Poco::Net::HTTPMessage::HTTP_1_1);
            session.sendRequest(req);

            // get response
            Poco::Net::HTTPResponse res;
            std::cout << res.getStatus() << " " << res.getReason() << std::endl;


            // print response
            std::istream &is = session.receiveResponse(res);
            std::ostringstream stream;
            Poco::StreamCopier::copyStream(is, stream);

            std::string temp = stream.str();
            if(temp == "401")
            {
                std::cout << "Username or password is wrong" << std::endl;
                this->cancel();
            }
            else
            {



                TiXmlDocument doc;
                if(!doc.Parse(temp.c_str()))
                {
                    std::cerr << doc.ErrorDesc() << std::endl;
                }

                TiXmlElement* root = doc.FirstChildElement();
                if(root == NULL)
                {
                    std::cerr << "Failed to load file: No root element." << std::endl;
                    doc.Clear();
                }

                for(TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
                {
                    std::string val = elem->Value();
                    if(val == "token")
                        sw_token = elem->Attribute("value");
                    else if(val == "Character")
                    {
                        OnlineServicesManager::CharacterLight *cl = new OnlineServicesManager::CharacterLight();
                        elem->Attribute("level", &cl->level);
                        cl->subtype = elem->Attribute("subtype");
                        cl->name = elem->Attribute("name");
                        cl->savegame = Poco::replace(std::string(elem->GetText()), "\n", "");
                        characters.push_back(cl);

                        Poco::Path p;
                        p.parse(dataPath);
                        p.append("save/" + username);
                        userDataPath = p.toString();

                        Poco::File pf(p.toString());
                        pf.createDirectories();

                        std::ofstream f;
                        f.open(p.toString() + "/" + cl->name + ".sav");
                        f << cl->savegame;
                        f.close();
                    }
                }

                std::cout << "Login finished. Retrieved " << characters.size() << " character infos.";
            }
        }
        catch(Poco::Exception &ex)
        {
        }
    }
};

class LogoutTask: public Poco::Task
{
public:
    std::string sw_token;

    LogoutTask(const std::string& name, std::string token): Task(name)
    {
        sw_token = token;
    }

    void runTask()
    {

        try
        {
            // prepare session
            Poco::URI uri("http://localhost:8081/api/remote_logout?token=" + sw_token);
            Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());

            // prepare path
            std::string path(uri.getPathAndQuery());
            if (path.empty()) path = "/";

            // send request
            Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_GET, path, Poco::Net::HTTPMessage::HTTP_1_1);
            session.sendRequest(req);

            // get response
            Poco::Net::HTTPResponse res;
            std::cout << res.getStatus() << " " << res.getReason() << std::endl;

            // print response
            std::istream &is = session.receiveResponse(res);
            std::ostringstream stream;
            Poco::StreamCopier::copyStream(is, stream);

            std::string temp = Poco::replace(stream.str(), "{", "");
            std::cout << temp << std::endl;
            sw_token = "";
        }
        catch(Poco::Exception &ex)
        {
        }
    }
};

template<> OnlineServicesManager* Ogre::Singleton<OnlineServicesManager>::SUMWARS_OGRE_SINGLETON = 0;

OnlineServicesManager::OnlineServicesManager(std::string &dataPath)
{
    mTaskManager = new Poco::TaskManager();
    mTaskManager->addObserver(Poco::Observer<OnlineServicesManager, Poco::TaskProgressNotification>(*this, &OnlineServicesManager::onProgress));
    mTaskManager->addObserver(Poco::Observer<OnlineServicesManager, Poco::TaskFinishedNotification>(*this, &OnlineServicesManager::onFinished));
    mDataPath = dataPath;
    mUserLoggedIn = false;
    mUserDataPath = "";
    mUserDataResGroupId = "";
    mCurrentUsername = "Default";
    mToken = "";
}

void OnlineServicesManager::onProgress(Poco::TaskProgressNotification *notification)
{
    notification->release();
}

void OnlineServicesManager::onFinished(Poco::TaskFinishedNotification *notification)
{
    std::string tName = notification->task()->name();
    if(tName == "Login Task")
    {
        mToken = ((LoginTask*)notification->task())->sw_token;

        mUserDataPath = ((LoginTask*)notification->task())->userDataPath;
        mUserDataResGroupId = "Savegame_" + ((LoginTask*)notification->task())->username;
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mUserDataPath, "FileSystem", mUserDataResGroupId);
        Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(mUserDataResGroupId);
        mCurrentUsername = ((LoginTask*)notification->task())->username;
		mUserLoggedIn = true;

        std::vector<LoginStatusListener*>::iterator iter;
        for(iter = mLoginStatusListener.begin(); iter != mLoginStatusListener.end(); iter++)
            (*iter)->onLoginFinished(((LoginTask*)notification->task())->characters);

    }
    else if(tName == "Logout Task")
    {
        std::vector<LoginStatusListener*>::iterator iter;
        for(iter = mLoginStatusListener.begin(); iter != mLoginStatusListener.end(); iter++)
            (*iter)->onLogoutFinished();
        mUserLoggedIn = false;
    }
    notification->release();
}

bool OnlineServicesManager::login(std::string userName, std::string password)
{
    mTaskManager->start(new LoginTask("Login Task", userName, password, mDataPath));
    return true;
}

bool OnlineServicesManager::logout()
{
    mTaskManager->start(new LogoutTask("Logout Task", mToken));
    return true;
}

void OnlineServicesManager::registerLoginStatusListener(LoginStatusListener* l)
{
    std::vector<LoginStatusListener*>::iterator iter;
    for(iter = mLoginStatusListener.begin(); iter != mLoginStatusListener.end(); iter++)
        if(*iter == l)
            return;

    mLoginStatusListener.push_back(l);
}

void OnlineServicesManager::unregisterLoginStatusListener(LoginStatusListener* l)
{
    std::vector<LoginStatusListener*>::iterator iter;
    for(iter = mLoginStatusListener.begin(); iter != mLoginStatusListener.end(); iter++)
        if(*iter == l)
            mLoginStatusListener.erase(iter);
}

OnlineServicesManager* OnlineServicesManager::getSingletonPtr(void)
{
    return SUMWARS_OGRE_SINGLETON;
}

OnlineServicesManager& OnlineServicesManager::getSingleton(void)
{
    assert( SUMWARS_OGRE_SINGLETON );
    return ( *SUMWARS_OGRE_SINGLETON );
}
