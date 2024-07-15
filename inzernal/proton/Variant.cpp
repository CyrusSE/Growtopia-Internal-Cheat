#include "PlatformPrecomp.h"
#include "Variant.h"
#include "MiscUtils.h"

Variant::~Variant()
{
	SAFE_DELETE(m_pSig_onChanged);
}

void* Variant::GetSigOnChanged()
{

	return m_pSig_onChanged;
}

void Variant::ClearConnections()
{
	SAFE_DELETE(m_pSig_onChanged);
}

void Variant::Set( string const &var )
{
	assert(m_type == TYPE_UNUSED || m_type == TYPE_STRING);
	m_type = TYPE_STRING;
	m_string = var;
}

void Variant::SetVariant( Variant *pVar ) //needed this because boost was confused...
{
	Set(*pVar);
}

void Variant::Set(const Variant &v )
{
	
	//update our data from another variant object
	switch(v.GetType())
	{
	case TYPE_FLOAT:
		Set(v.GetFloat());
		break;
	case TYPE_STRING:
		Set(v.GetString());
		break;
	case TYPE_VECTOR2:
		Set(v.GetVector2());
		break;
	case TYPE_VECTOR3:
		Set(v.GetVector3());
		break;
	case TYPE_UINT32:
		Set(v.GetUINT32());
		break;
	case TYPE_INT32:
		Set(v.GetINT32());
		break;
	case TYPE_ENTITY:
		Set(v.GetEntity());
		break;
	case TYPE_COMPONENT:
		Set(v.GetComponent());
		break;
	case TYPE_RECT:
		Set(v.GetRect());
		break;
	
	default:

		assert(!"Add support for this var type?");
	}
}

Variant& Variant::operator+=(const Variant& rhs)
{
	if (GetType() == rhs.GetType())
	{
		switch (GetType())
		{
		case TYPE_FLOAT:
			Set(GetFloat() + rhs.GetFloat());
			break;

		case TYPE_STRING:
			Set(GetString() + rhs.GetString());
			break;

		case TYPE_VECTOR2:
			Set(GetVector2() + rhs.GetVector2());
			break;

		case TYPE_VECTOR3:
			Set(GetVector3() + rhs.GetVector3());
			break;

		case TYPE_UINT32:
			Set(GetUINT32() + rhs.GetUINT32());
			break;

		case TYPE_INT32:
			Set(GetINT32() + rhs.GetINT32());
			break;

		default:
			break;
		}
	}

	return *this;
}

Variant& Variant::operator-=(const Variant& rhs)
{
	if (GetType() == rhs.GetType())
	{
		switch (GetType())
		{
		case TYPE_FLOAT:
			Set(GetFloat() - rhs.GetFloat());
			break;

		case TYPE_VECTOR2:
			Set(GetVector2() - rhs.GetVector2());
			break;

		case TYPE_VECTOR3:
			Set(GetVector3() - rhs.GetVector3());
			break;

		case TYPE_UINT32:
			Set(GetUINT32() - rhs.GetUINT32());
			break;

		case TYPE_INT32:
			Set(GetINT32() - rhs.GetINT32());
			break;

		default:
			break;
		}
	}

	return *this;
}

bool Variant::operator==(const Variant& rhs) const
{
	if (GetType() != rhs.GetType()) {
		return false;
	}

	switch (GetType())
	{
	case TYPE_UNUSED:
		return true;

	case TYPE_FLOAT:
		return GetFloat() == rhs.GetFloat();

	case TYPE_STRING:
		return GetString() == rhs.GetString();

	case TYPE_VECTOR2:
		return GetVector2() == rhs.GetVector2();

	case TYPE_VECTOR3:
		return GetVector3() == rhs.GetVector3();

	case TYPE_UINT32:
		return GetUINT32() == rhs.GetUINT32();

	case TYPE_ENTITY:
		return GetEntity() == rhs.GetEntity();

	case TYPE_COMPONENT:
		return GetComponent() == rhs.GetComponent();

	case TYPE_RECT:
		return GetRect() == rhs.GetRect();

	case TYPE_INT32:
		return GetINT32() == rhs.GetINT32();

	default:
		assert(!"No equals operator for this type yet implemented");
		return false;
	}
}

bool Variant::operator!=(const Variant& rhs) const
{
	return !operator==(rhs);
}

std::string Variant::Print()
{
	switch(GetType())
	{
	case TYPE_FLOAT:
		return toString(GetFloat());
		break;
	case TYPE_STRING:
		return GetString();
		break;
	
	case TYPE_VECTOR2:
		return PrintVector2(GetVector2());
		break;
	case TYPE_VECTOR3:
		return PrintVector3(GetVector3());
		break;

	case TYPE_UINT32:
		return toString(GetUINT32());
		break;
	case TYPE_INT32:
		return toString(GetINT32());
		break;
	case TYPE_ENTITY:
		//return GetEntity()->GetName();
		return "An entity"; //I don't want to include the entity.h here right now
		break;
	case TYPE_COMPONENT:
		//return GetEntity()->GetName();
		return "A component"; //I don't want to include the entity.h here right now
		break;
	case TYPE_RECT:
		return PrintRect(GetRect());
		break;

	case TYPE_UNUSED:
		return "Unknown";
		break;


	default:

		assert(!"Add support for this var type?");
	}

	return "Unknown";
}



//curPos is between 0 (value of A) and 1 (value of B)

