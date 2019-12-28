#include "stdafx.h"
#include "LinearTrajectory.h"

LinearTrajectory::LinearTrajectory(const char* type, GLint itCount, GLfloat speed, GLboolean direction, const Vector3& start, const Vector3& end)
	:VaryingTrajectory{ type, itCount, speed, direction }, LineTrajectory{ type, itCount, speed, {start, end} }, Trajectory{ type, itCount, speed }
{
	calculateTravelProps(orientation);
}

LinearTrajectory::LinearTrajectory(Type type, GLint itCount, GLfloat speed, GLboolean direction, const Vector3& start, const Vector3& end)
	: VaryingTrajectory{ type, itCount, speed, direction }, LineTrajectory{ type, itCount, speed, {start, end} }, Trajectory{ type, itCount, speed }
{
	calculateTravelProps(orientation);
}

void LinearTrajectory::rotate(SceneObject* object, GLfloat time)
{
	// TODO;
}

void LinearTrajectory::move(SceneObject* object, GLfloat time)
{
	if (iterationCount != 0)
	{
		Vector4 step = calculateStep(time);

		if (distance - step.Length() < 0.0f)
		{
			Vector3& obj = object->getPosition();
			object->setPosition(points[pos + orientation]);
			
			iterationCount--;

			if (iterationCount != 0)
			{
				// TODO; rotate

				if (alternateDirection)
				{
					orientation = -orientation;
				}

				calculateTravelProps(orientation);
			}
		}
		else
		{
			Vector3& obj = object->getPosition();
			object->setPosition({ obj.x + step.x, obj.y + step.y, obj.z + step.z });

			distance -= step.Length();
		}
	}
}
