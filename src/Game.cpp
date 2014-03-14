#include "Game.hpp"

Game::Game(std::string nickname, std::string address, std::string port):
	mOgreRoot(0),
	mOgreResourcesCfg(Ogre::StringUtil::BLANK),
	mOgrePluginsCfg(Ogre::StringUtil::BLANK),
	mSceneMgr(0),
	mCameraManager(0),
	mGameWindow(0),
	mInput(0),
	mNickname(nickname),
	mLocalPlayer(0),
	mPlayerList(new OgrePlayerList()),
	mBombManager(0),
	mExplosionManager(0),
	mLocalMap(0),
	mAddress(address),
	mPort(port),
	mNMFactory(new NetworkMessage::NetworkMessageFactory()),
	mGCListener(0),
	mOnlineMode(false),
	mServerJoined(false),
	mGameSetUp(false),
	mSceneCreated(false),
	mGameRunning(false),
	mShutDownFlag(false),
	cd(0.5)
{
	if(mAddress != "")
		mOnlineMode = true;
}

Game::~Game(){

	Ogre::LogManager::getSingletonPtr()->logMessage("Deleting Client Listener");
	delete mGCListener;
	Ogre::LogManager::getSingletonPtr()->logMessage("Deleting Inputs");
	delete mInput;
	Ogre::LogManager::getSingletonPtr()->logMessage("Deleting Camera Manager");
	delete mCameraManager;
	Ogre::LogManager::getSingletonPtr()->logMessage("Deleting Local Map");
	delete mLocalMap;
	Ogre::LogManager::getSingletonPtr()->logMessage("Deleting Local Player");
	delete mLocalPlayer;
	Ogre::LogManager::getSingletonPtr()->logMessage("Deleting Game Window");
	delete mGameWindow;
	Ogre::LogManager::getSingletonPtr()->logMessage("Deleting OGRE Root");
	delete mOgreRoot;

}

void Game::go(){

#ifdef _DEBUG
	mOgreResourcesCfg = "resources_d.cfg";
	mOgrePluginsCfg = "plugins_d.cfg";
#else
	mOgreResourcesCfg = "resources.cfg";
	mOgrePluginsCfg = "plugins.cfg";
#endif

	if (!setup())
		return;

	mOgreRoot->startRendering();

	// clean up
	destroyScene();

}

void Game::shutDown(){
	mShutDownFlag = true;
}

bool Game::injectMouseMove(const OIS::MouseEvent &arg){

	if(mGameSetUp)
		return mLocalPlayer->injectMouseMove(arg);

	return true;

}

bool Game::injectMouseDown(
	const OIS::MouseEvent &arg, OIS::MouseButtonID id
){

	if(mGameSetUp)
		return mLocalPlayer->injectMouseDown(arg, id);

	return true;

}

bool Game::injectMouseUp(
	const OIS::MouseEvent &arg, OIS::MouseButtonID id
){

	if(mGameSetUp)
		return mLocalPlayer->injectMouseUp(arg, id);

	return true;

}

bool Game::injectKeyDown(const OIS::KeyEvent &arg){

	if(arg.key == OIS::KC_ESCAPE)
		mShutDownFlag = true;
	if(mOnlineMode && arg.key == OIS::KC_R)
		mGCListener->sendMessage(
			mNMFactory->buildMessage(NetworkMessage::GAMESTART)
		);

	mGameWindow->injectKeyDown(arg);

	if(mGameSetUp)
		return mLocalPlayer->injectKeyDown(arg);

	return true;

}

bool Game::injectKeyUp(const OIS::KeyEvent &arg){

	if(mGameSetUp)
		return mLocalPlayer->injectKeyUp(arg);

	return true;

}

bool Game::injectHeadMove(const Ogre::Vector3 &evt){

	if(mGameSetUp)
		return mLocalPlayer->injectHeadMove(evt);

	return true;

}

