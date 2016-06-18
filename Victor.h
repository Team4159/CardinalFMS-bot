#ifndef VICTOR_H
#define VICTOR_H

class Victor {
public:
	Victor(int channel);

	void set(double value);
	double get();
	void setInverted(bool inverted);
	bool getInverted();
	int getChannel();
	
	void stopMotor();
	
	void disable();
	void enable();
	bool isEnabled();
	
	~Victor();
private:
	double value;
	bool inverted;
	int channel;
	bool enabled;
};

#endif