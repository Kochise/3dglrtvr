#pragma once

#include "../SmartPtr.h"
#include "Vector.h"
#include <vector>
using namespace std;



//Basic force class. 
class CForce
{
public:
	virtual ~CForce(void){}
	CForce(CVector3 Linear,CVector3 Torque){m_Linear = Linear; m_Torque = Torque;}

	CVector3 m_Torque;
	CVector3 m_Linear;
};



class CEntity;
typedef SmartPtr<CEntity> CEntityPtr;


//Interface for a force. The entity is passed in so we can model forces that vary based on the entity.
class IForce
{
public:
	virtual ~IForce(){}
	IForce(){}

	virtual CForce GetForce(CEntityPtr& pEntity) = 0;
};
typedef SmartPtr<IForce> IForcePtr;
typedef vector<IForcePtr> IForcePtrVector;


//Directional Force. Like gravity pulling down on an object
class CDirectionalForce : public IForce
{
public:
	virtual ~CDirectionalForce(){}
	CDirectionalForce(CVector3 direction,float fAcceleration)
	{
		m_Direction = direction;
		m_fAcceleration = fAcceleration;
	}

	virtual CForce GetForce(CEntityPtr& pEntity);

protected:

	CVector3 m_Direction;
	float    m_fAcceleration;
};
typedef SmartPtr<CDirectionalForce> CDirectionalForcePtr;


//Gravitational force (like that between planets)
class CGravitationalForce : public IForce
{
public:
	virtual ~CGravitationalForce(){}
	CGravitationalForce(CEntityPtr pEntity,float fGravitationalConstant)
	{
		m_pEntity = pEntity;
		m_fGravitationalConstant = fGravitationalConstant;
	}

	virtual CForce GetForce(CEntityPtr& pEntity);

protected:

	CEntityPtr m_pEntity;
	float      m_fGravitationalConstant;
};
typedef SmartPtr<CGravitationalForce> CGravitationalForcePtr;


//A spring which is attached to the world and to an object
class CGroundedSpringForce : public IForce
{
public:
	virtual ~CGroundedSpringForce(){}
	CGroundedSpringForce(CVector3 GroundPosition,CVector3 AttachmentPosition, float fRestingLength, float fForce)
	{
		m_GroundPosition     = GroundPosition;
		m_AttachmentPosition = AttachmentPosition;
		m_fRestingLength     = fRestingLength;
		m_fForce             = fForce;
	}

	virtual CForce GetForce(CEntityPtr& pEntity);

protected:

	CVector3 m_GroundPosition;
	CVector3 m_AttachmentPosition;
	float    m_fRestingLength;
	float    m_fForce;
};
typedef SmartPtr<CGroundedSpringForce> CGroundedSpringForcePtr;





//Thrust force. This is a force that is applied in a direction relative to the entity's orientation
class CThrustForce : public IForce
{
public:
	virtual ~CThrustForce(){}
	CThrustForce(CVector3 Position,CVector3 ForceDirection,float fForce)
	{
		m_Position  = Position;
		m_Direction = ForceDirection;
		m_fForce = fForce;
	}

	virtual CForce GetForce(CEntityPtr& pEntity);

	void SetThrustForce(float fForce){m_fForce = fForce;}

	CVector3 GetPosition()                 {return m_Position;}
	void     SetPosition(CVector3 Position){m_Position = Position;}


protected:

	CVector3 m_Direction;
	CVector3 m_Position;
	float    m_fForce;
};
typedef SmartPtr<CThrustForce> CThrustForcePtr;