void Game::injectClientClose(){

	if(!mGameSetUp){
		if(mServerJoined){
			Ogre::LogManager::getSingletonPtr()->logMessage("Lost connection to the server, shutting down");
			shutDown();
		}
		else{
			Ogre::LogManager::getSingletonPtr()->logMessage("Error while joining server, setting up the game as single player");
			offlineSetup();
		}
	}

}

void Game::injectJoinAccept(NetworkMessage::JoinAccept *message){

	mServerJoined = true;

	PlayerList *pl = message->getPlayerList();
	OgrePlayer *tmp;

	mExplosionManager = new ExplosionManager(mWorld);
	mBombManager = new BombManager(mWorld, mExplosionManager);

	Ogre::LogManager::getSingletonPtr()->logMessage("Creating Local Map");
	mLocalMap = new LocalMap(
		mWorld,
		mPlayerList,
		mBombManager,
		message->getMapHeight(),
		message->getMapWidth(),
		message->getSeed()
	);

	Ogre::LogManager::getSingletonPtr()->logMessage("Filling player list");
	for(unsigned int i = 0; i < pl->size(); i++){
		if((*pl)[i]->getNickname() == mNickname){
			Ogre::LogManager::getSingletonPtr()->logMessage("Creating Local Player");
			mLocalPlayer = new LocalPlayer(mNickname, mWorld, mCameraManager);
			tmp = mLocalPlayer;
		}
		else
			tmp = new RemotePlayer((*pl)[i]->getNickname(), mWorld);

		tmp->setNodePositionX((*pl)[i]->getNodePositionX());
		tmp->setNodePositionY((*pl)[i]->getNodePositionY());
		tmp->setNodePositionZ((*pl)[i]->getNodePositionZ());

		mPlayerList->addPlayer(tmp);

	}
	
	mLocalPlayer->setPlayerEventListener(this);

	mGameSetUp = true;

}

void Game::injectJoinRefuse(NetworkMessage::JoinRefuse *message){
	shutDown();
}

void Game::injectPlayerJoined(NetworkMessage::PlayerJoined *message){

	if(!mGameRunning){

		Ogre::LogManager::getSingletonPtr()->logMessage("Adding new player");
		RemotePlayer *rp = new RemotePlayer(message->getNickname(), mWorld);
		rp->setNodePositionX(message->getPositionX());
		rp->setNodePositionY(message->getPositionY());
		rp->setNodePositionZ(message->getPositionZ());
		mPlayerList->addPlayer(rp);

	}

}

void Game::injectPlayerLeft(NetworkMessage::PlayerLeft *message){

	Ogre::LogManager::getSingletonPtr()->logMessage("Removing player");
	mPlayerList->removePlayer(
		mPlayerList->getPlayerByName(message->getNickname())
	);

}

void Game::injectGameStart(NetworkMessage::GameStart *message){
	Ogre::LogManager::getSingletonPtr()->logMessage("Starting game");
	mGameRunning = true;
}

void Game::injectGameEnd(NetworkMessage::GameEnd *message){
	Ogre::LogManager::getSingletonPtr()->logMessage("Ending game");
	mGameRunning = false;
}

void Game::injectPlayerInput(NetworkMessage::PlayerInput *message){

	std::string nickname = message->getNickname();

	if(nickname != mNickname)
		mPlayerList->getPlayerByName(nickname)->injectPlayerInput(message);

}

void Game::injectPlayerKilled(NetworkMessage::PlayerKilled *message){

	std::string nickname = message->getNickname();

	if(nickname != mNickname)
		mPlayerList->getPlayerByName(nickname)->die();

}

bool  Game::playerInput(){

	mGCListener->sendMessage(
		mNMFactory->buildMessage(NetworkMessage::PLAYERINPUT, mLocalPlayer)
	);

	return true;

}

