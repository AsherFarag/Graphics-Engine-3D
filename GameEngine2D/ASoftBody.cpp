#include "ASoftBody.h"
#include "Transform.h"
#include "CircleRB.h"
#include "SpringRB.h"

ASoftBody::ASoftBody(ESoftBodyShape a_Shape, vec2 a_Position, float a_Damping, float a_SpringForce, float a_Spacing)
{
	std::vector<std::string> sb;
	switch (a_Shape)
	{
	case ASoftBody::MAGNET:
		sb.push_back("000000");
		sb.push_back("000000");
		sb.push_back("00....");
		sb.push_back("00....");
		sb.push_back("000000");
		sb.push_back("000000");
		break;
	case ASoftBody::BOX:
		sb.push_back("000000");
		sb.push_back("000000");
		sb.push_back("00..00");
		sb.push_back("00..00");
		sb.push_back("000000");
		sb.push_back("000000");
		break;
	default:
		break;
	}
	Build(a_Position, a_Damping, a_SpringForce, a_Spacing, sb);
}

void ASoftBody::Build(vec2 a_Position, float a_Damping, float a_SpringForce, float a_Spacing, std::vector<std::string>& a_Joints)
{
	const int NumColumns = a_Joints.size();
	const int NumRows = a_Joints[0].length();

	// Add Joints
	CircleRB** Joints = new CircleRB*[NumRows * NumColumns];
	for (int i = 0; i < NumRows; i++)
	{
		for (int j = 0; j < NumColumns; j++)
		{
			if (a_Joints[j][i] == '0')
			{
				Transform* T = new Transform(a_Position + vec2(i, j) * a_Spacing);
				Joints[i * NumColumns + j] = T->AddComponent<CircleRB>(T, RigidBody::DYNAMIC, 10.f, DEFAULT_COLOUR, 1.f);
			}
			else
			{
				Joints[i * NumColumns + j] = nullptr;
			}
		}
	}

	// Add Springs
	for (int i = 1; i < NumRows; i++)
	{
		for (int j = 1; j < NumColumns; j++)
		{
			CircleRB* s11 = Joints[i * NumColumns + j];
			CircleRB* s01 = Joints[(i - 1) * NumColumns + j];
			CircleRB* s10 = Joints[i * NumColumns + j - 1];
			CircleRB* s00 = Joints[(i - 1) * NumColumns + j - 1];

			// make springs to cardinal neighbours
			if (s11 && s01)	new SpringRB(nullptr, s11, s01, a_SpringForce, a_Damping, vec4(0, 1, 1, 1), a_Spacing);
			if (s11 && s10)	new SpringRB(nullptr, s11, s10, a_SpringForce, a_Damping, vec4(0, 1, 1, 1), a_Spacing);
			if (s10 && s00)	new SpringRB(nullptr, s10, s00, a_SpringForce, a_Damping, vec4(0, 1, 1, 1), a_Spacing);
			if (s01 && s00)	new SpringRB(nullptr, s01, s00, a_SpringForce, a_Damping, vec4(0, 1, 1, 1), a_Spacing);

			bool EndOfJ = j == NumColumns - 1;
			bool EndOfI = i == NumRows - 1;

			CircleRB* s22 = (!EndOfI && !EndOfJ) ? Joints[(i + 1) * NumColumns + (j + 1)] : nullptr;
			CircleRB* s02 = !EndOfJ ? Joints[(i - 1) * NumColumns + (j + 1)] : nullptr;
			CircleRB* s20 = !EndOfI ? Joints[(i + 1) * NumColumns + j - 1] : nullptr;

			if (s11 && s00)	new SpringRB(nullptr, s11, s00, a_SpringForce, a_Damping, vec4(0, 1, 1, 1), a_Spacing);
			if (s10 && s01)	new SpringRB(nullptr, s10, s01, a_SpringForce, a_Damping, vec4(0, 1, 1, 1), a_Spacing);
			//if (s20 && s00)	new SpringRB(nullptr, s20, s00, a_SpringForce, a_Damping, vec4(0, 1, 1, 1), a_Spacing);
			//if (s02 && s00)	new SpringRB(nullptr, s02, s00, a_SpringForce, a_Damping, vec4(0, 1, 1, 1), a_Spacing);
		}
	}
}
