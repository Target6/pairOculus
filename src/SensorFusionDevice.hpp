#ifndef __SENSORFUSIONDEVICE_H
#define __SENSORFUSIONDEVICE_H

#include "SensorFusionListener.hpp"
#include <OVR.h>

class SensorFusionDevice{

	public:
		//Methods
		SensorFusionDevice();
		~SensorFusionDevice();

		void capture();
		virtual void setEventCallback(SensorFusionListener *listener);

	private:
		//Arguments
		Ogre::Vector3 mLast;
		SensorFusionListener *mListener;
		OVR::Ptr<OVR::DeviceManager> mManager;
		OVR::Ptr<OVR::HMDDevice> mHMD;
		OVR::Ptr<OVR::SensorDevice> mSensor;
		OVR::SensorFusion *mSensorFusion;

};

#endif //__SENSORFUSIONDEVICE_H