/*
class CSuspensionForce : public IForce
{
public:
	virtual ~CSuspensionForce(){}
	CSuspensionForce(CVector3 AttachmentPosition,CVector3 ForceDirection,float fForce,float fRestingLength)
	{
		m_Position       = AttachmentPosition;
		m_Direction      = ForceDirection;
		m_fForce         = fForce;
		m_fRestingLength = fRestingLength;
	}

	virtual CForce GetForce(CEntityPtr& pEntity);

	CVector3 GetPosition()                 {return m_Position;}
	void     SetPosition(CVector3 Position){m_Position = Position;}

protected:

	CVector3 m_Position;
	CVector3 m_Direction;
	float    m_fForce;
	float    m_fRestingLength;
};
typedef SmartPtr<CSuspensionForce> CSuspensionForcePtr;
*/

class CDragForce : public IForce
{
public:
	virtual ~CDragForce(){}
	CDragForce(float fLinearDrag,float fAngularDrag)
	{
		m_fLinear   = fLinearDrag;
		m_fAngular  = fAngularDrag;
	}

	virtual CForce GetForce(CEntityPtr& pEntity);

	float GetLinear()              {return m_fLinear;}
	void  SetLinear(float fLinear) {m_fLinear = fLinear;}

	float GetAngular()              {return m_fAngular;}
	void  SetAngular(float fAngular) {m_fAngular = fAngular;}

protected:

	float    m_fLinear;
	float    m_fAngular;
};
typedef SmartPtr<CDragForce> CDragForcePtr;


/*
class CFollowForce : public IForce
{
public:
	virtual ~CFollowForce(){}
	CFollowForce(CEntityPtr pFollow,CVector3 Position,float fForce)
	{
		m_Position = Position;
		m_pFollow   = pFollow;
		m_fForce   = fForce;
	}

	virtual CForce GetForce(CEntityPtr& pEntity);

protected:

	CEntityPtr  m_pFollow;
	CVector3    m_Position;
	float       m_fForce;
};
typedef SmartPtr<CDragForce> CDragForcePtr;
*/


