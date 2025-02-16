#pragma once 

#include "SingleInstanceFuture.h"
#include "BluetoothWrapper.h"
#include "Constants.h"

#include <iostream>
#include <mutex>
#include <vector>
#include <tuple>

template <class T>
struct Property {
	T current;
	T desired;

	void fulfill();
	bool isFulfilled();
	void setState(T val);
};

class Headphones {
public:
	Headphones(BluetoothWrapper& conn);

	void setAmbientSoundControl(bool val);
	bool getAmbientSoundControl();

	bool isFocusOnVoiceAvailable();
	void setFocusOnVoice(bool val);
	bool getFocusOnVoice();

	bool isSetAsmLevelAvailable();
	void setAsmLevel(int val);
	int getAsmLevel();

	void setOptimizerState(OPTIMIZER_STATE val);
	OPTIMIZER_STATE getOptimizerState();

	void setS2CToggle(S2C_TOGGLE val);
	void setS2COptions(int sensitivity, bool voice, int offTime);
	S2C_TOGGLE getS2CToggle();
	unsigned int getS2COptions();

	void setSurroundPosition(SOUND_POSITION_PRESET val);
	SOUND_POSITION_PRESET getSurroundPosition();

	void setVptType(int val);
	int getVptType();

	bool getMultiPointSetting();
	const std::vector<BluetoothDevice>& getDevices();
	std::pair<int, int> getConnectedDevices();
	void setMultiPointConnection(int connectionId, int newDevice, int oldDevice);
	inline void disconnect(int deviceIdx);
	inline void connect(int deviceIdx);

	bool isChanged();
	void setChanges();

	void queryState();
	void setStateFromReply(BtMessage replyMessage);

	void queryDeviceCapabilities();
	void queryDevices();
	void queryS2C();
	void queryS2COptions();
	
	unsigned int getCapabilities();
private:
	unsigned int _capabilities = 0;

	Property<bool> _ambientSoundControl = { 0 };
	Property<bool> _focusOnVoice = { 0 };
	Property<int> _asmLevel = { 0 };

	Property<SOUND_POSITION_PRESET> _surroundPosition = { SOUND_POSITION_PRESET::OUT_OF_RANGE, SOUND_POSITION_PRESET::OFF };
	Property<int> _vptType = { 0 };

	Property<OPTIMIZER_STATE> _optimizerState = { OPTIMIZER_STATE::IDLE };

	Property<S2C_TOGGLE> _speakToChat = { S2C_TOGGLE::INACTIVE, S2C_TOGGLE::INACTIVE };
	Property<unsigned int> _s2cOptions = { 0 };
	
	std::vector<BluetoothDevice> _savedDevices;
	Property<int> _dev1 = { 0, 0 }, _dev2 = { 0, 0 };
	bool _multiPointSetting = true;

	std::mutex _propertyMtx;

	BluetoothWrapper& _conn;
};

template<class T>
inline void Property<T>::fulfill()
{
	this->current = this->desired;
}

template<class T>
inline bool Property<T>::isFulfilled()
{
	return this->desired == this->current;
}

template<class T>
inline void Property<T>::setState(T val)
{
	this->current = val;
	this->desired = val;
}