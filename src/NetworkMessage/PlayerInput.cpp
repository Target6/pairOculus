#include "PlayerInput.hpp"

namespace NetworkMessage{

	PlayerInput::PlayerInput(Player *player):
		NetworkMessage(PLAYERINPUT),
		mNickname(player->getNickname()),
		mNodeYaw(player->getNodeYaw()),
		mNodePitch(player->getNodePitch()),
		mNodeRoll(player->getNodeRoll()),
		mNodePositionX(player->getNodePositionX()),
		mNodePositionY(player->getNodePositionY()),
		mNodePositionZ(player->getNodePositionZ()),
		mGoingForward(player->getGoingForward()),
		mGoingBack(player->getGoingBack()),
		mGoingLeft(player->getGoingLeft()),
		mGoingRight(player->getGoingRight()),
		mGoingUp(player->getGoingUp()),
		mGoingDown(player->getGoingDown())
	{
	}

	PlayerInput::PlayerInput(
		std::string nickname,
		double nodeYaw,
		double nodePitch,
		double nodeRoll,
		long nodePositionX,
		long nodePositionY,
		long nodePositionZ,
		bool goingForward,
		bool goingBack,
		bool goingLeft,
		bool goingRight,
		bool goingUp,
		bool goingDown
	):
		NetworkMessage(PLAYERINPUT),
		mNickname(nickname),
		mNodeYaw(nodeYaw),
		mNodePitch(nodePitch),
		mNodeRoll(nodeRoll),
		mNodePositionX(nodePositionX),
		mNodePositionY(nodePositionY),
		mNodePositionZ(nodePositionZ),
		mGoingForward(goingForward),
		mGoingBack(goingBack),
		mGoingLeft(goingLeft),
		mGoingRight(goingRight),
		mGoingUp(goingUp),
		mGoingDown(goingDown)
	{
	}

	PlayerInput::~PlayerInput(){
	}

	std::string PlayerInput::getNickname(){return mNickname;}

	double PlayerInput::getNodeYaw(){return mNodeYaw;}
	double PlayerInput::getNodePitch(){return mNodePitch;}
	double PlayerInput::getNodeRoll(){return mNodeRoll;}

	long PlayerInput::getNodePositionX(){return mNodePositionX;}
	long PlayerInput::getNodePositionY(){return mNodePositionY;}
	long PlayerInput::getNodePositionZ(){return mNodePositionZ;}

	bool PlayerInput::getGoingForward(){return mGoingForward;}
	bool PlayerInput::getGoingBack(){return mGoingBack;}
	bool PlayerInput::getGoingLeft(){return mGoingLeft;}
	bool PlayerInput::getGoingRight(){return mGoingRight;}
	bool PlayerInput::getGoingUp(){return mGoingUp;}
	bool PlayerInput::getGoingDown(){return mGoingDown;}

	void PlayerInput::buildSelf(){
		
		
		
	}

};
