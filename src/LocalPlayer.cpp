#include "LocalPlayer.hpp"

LocalPlayer::LocalPlayer(
	std::string name,
	Ogre::SceneManager *sceneMgr,
	OgreBulletDynamics::DynamicsWorld *world,
	CameraManager *cameraManager
):
	Player(name),
	mSceneMgr(sceneMgr),
	mWorld(world),
	mBody(0),
	mCameraManager(cameraManager),
	mAccelForward(0),
	mAccelBack(0),
	mAccelLeft(0),
	mAccelRight(0),
	mAccelUp(0),
	mAccelDown(0),
	mGraphicsSetUp(false),
	mHadInputUseful(false),
	mYawCorrection(0),
	mPitchCorrection(0),
	mRollCorrection(0),
	mPositionCorrection(Ogre::Vector3::ZERO),
	mOrientationCorrection(Ogre::Quaternion::ZERO)
{
	mTopSpeed = 100;
	mNodePositionX = 100;
	mNodePositionY = 100;
	mNodePositionZ = 100;
}

LocalPlayer::~LocalPlayer(){
}

void LocalPlayer::generateGraphics(){

	using namespace OgreBulletCollisions;
	using namespace Ogre;

	Entity *entity = mSceneMgr->createEntity("PlayerMesh", "bomberman.mesh");
	entity->setCastShadows(true);
	AxisAlignedBox boundingB = entity->getBoundingBox();
	Vector3 size = boundingB.getSize();
	size /= 2;
	SceneNode *bodyNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	SceneNode *entityNode = bodyNode->createChildSceneNode("entity");
	entityNode->attachObject(entity);
	entityNode->setPosition(0, -3, 0);
	//entityNode->roll(Degree(90));
	BoxCollisionShape *boxShape = new BoxCollisionShape(size);
	mBody = new OgreBulletDynamics::RigidBody("PlayerHitbox", mWorld);

	Vector3 position(
		mNodePositionX,
		mNodePositionY,
		mNodePositionZ
	);

	mBody->setShape(
		bodyNode,
		boxShape,
		0.0f,
		0.0f,
		50.0f,
		position
	);

	mBody->disableDeactivation();
	mGraphicsSetUp = true;

}

bool LocalPlayer::injectMouseMove(const OIS::MouseEvent &arg){

	mNodeYaw += mYawCorrection.valueDegrees();
	mNodePitch += mPitchCorrection.valueDegrees();
	mNodeRoll += mRollCorrection.valueDegrees();

	mNodeYaw += (-arg.state.X.rel * 0.15f);
	mNodePitch += (-arg.state.Y.rel * 0.15f);
	//mNodeRoll = Ogre::Degree(-arg.state.Z.rel * 0.15f);

	if(mGraphicsSetUp){

		//mBody->getSceneNode()->getChild("entity")->yaw(
			//mYawCorrection + Ogre::Degree(-arg.state.X.rel * 0.15f)
		//);
		//mBody->getSceneNode()->getChild("entity")->pitch(
			//mPitchCorrection + Ogre::Degree(-arg.state.Y.rel * 0.15f)
		//);
		//mBody->getSceneNode()->getChild("entity")->roll(mRollCorrection);

		mBody->getBulletRigidBody()->proceedToTransform(
			btTransform(
				btQuaternion(Ogre::Degree(mNodeYaw + 180).valueRadians(), 0, 0),
				btVector3(mNodePositionX, mNodePositionY, mNodePositionZ)
			)
		);
		//mBody->getBulletRigidBody()->proceedToTransform(
			//btTransform(
				//btQuaternion(mNodeYaw, mNodePitch, mNodeRoll),
				//btVector3(mNodePositionX, mNodePositionY, mNodePositionZ)
			//)
		//);
		//mBody->getBulletRigidBody()->yaw(
			//mYawCorrection + Ogre::Degree(-arg.state.X.rel * 0.15f)
		//);
		//mBody->getBulletRigidBody()->pitch(
			//mPitchCorrection + Ogre::Degree(-arg.state.Y.rel * 0.15f)
		//);
		//mBody->getBulletRigidBody()->roll(mRollCorrection);

	}

	if(mCameraManager){

		mCameraManager->yaw(
			mYawCorrection + Ogre::Degree(-arg.state.X.rel * 0.15f)
		);
		mCameraManager->pitch(
			mPitchCorrection + Ogre::Degree(-arg.state.Y.rel * 0.15f)
		);
		mCameraManager->roll(mRollCorrection);

	}

	mYawCorrection = 0;
	mPitchCorrection = 0;
	mRollCorrection = 0;

	mHadInputUseful = true;

	return true;

}

