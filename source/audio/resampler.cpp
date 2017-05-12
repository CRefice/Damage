#include "resampler.hpp"

void Resampler::reset(double inputFrequency, double outputFrequency) {
	inputFreq = inputFrequency;
	outputFreq = outputFrequency;

	ratio = inputFreq / outputFreq;
	fraction = 0.0;
	for (auto& s : history) s = 0.0;
	samples.resize(outputFreq * 0.02);
}

void Resampler::write(double sample) {
	auto& mu = fraction;
	auto& s = history;

	s[0] = s[1];
	s[1] = s[2];
	s[2] = s[3];
	s[3] = sample;

	while (mu <= 1.0) {
		double A = s[3] - s[2] - s[0] + s[1];
		double B = s[0] - s[1] - A;
		double C = s[2] - s[0];
		double D = s[1];

		samples.write(A * mu * mu * mu + B * mu * mu + C * mu + D);
		mu += ratio;
	}

	mu -= 1.0;
}
