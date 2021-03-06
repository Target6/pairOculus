/*
This source file is part of pairOculus, a student project aiming at creating a
simple 3D multiplayer game for the Oculus Rift.

Repository can be found here : https://github.com/Target6/pairOculus

Copyright (c) 2013 Zykino

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

#ifndef PYRAMID_HPP
#define PYRAMID_HPP

#include "Block.hpp"

///	A Pyramidal Block.
class Pyramid : public Block {
	public:
		/// Constructor
		Pyramid(
			Ogre::ManualObject *man,
			PrintType print,
			Real px,
			Real py,
			Real pz,
			Real sx = 100.0,
			Real sy = 100.0,
			Real sz = 100.0
		);
		/// Default destructor
		virtual ~Pyramid();
		/// Show the caracteristical points of the Pyramid
		virtual void createBlockPoint();
		/// Show the skeleton of the Pyramid
		virtual void createBlockWireframe();
		/// Show the solid Pyramid
		virtual void createBlockSolid();

	protected:
	private:
};

#endif // PYRAMID_HPP
