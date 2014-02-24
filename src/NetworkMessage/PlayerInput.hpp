#ifndef __NETWORKMESSAGEPLAYERINPUT_H
#define __NETWORKMESSAGEPLAYERINPUT_H

#include <string>
#include <sstream>
#include "../Player.hpp"
#include "NetworkMessage.hpp"

namespace NetworkMessage{

	class PlayerInput : public NetworkMessage{

		public:
			//methods
			PlayerInput(Player *player);
			PlayerInput(
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
			);
			~PlayerInput();
			
			std::string getNickname();
			
			double getNodeYaw();
			double getNodePitch();
			double getNodeRoll();

			long getNodePositionX();
			long getNodePositionY();
			long getNodePositionZ();

			bool getGoingForward();
			bool getGoingBack();
			bool getGoingLeft();
			bool getGoingRight();
			bool getGoingUp();
			bool getGoingDown();

		private:
			//methods
			void buildSelf();

			//arguments
			std::string mNickname;

			double mNodeYaw;
			double mNodePitch;
			double mNodeRoll;

			long mNodePositionX;
			long mNodePositionY;
			long mNodePositionZ;

			bool mGoingForward;
			bool mGoingBack;
			bool mGoingLeft;
			bool mGoingRight;
			bool mGoingUp;
			bool mGoingDown;

	};

};

#endif //__NETWORKMESSAGEPLAYERINPUT_H
