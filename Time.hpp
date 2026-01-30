#ifndef TIME_HPP
#define TIME_HPP

class TimeManager
{
public:
	static TimeManager& Instance()
	{
		static TimeManager instance;
		return instance;
	}

	TimeManager(const TimeManager&) = delete;
	TimeManager& operator=(const TimeManager&) = delete;

	void Update(unsigned long elapsedTimeMs)
	{
		_time = (double)elapsedTimeMs / 1000.0;
		_deltaTime = (double)(elapsedTimeMs - _previousTime) / 1000.0;
		_previousTime = elapsedTimeMs;
	}

	double GetTime() const { return _time; }
	double GetDeltaTime() const { return _deltaTime; }

private:
	TimeManager() = default;

	unsigned long _previousTime = 0;
	double _time = 0.0;
	double _deltaTime = 0.0;
};

#endif