bool Game::playerDied(){

	mGCListener->sendMessage(
		mNMFactory->buildMessage(NetworkMessage::PLAYERKILLED, mNickname)
	);

	return true;

}

bool Game::setup(){

	mOgreRoot = new Ogre::Root(mOgrePluginsCfg);

	setupResources();

	chooseSceneManager();

	if(!configure())
		return false;

	// Create any resource listeners (for loading screens)
	createResourceListener();
	// Load resources
	loadResources();
	mGameWindow->setViewMode("default");

	// Create the frame listener
	createFrameListener();

	return true;

}

void Game::chooseSceneManager(){
	// Get the SceneManager, in this case a generic one
	mSceneMgr = mOgreRoot->createSceneManager(Ogre::ST_GENERIC);
}

bool Game::configure(){

	// Show the configuration dialog and initialise the system
	// You can skip this and use root.restoreConfig() to load configuration
	// settings if you were sure there are valid ones saved in ogre.cfg
	if(mOgreRoot->showConfigDialog()){

		Ogre::LogManager::getSingletonPtr()->logMessage("Creating Camera Manager");
		mCameraManager = new CameraManager(mSceneMgr);

		Ogre::LogManager::getSingletonPtr()->logMessage("Creating Game Window");
		mGameWindow = new GameWindow(
			mCameraManager,
			mOgreRoot->initialise(true, "Game Render Window")
		);

		bulletSetup();

		if(mOnlineMode)
			networkSetup();
		else
			offlineSetup();

		return true;

	}
	else
		return false;

}

bool Game::networkSetup(){

	Ogre::LogManager::getSingletonPtr()->logMessage("Starting Game Client");
	mGCListener = new GameClient::Listener(mAddress, mPort);
	createNetworkListener();
	mGCListener->start();

	mGCListener->sendMessage(mNMFactory->buildMessage(NetworkMessage::JOIN, mNickname));

	return true;

}

bool Game::offlineSetup(){

	mExplosionManager = new ExplosionManager(mWorld);
	mBombManager = new BombManager(mWorld, mExplosionManager);

	Ogre::LogManager::getSingletonPtr()->logMessage("Generating Local Map");
	mLocalMap = new LocalMap(mWorld, mPlayerList, mBombManager, 15, 15);

	Ogre::LogManager::getSingletonPtr()->logMessage("Creating Local Player");
	mLocalPlayer = new LocalPlayer(mNickname, mWorld, mCameraManager);
	mPlayerList->addPlayer(mLocalPlayer);
	mLocalPlayer->setPlayerEventListener(this);

	mLocalMap->setStartingPosition(0, mLocalPlayer);

	mOnlineMode = false;
	mGameSetUp = true;
	mGameRunning = true;

	return true;

}

bool Game::bulletSetup(){

	AxisAlignedBox bounds(
		Ogre::Vector3 (-10000, -10000, -10000),
		Ogre::Vector3 (10000,  10000,  10000)
	);
	Vector3 gravityVector(0,-9.81 * 2,0);

	mWorld = new OgreBulletDynamics::DynamicsWorld(mSceneMgr, bounds, gravityVector);
	debugDrawer = new OgreBulletCollisions::DebugDrawer();
	debugDrawer->setDrawWireframe(true);	// we want to see the Bullet containers

	mWorld->setDebugDrawer(debugDrawer);
	//mWorld->setShowDebugShapes(true);		// enable it if you want to see the Bullet containers
	SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("debugDrawer", Ogre::Vector3::ZERO);
	node->attachObject(static_cast <SimpleRenderable *> (debugDrawer));

	return true;

}