/*

	public interface IForce  //Interface for a force. The entity is passed in so we can model forces that vary based on the entity.
	{
		Force GetForce(Entity entity);
	}

	//Basic force class. This also implements IForce for a force that is independant of the entity it is acting on.
	public class Force : IForce
	{
		public Vector3 Torque;
		public Vector3 Linear;

		public Force(Vector3 linear,Vector3 torque)
		{
			Linear = linear;
			Torque = torque;
		}

		public virtual Force GetForce(Entity entity)
		{
			return this;
		}
	}
	
	//Thrust force. This is a force that is applied in a direction relative to the entity's orientation
	public class ThrustForce : IForce
	{
		public Vector3 Torque;
		public Vector3 Linear;

		public ThrustForce(Vector3 linear,Vector3 torque)
		{
			Linear = linear;
			Torque = torque;
		}

		public Force GetForce(Entity entity)
		{
			Vector4 newLinear = Vector3.Transform(Linear,entity.Position.Orientation);
			Vector4 newTorque = Vector3.Transform(Torque,entity.Position.Orientation);
			return new Force(new Vector3(newLinear.X,newLinear.Y,newLinear.Z),new Vector3(newTorque.X,newTorque.Y,newTorque.Z));
		}
	}

	//Drag Force. This force slows an object down and is proportional to the objects velocity^2
	public class DragForce : IForce
	{
		public float LinearDrag;
		public float AngularDrag;

		public DragForce(float linearDrag,float angularDrag)
		{
			LinearDrag = linearDrag;
			AngularDrag = angularDrag;
		}

		public Force GetForce(Entity entity)
		{
			return new Force( -1f *Vector3.Normalize(entity.Motion.Velocity)        * LinearDrag * entity.Motion.Velocity.LengthSq() ,
			                  -1f *Vector3.Normalize(entity.Motion.AngularVelocity) * AngularDrag* entity.Motion.Velocity.LengthSq() );
		}
	}

	public class ZAxisDragForce : IForce
	{
		public float LinearDrag;

		public ZAxisDragForce(float linearDrag)
		{
			LinearDrag = linearDrag;
		}

		public Force GetForce(Entity entity)
		{
			Vector3 zVelocity = new Vector3(0,0,entity.Motion.Velocity.Z);
			return new Force( -1f * LinearDrag * zVelocity * (float)Math.Abs(zVelocity.Z), new Vector3(0,0,0) );
		}
	}

	public class SpringForce : IForce
	{
		public Vector3 Position;
		public float Force;

		public SpringForce(Vector3 position,float force)
		{
			Position = position;
			Force = force;
		}

		public Force GetForce(Entity entity)
		{
			Vector3 force = new Vector3(0,0,0);
			force = (Position - entity.Position.Center)*Force;
			return new Force(force,new Vector3(0,0,0));
		}
	}

	public class FollowForce : IForce
	{
		public Entity  Follow;
		public Vector3 Position;
		public float   Force;
		public float   AngularForce;

		public FollowForce(Entity follow,Vector3 position,float force,float angularForce)
		{
			Position = position;
			Follow = follow;
			Force = force;
			AngularForce = angularForce;
		}

		public Force GetForce(Entity entity)
		{
			//Spring that equalizes velocity
			Vector3 RelativeVelocity = Follow.Motion.Velocity - entity.Motion.Velocity; 
			Vector3 velocityForce = RelativeVelocity * Force  *2;

			//Spring that equalizes position
			Vector3 relativePosition = Vector3.TransformNormal(Position,Follow.Position.Orientation);
			Vector3 desiredPosition  = Follow.Position.Center + relativePosition;
			Vector3 springForce = (desiredPosition - entity.Position.Center)*(desiredPosition - entity.Position.Center).Length()*Force;

			return new Force(velocityForce+springForce,new Vector3(0,0,0));
		}
	}


	public class GravitationalForce : IForce
	{
		public Vector3 Direction;
		public float   Acceleration;

		public GravitationalForce(Vector3 direction,float acceleration)
		{
			Direction = direction;
			Acceleration = acceleration;
		}

		public Force GetForce(Entity entity)
		{
			return new Force(Direction * Acceleration * entity.CollisionModel.Mass,new Vector3(0,0,0));
		}
	}

	public class AttractiveForce : IForce
	{
		public Vector3 Position;
		public float   Magnitude;

		public AttractiveForce(Vector3 position,float magnitude)
		{
			Position = position;
			Magnitude = magnitude;
		}

		public Force GetForce(Entity entity)
		{
			Vector3 distanceVector = Position-entity.Position.Center;
			float   distance       = distanceVector.Length();
			Vector3 forceDirection = distanceVector * (1f/distance);

			Vector3 force = forceDirection * Magnitude * distance;
			return new Force(force,new Vector3(0,0,0));
		}
	}

	public class GravityWell : IForce
	{
		public Vector3 Position;
		public float   Magnitude;

		public GravityWell(Vector3 position,float magnitude)
		{
			Position = position;
			Magnitude = magnitude;
		}

		public Force GetForce(Entity entity)
		{
			Vector3 distanceVector = Position-entity.Position.Center;
			float   distance       = distanceVector.Length();
			if(distance < 0.001f) distance = 0.001f;

			Vector3 forceDirection = distanceVector * (1f/distance);
			Vector3 force = Magnitude * forceDirection * (1f / distance * distance);
			return new Force(force,new Vector3(0,0,0));
		}
	}


	public class ZAxisGravityWell : IForce
	{
		public float   Magnitude;

		public ZAxisGravityWell(float magnitude)
		{
			Magnitude = magnitude;
		}

		public Force GetForce(Entity entity)
		{
			Vector3 distanceVector = entity.Position.Center * -1;
			distanceVector.Z = 0;

			float   distance       = distanceVector.Length();
			if(distance < 0.001f) distance = 0.001f;

			Vector3 forceDirection = distanceVector * (1f/distance);
			Vector3 force = Magnitude * forceDirection * (1f / distance * distance);
			return new Force(force,new Vector3(0,0,0));
		}
	}

	*/