bool LocalPlayer::injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
	return true;
}

bool LocalPlayer::injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id){
	return true;
}

bool LocalPlayer::injectKeyDown(const OIS::KeyEvent &arg){

	if (arg.key == OIS::KC_Z || arg.key == OIS::KC_UP)
		mGoingForward = true;
	else if (arg.key == OIS::KC_S || arg.key == OIS::KC_DOWN)
		mGoingBack = true;
	else if (arg.key == OIS::KC_Q || arg.key == OIS::KC_LEFT)
		mGoingLeft = true;
	else if (arg.key == OIS::KC_D || arg.key == OIS::KC_RIGHT)
		mGoingRight = true;
	else if (arg.key == OIS::KC_PGUP)
		mGoingUp = true;
	else if (arg.key == OIS::KC_PGDOWN)
		mGoingDown = true;
	else if (arg.key == OIS::KC_LSHIFT)
		mFastMove = true;

	if(
		arg.key == OIS::KC_Z ||
		arg.key == OIS::KC_UP ||
		arg.key == OIS::KC_S ||
		arg.key == OIS::KC_DOWN ||
		arg.key == OIS::KC_Q ||
		arg.key == OIS::KC_LEFT ||
		arg.key == OIS::KC_D ||
		arg.key == OIS::KC_RIGHT ||
		arg.key == OIS::KC_PGUP ||
		arg.key == OIS::KC_PGDOWN
	)
		mHadInputUseful = true;

	return true;

}

bool LocalPlayer::injectKeyUp(const OIS::KeyEvent &arg){

	if (arg.key == OIS::KC_Z || arg.key == OIS::KC_UP)
		mGoingForward = false;
	else if (arg.key == OIS::KC_S || arg.key == OIS::KC_DOWN)
		mGoingBack = false;
	else if (arg.key == OIS::KC_Q || arg.key == OIS::KC_LEFT)
		mGoingLeft = false;
	else if (arg.key == OIS::KC_D || arg.key == OIS::KC_RIGHT)
		mGoingRight = false;
	else if (arg.key == OIS::KC_PGUP)
		mGoingUp = false;
	else if (arg.key == OIS::KC_PGDOWN)
		mGoingDown = false;
	else if (arg.key == OIS::KC_LSHIFT)
		mFastMove = false;

	if(
		arg.key == OIS::KC_Z ||
		arg.key == OIS::KC_UP ||
		arg.key == OIS::KC_S ||
		arg.key == OIS::KC_DOWN ||
		arg.key == OIS::KC_Q ||
		arg.key == OIS::KC_LEFT ||
		arg.key == OIS::KC_D ||
		arg.key == OIS::KC_RIGHT ||
		arg.key == OIS::KC_PGUP ||
		arg.key == OIS::KC_PGDOWN
	)
		mHadInputUseful = true;

	return true;

}

void LocalPlayer::injectPlayerInput(NetworkMessage::PlayerInput *message){

	if(mCameraManager){

		mYawCorrection += Ogre::Degree(message->getNodeYaw() - mNodeYaw);
		mPitchCorrection += Ogre::Degree(message->getNodePitch() - mNodePitch);
		mRollCorrection += Ogre::Degree(message->getNodeRoll() - mNodeRoll);

	}

	mPositionCorrection.x += message->getNodePositionX() - mNodePositionX;
	mPositionCorrection.y += message->getNodePositionY() - mNodePositionY;
	mPositionCorrection.z += message->getNodePositionZ() - mNodePositionZ;

	mGoingForward = message->getGoingForward();
	mGoingBack = message->getGoingBack();
	mGoingLeft = message->getGoingLeft();
	mGoingRight = message->getGoingRight();
	mGoingUp = message->getGoingUp();
	mGoingDown = message->getGoingDown();

}