void Variant::Interpolate(Variant *pA,Variant *pB, float curPos, eInterpolateType type)
{

	assert(GetType() == pA->GetType() && GetType() == pB->GetType() && "these should all be of the same type");
	bool bAsColor = false;

	switch (type)
	{
	case INTERPOLATE_LINEAR_AS_COLOR:
		bAsColor = true;
		break; //as is

	case INTERPOLATE_SMOOTHSTEP:
		curPos = SMOOTHSTEP(curPos);
		break;

	case INTERPOLATE_SMOOTHSTEP_AS_COLOR:
		curPos = SMOOTHSTEP(curPos);
		bAsColor = true;
		break;

	case INTERPOLATE_EASE_TO:
		curPos = EASE_TO(curPos);
		break;
	case INTERPOLATE_EASE_FROM:
		curPos = EASE_FROM(curPos);
		break;

	case INTERPOLATE_EASE_TO_QUARTIC:
		curPos = 1 - (curPos = 1 - curPos) * curPos * curPos * curPos;
		break;
	case INTERPOLATE_EASE_FROM_QUARTIC:
		curPos = curPos * curPos * curPos * curPos;
		break;

	case INTERPOLATE_BOUNCE_TO:
		if (curPos < 0.36363636f) {
			curPos = 7.5625f * curPos * curPos;
		} else if (curPos < 0.72727273f) {
			curPos = 7.5625f * (curPos -= 0.54545455f) * curPos + 0.75f;
		} else if (curPos < 0.90909091f) {
			curPos = 7.5625f * (curPos -= 0.81818182f) * curPos + 0.9375f;
		} else {
			curPos = 7.5625f * (curPos -= 0.95454545f) * curPos + 0.984375f;
		}
		break;

	case INTERPOLATE_LINEAR:
		break; //as is

	default:
		assert(0);
	}

	switch (pA->GetType())
	{

	case Variant::TYPE_FLOAT:
		{
			Set( pA->GetFloat() + (   (pB->GetFloat()- pA->GetFloat()  ) * curPos));
		}
		break;

	case Variant::TYPE_VECTOR2:
		{
			Set( pA->GetVector2() + (   (pB->GetVector2()- pA->GetVector2()  ) * curPos));
		}

		break;

		case Variant::TYPE_UINT32:
		{
			if (bAsColor)
			{
#ifdef _CONSOLE
assert(!"Not supported in console builds");
	pA->Set(pB->GetUINT32());
#else
				//Set(ColorCombineMix(pA->GetUINT32(), pB->GetUINT32(), curPos));
#endif
			} else
			{
				Set( uint32_t(  float(pA->GetUINT32()) + (   (float(pB->GetUINT32())- float(pA->GetUINT32())  ) * curPos)));

			}
		}

		break;

		case Variant::TYPE_INT32:
			{
				Set( int32_t(  float(pA->GetINT32()) + (   (float(pB->GetINT32())- float(pA->GetINT32())  ) * curPos)));
			}

			break;
	default:
		assert(0);
	}
}

bool Variant::Save( FILE *fp, const string &varName )
{
	//first, filter out types we can't save, like pointers
	int dataSizeBytes = 0;
	
	if (GetType() == TYPE_STRING)
	{
		dataSizeBytes = (int)m_string.size();
	} else
	{
		dataSizeBytes = GetSizeOfData(GetType());
	}
	
	//if (dataSizeBytes == 0)
	//{
	//	//LogMsg("Warning: Not going to save variant %s because it was never actually set a value", varName.c_str());
	//	return true; //can't save this type, but not an error
	//}

	uint32_t varType = GetType();
	int bytesRead = (int)fwrite(&varType, 1, sizeof(uint32_t), fp);
	if (bytesRead == 0) return false;

	//save the variable name
	//SaveToFile(varName, fp);
	
	if (GetType() == TYPE_STRING)
	{
		//SaveToFile(m_string, fp);
	} else
	{
		fwrite(m_var, dataSizeBytes, 1, fp);
	}

	
	return true;
}

inline Variant operator+(Variant lhs, const Variant& rhs)
{
	lhs += rhs;
	return lhs;
}

inline Variant operator-(Variant lhs, const Variant& rhs)
{
	lhs -= rhs;
	return lhs;
}

int GetSizeOfData(Variant::eType type)
{
	switch(type)
	{
	case Variant::TYPE_UNUSED:
	case Variant::TYPE_COMPONENT:
	case Variant::TYPE_ENTITY:
		return 0;
		break;

	case Variant::TYPE_UINT32:
	case Variant::TYPE_INT32:
	case Variant::TYPE_FLOAT:
		return 4;
	case Variant::TYPE_VECTOR2:
		return sizeof(CL_Vec2f);
	case Variant::TYPE_VECTOR3:
		return sizeof(CL_Vec3f);
	case Variant::TYPE_RECT:
		return sizeof(CL_Rectf);

	default:

		assert(!"Uh..");
	}

	return 0;
}

void VariantList::GetVariantListStartingAt(VariantList *pOut, int startIndex)
{
	int cur = 0;

	for (int i=startIndex; i < C_MAX_VARIANT_LIST_PARMS;i++)
	{
		pOut->m_variant[cur++] = m_variant[i];
	}
}

string VariantList::GetContentsAsDebugString()
{
	string s;

	for (int i=0; i < C_MAX_VARIANT_LIST_PARMS; i++)
	{
		if (m_variant[i].GetType() == Variant::TYPE_UNUSED)
		{
			break;
		} else
		{
			s += "param "+toString(i)+", type " + toString(m_variant[i].GetType()) + ":" + m_variant[i].Print() + "\n";
		}
	}

	if (s.empty())
	{
		s = "(None)";
	}
	return s;
}
