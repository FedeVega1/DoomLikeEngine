#pragma once

class Time
{
	friend class Game;

public:
	static Time INS;

	Time() : startTime(), simulationTime(0LL), fixedTimeSlice((long long) (TimeSlice * 1000))
	{ }

	static const float TimeSlice;

	long long GetTime() const
	{
		std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
	}

private:
	std::chrono::high_resolution_clock::time_point startTime;

	long long simulationTime;
	long long fixedTimeSlice;

	long long GetGameTickCount() const
	{
		std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
	}
};