bool LocalPlayer::frameRenderingQueued(const Ogre::FrameEvent &evt){

	if(mGoingForward && mAccelForward < mTopAccel) mAccelForward += 1;
	else if(!mGoingForward && mAccelForward > 0) mAccelForward -= 1;

	if(mGoingLeft && mAccelLeft < mTopAccel) mAccelLeft += 1;
	else if(!mGoingLeft && mAccelLeft > 0) mAccelLeft -= 1;

	if(mGoingBack && mAccelBack < mTopAccel) mAccelBack += 1;
	else if(!mGoingBack && mAccelBack > 0) mAccelBack -= 1;

	if(mGoingRight && mAccelRight < mTopAccel) mAccelRight += 1;
	else if(!mGoingRight && mAccelRight > 0) mAccelRight -= 1;

	if(mGoingUp && mAccelUp < mTopAccel) mAccelUp += 1;
	else if(!mGoingUp && mAccelUp > 0) mAccelUp -= 1;

	if(mGoingDown && mAccelDown < mTopAccel) mAccelDown += 1;
	else if(!mGoingDown && mAccelDown > 0) mAccelDown -= 1;

	Ogre::Vector3 velocity = Ogre::Vector3::ZERO;

	if(mGoingForward || mAccelForward)
		velocity += mAccelForward * getForwardDirection() / mTopAccel;
	if(mGoingLeft || mAccelLeft)
		velocity -= mAccelLeft * getRightDirection() / mTopAccel;
	if(mGoingBack || mAccelBack)
		velocity -= mAccelBack * getForwardDirection() / mTopAccel;
	if(mGoingRight || mAccelRight)
		velocity += mAccelRight * getRightDirection() / mTopAccel;

	if(mGoingUp || mAccelUp)
		velocity += mAccelUp * getUpDirection()  / mTopAccel;
	if(mGoingDown || mAccelDown)
		velocity -= mAccelDown * getUpDirection()  / mTopAccel;

	if(mGraphicsSetUp){
		mNodePositionX = mBody->getSceneNode()->getPosition().x;
		mNodePositionY = mBody->getSceneNode()->getPosition().y;
		mNodePositionZ = mBody->getSceneNode()->getPosition().z;
	}

	mNodePositionX += (velocity.x * evt.timeSinceLastFrame * mTopSpeed);
	mNodePositionY += (velocity.y * evt.timeSinceLastFrame * mTopSpeed);
	mNodePositionZ += (velocity.z * evt.timeSinceLastFrame * mTopSpeed);

	Ogre::Vector3 pos(mNodePositionX, mNodePositionY + 7, mNodePositionZ);

	if(mGraphicsSetUp){

		mBody->getBulletRigidBody()->translate(
			btVector3(
				mPositionCorrection.x,
				mPositionCorrection.y,
				mPositionCorrection.z
			)
		);

		mBody->getBulletRigidBody()->setAngularFactor(btVector3(0, 0, 0));
		mBody->setLinearVelocity(velocity * mTopSpeed);

	}

	if(mCameraManager)
		mCameraManager->setPosition(mPositionCorrection + pos);

	mNodePositionX += mPositionCorrection.x;
	mNodePositionY += mPositionCorrection.y;
	mNodePositionZ += mPositionCorrection.z;

	mPositionCorrection = Ogre::Vector3::ZERO;

	return true;

}

bool LocalPlayer::hadUsefulInput(){

	if(!mHadInputUseful)
		return false;

	mHadInputUseful = false;

	return true;

}

double LocalPlayer::getNodePositionX(){
	return mBody->getSceneNode()->getPosition().x;
}
double LocalPlayer::getNodePositionY(){
	return mBody->getSceneNode()->getPosition().y;
}
double LocalPlayer::getNodePositionZ(){
	return mBody->getSceneNode()->getPosition().z;
}
void LocalPlayer::setNodePositionX(double nodePositionX){
	mPositionCorrection.x += mBody->getSceneNode()->getPosition().x - nodePositionX;
}
void LocalPlayer::setNodePositionY(double nodePositionY){
	mPositionCorrection.x += mBody->getSceneNode()->getPosition().y - nodePositionY;
}
void LocalPlayer::setNodePositionZ(double nodePositionZ){
	mPositionCorrection.x += mBody->getSceneNode()->getPosition().z - nodePositionZ;
}

Ogre::Vector3 LocalPlayer::getForwardDirection(){
	using namespace Ogre;
	return Quaternion(Degree(mNodeYaw), Vector3::UNIT_Y) * Vector3::UNIT_Z * -1;
}

Ogre::Vector3 LocalPlayer::getUpDirection(){
	return Ogre::Vector3::UNIT_Y;
}

Ogre::Vector3 LocalPlayer::getRightDirection(){
	using namespace Ogre;
	return Quaternion(Degree(mNodeYaw), Vector3::UNIT_Y) * Vector3::UNIT_X;
}
