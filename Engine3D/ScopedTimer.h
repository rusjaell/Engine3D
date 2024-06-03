#ifndef SCOPED_TIMER_H
#define SCOPED_TIMER_H

struct ScopedTimer {

	ScopedTimer(const char* message)
		: message_(message), duration_(std::chrono::duration<double>(0.0))
	{
		start_ = std::chrono::high_resolution_clock::now();
	}

	~ScopedTimer() {
		end_ = std::chrono::high_resolution_clock::now();
		duration_ = end_ - start_;

		double ms = duration_.count() * 1000.0;

#ifdef _DEBUG
		std::cout << "[Debug] " << message_ << " | " << ms << " ms" << '\n';
#else
		std::cout << "[Release] " << message_ << " | " << ms << " ms" << '\n';
#endif
	}

private:
	const std::string message_;

	std::chrono::time_point<std::chrono::steady_clock> start_;
	std::chrono::time_point<std::chrono::steady_clock> end_;
	std::chrono::duration<double> duration_;
};

#endif // !SCOPED_TIMER_H