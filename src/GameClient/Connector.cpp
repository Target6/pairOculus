/*
This source file is part of pairOculus, a student project aiming at creating a
simple 3D multiplayer game for the Oculus Rift.

Repository can be found here : https://github.com/Target6/pairOculus 

Copyright (c) 2013 Target6

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "Connector.hpp"

namespace GameClient{

	Connector::Connector(
		boost::asio::io_service &io_service,
		tcp::resolver::iterator iterator,
		boost::function<void ()> close,
		boost::function<
			void (NetworkMessage::NetworkMessage *message)
		> receive
	):
		NetworkIO(io_service),
		mIterator(iterator),
		mClose(close),
		mReceive(receive)
	{
	}

	Connector::~Connector(){
		close();
	}

	void Connector::start(){
		boost::asio::async_connect(
			mSocket,
			mIterator,
			boost::bind(
				&Connector::handleReadBody,
				this,
				boost::asio::placeholders::error
			)
		);
	}

	void Connector::close(){

		if(mSocket.is_open()){
			try{
				mSocket.shutdown(mSocket.shutdown_both);
				mSocket.close();
			}
			catch(std::exception& e){
				std::cout << e.what() << std::endl;
			}
		}

		mClose();
		mIsClosed = true;

	}

	void Connector::handleReceive(NetworkMessage::NetworkMessage *message){
		mReceive(message);
	}

};
