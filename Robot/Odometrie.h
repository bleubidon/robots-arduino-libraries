#pragma once

#include "vec.h"
#include "Codeuse.h"
#include "Gyroscope.h"

extern const uint8_t DOUBLE_CODEUSE;
extern const uint8_t CODEUSE_GYROSCOPE;

class Odometrie
{
	public:
		struct Config
		{
			vec position;
			int angle;
			uint8_t mode;

			union
			{
				// DOUBLE_CODEUSE
				struct {
					Codeuse::Config gauche, droite;
					float ecart_entre_roues;  // en cm
				};

				// CODEUSE_GYROSCOPE
				struct {
					Codeuse::Config codeuse;
				};
			};
		};


		void setup(Odometrie::Config config);

		void update();

		const vec& pos();  // en cm
		const vec& dir();  // norme 1
		const float& dist();  // en cm
		const float& rot();  // en °

	private:
		uint8_t mode;
		union
		{
			// DOUBLE_CODEUSE
			struct {
				Codeuse codeuses[2];
				float ecart_entre_roues;  // en cm
			};

			// CODEUSE_GYROSCOPE
			struct {
				Codeuse codeuse;
				Gyroscope gyro;
			};
		};

		vec position, direction;
		float distance, angle;
		float dist_prev;

		void updateDoubleCodeuse();
		void updateCodeuseGyroscope();
};