void Game::createNetworkListener(){

	mGCListener->setCallbackClose(boost::bind(&Game::injectClientClose, this));
	mGCListener->setCallbackJoinAccept(
		boost::bind(&Game::injectJoinAccept, this, _1)
	);
	mGCListener->setCallbackJoinRefuse(
		boost::bind(&Game::injectJoinRefuse, this, _1)
	);
	mGCListener->setCallbackPlayerJoined(
		boost::bind(&Game::injectPlayerJoined, this, _1)
	);
	mGCListener->setCallbackPlayerLeft(
		boost::bind(&Game::injectPlayerLeft, this, _1)
	);
	mGCListener->setCallbackGameStart(
		boost::bind(&Game::injectGameStart, this, _1)
	);
	mGCListener->setCallbackGameEnd(
		boost::bind(&Game::injectGameEnd, this, _1)
	);
	mGCListener->setCallbackPlayerInput(
		boost::bind(&Game::injectPlayerInput, this, _1)
	);
	mGCListener->setCallbackPlayerKilled(
		boost::bind(&Game::injectPlayerKilled, this, _1)
	);

}

void Game::createFrameListener(){

	Ogre::LogManager::getSingletonPtr()->logMessage("Creating Input");
	mInput = new Input(mGameWindow->getWindow());

	mInput->setMouseListener(
		boost::bind(&Game::injectMouseMove, this, _1),
		boost::bind(&Game::injectMouseDown, this, _1, _2),
		boost::bind(&Game::injectMouseUp, this, _1, _2)
	);

	mInput->setKeyboardListener(
		boost::bind(&Game::injectKeyDown, this, _1),
		boost::bind(&Game::injectKeyUp, this, _1)
	);

	mInput->setSensorFusionListener(
		boost::bind(&Game::injectHeadMove, this, _1)
	);

	mOgreRoot->addFrameListener(this);

}

void Game::createScene(){

	Animation::setDefaultInterpolationMode(Animation::IM_LINEAR);
	Animation::setDefaultRotationInterpolationMode(Animation::RIM_LINEAR);

	Ogre::LogManager::getSingletonPtr()->logMessage("Generating Player Graphics");

	for(unsigned int i = 0; i < mPlayerList->size(); i++)
		(*mPlayerList)[i]->generateGraphics();

	mLocalPlayer->lookAt(mLocalMap->getMapCenter());

	Ogre::LogManager::getSingletonPtr()->logMessage("Generating Local Map");
	mLocalMap->generate();

	// Lights
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	mSceneCreated = true;

}

void Game::destroyScene(){}

void Game::setupResources(){

	// Load resource paths from config file
	Ogre::ConfigFile cf;
	cf.load(mOgreResourcesCfg);

	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;

	while (seci.hasMoreElements()){

		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;

		for (i = settings->begin(); i != settings->end(); ++i){

			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);

		}

	}

}

void Game::createResourceListener(){}

void Game::loadResources(){
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

bool Game::frameRenderingQueued(const Ogre::FrameEvent &evt){

	if(mShutDownFlag)
		return false;

	if(mOnlineMode && mGCListener->isClosed())
		return false;

	mInput->capture();

	if(mGameRunning){

		if(!mSceneCreated){

			createScene();

			mBombManager->add("Target6", Ogre::Vector3(10,1,10));
			mBombManager->add("Target6", Ogre::Vector3(11,1,10));
			mBombManager->add("Target6", Ogre::Vector3(9,1,10));

			mPlayerList->addPlayer (new RemotePlayer("Zykino", mWorld));
			mLocalMap->setStartingPosition(2, (*mPlayerList)[1]);
			(*mPlayerList)[1]->generateGraphics();

		}

		for(unsigned int i = 0; i < mPlayerList->size(); i++)
			(*mPlayerList)[i]->frameRenderingQueued(evt);

		cd -= evt.timeSinceLastFrame;

		if(cd < 0){
			mBombManager->add("Target6", Ogre::Vector3(10,5,10));
			cd = 0.5;
		}

		mBombManager->frameRenderingQueued();
		mExplosionManager->frameRenderingQueued(evt);

		mWorld->stepSimulation(evt.timeSinceLastFrame);
	}

	return true;

}
