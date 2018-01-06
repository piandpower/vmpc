#include "IPlugStructs.h"
#include "Log.h"

IColor::IColor(int hexRGB, int alpha)
{
	B = ((hexRGB) & 0xff);
	G = (((hexRGB) >> 8) & 0xff);
	R = (((hexRGB) >> 16) & 0xff);
	A = alpha;
}
void IColor::SetHSV(double H, double S, double V)
{
	S /= 100;
	V /= 100;

	if (S <= 0)
	{
		R = G = B = int(V * 255);
	}
	else
	{
		int i;
		double f, p, q, t;

		if (H >= 360)
			H = 0;
		else
			H = H / 60;

		i = (int)trunc(H);
		f = H - i;

		p = V * (1.0 - S);
		q = V * (1.0 - (S * f));
		t = V * (1.0 - (S * (1.0 - f)));

		switch (i)
		{
		case 0:
			R = int(V * 255);
			G = int(t * 255);
			B = int(p * 255);
			break;

		case 1:
			R = int(q * 255);
			G = int(V * 255);
			B = int(p * 255);
			break;

		case 2:
			R = int(p * 255);
			G = int(V * 255);
			B = int(t * 255);
			break;

		case 3:
			R = int(p * 255);
			G = int(q * 255);
			B = int(V * 255);
			break;

		case 4:
			R = int(t * 255);
			G = int(p * 255);
			B = int(V * 255);
			break;

		default:
			R = int(V * 255);
			G = int(p * 255);
			B = int(q * 255);
			break;
		}
	}
}
void IColor::GetHSV(double *getH, double *getS, double *getV)
{
	double delta, min;
	double h = 0, s, v;

	min = IPMIN(IPMIN(R, G), B);
	v = IPMAX(IPMAX(R, G), B);
	delta = v - min;

	if (v <= 0.0)
		s = 0;
	else
		s = delta / v;

	if (s <= 0)
		h = 0.0;

	else
	{
		if (R >= v)
			h = (G - B) / delta;
		else if (G >= v)
			h = 2 + (B - R) / delta;
		else if (B >= v)
			h = 4 + (R - G) / delta;

		h *= 60;

		if (h < 0.0)
			h = h + 360;
	}
	*getH = h;
	*getS = s * 100;
	*getV = v / 255 * 100;
}
void IColor::SetHue(double hue)
{
	double H = 0, S = 0, V = 0;
	GetHSV(&H, &S, &V);
	SetHSV(hue, S, V);
}
void IColor::SetSaturation(double saturation)
{
	double H = 0, S = 0, V = 0;
	GetHSV(&H, &S, &V);
	SetHSV(H, saturation, V);
}
void IColor::SetBrightness(double brightness)
{
	double H = 0, S = 0, V = 0;
	GetHSV(&H, &S, &V);
	SetHSV(H, S, brightness);
}

void IMidiMsg::MakeNoteOnMsg(int noteNumber, int velocity, int offset, int channel)
{
	Clear();
	mStatus = channel | (kNoteOn << 4);
	mData1 = noteNumber;
	mData2 = velocity;
	mOffset = offset;
}

void IMidiMsg::MakeNoteOffMsg(int noteNumber, int offset, int channel)
{
	Clear();
	mStatus = channel | (kNoteOff << 4);
	mData1 = noteNumber;
	mOffset = offset;
}

void IMidiMsg::MakePitchWheelMsg(double value, int channel)
{
	Clear();
	mStatus = channel | (kPitchWheel << 4);
	int i = 8192 + (int)(value * 8192.0);
	i = BOUNDED(i, 0, 16383);
	mData2 = i >> 7;
	mData1 = i & 0x7F;
}

void IMidiMsg::MakeControlChangeMsg(EControlChangeMsg idx, double value, int channel)
{
	Clear();
	mStatus = channel | (kControlChange << 4);
	mData1 = idx;
	mData2 = (int)(value * 127.0);
}

int IMidiMsg::Channel()
{
	return mStatus & 0x0F;
}

IMidiMsg::EStatusMsg IMidiMsg::StatusMsg() const
{
	unsigned int e = mStatus >> 4;
	if (e < kNoteOff || e > kPitchWheel)
	{
		return kNone;
	}
	return (EStatusMsg)e;
}

int IMidiMsg::NoteNumber() const
{
	switch (StatusMsg())
	{
	case kNoteOn:
	case kNoteOff:
	case kPolyAftertouch:
		return mData1;
	default:
		return -1;
	}
}

int IMidiMsg::Velocity() const
{
	switch (StatusMsg())
	{
	case kNoteOn:
	case kNoteOff:
		return mData2;
	default:
		return -1;
	}
}

int IMidiMsg::PolyAfterTouch() const
{
	switch (StatusMsg())
	{
	case kPolyAftertouch:
		return mData2;
	default:
		return -1;
	}
}

int IMidiMsg::ChannelAfterTouch() const
{
	switch (StatusMsg())
	{
	case kChannelAftertouch:
		return mData1;
	default:
		return -1;
	}
}

int IMidiMsg::Program() const
{
	if (StatusMsg() == kProgramChange)
	{
		return mData1;
	}
	return -1;
}

double IMidiMsg::PitchWheel() const
{
	if (StatusMsg() == kPitchWheel)
	{
		int iVal = (mData2 << 7) + mData1;
		return (double)(iVal - 8192) / 8192.0;
	}
	return 0.0;
}

IMidiMsg::EControlChangeMsg IMidiMsg::ControlChangeIdx() const
{
	return (EControlChangeMsg)mData1;
}

double IMidiMsg::ControlChange(EControlChangeMsg idx) const
{
	if (StatusMsg() == kControlChange && ControlChangeIdx() == idx)
	{
		return (double)mData2 / 127.0;
	}
	return -1.0;
}

void IMidiMsg::Clear()
{
	mOffset = 0;
	mStatus = mData1 = mData2 = 0;
}

const char* StatusMsgStr(IMidiMsg::EStatusMsg msg)
{
	switch (msg)
	{
	case IMidiMsg::kNone: return "none";
	case IMidiMsg::kNoteOff: return "noteoff";
	case IMidiMsg::kNoteOn: return "noteon";
	case IMidiMsg::kPolyAftertouch: return "aftertouch";
	case IMidiMsg::kControlChange: return "controlchange";
	case IMidiMsg::kProgramChange: return "programchange";
	case IMidiMsg::kChannelAftertouch: return "channelaftertouch";
	case IMidiMsg::kPitchWheel: return "pitchwheel";
	default:  return "unknown";
	};
}

void IMidiMsg::LogMsg()
{
#ifdef TRACER_BUILD
	Trace(TRACELOC, "midi:(%s:%d:%d:%d)", StatusMsgStr(StatusMsg()), Channel(), mData1, mData2);
#endif
}

void ISysEx::Clear()
{
	mOffset = mSize = 0;
	mData = NULL;
}

char* SysExStr(char *str, int maxlen, const BYTE* pData, int size)
{
	assert(str != NULL && maxlen >= 3);

	if (!pData || !size) {
		*str = '\0';
		return str;
	}

	char* pStr = str;
	int n = maxlen / 3;
	if (n > size) n = size;
	for (int i = 0; i < n; ++i, ++pData) {
		sprintf(pStr, "%02X", (int)*pData);
		pStr += 2;
		*pStr++ = ' ';
	}
	*--pStr = '\0';

	return str;
}

void ISysEx::LogMsg()
{
#ifdef TRACER_BUILD
	char str[96];
	Trace(TRACELOC, "sysex:(%d:%s)", mSize, SysExStr(str, sizeof(str), mData, mSize));
#endif
}

IRECT::operator DRECT() const
{
	return DRECT((double)L, (double)T, (double)R, (double)B);
}

DRECT::operator IRECT()
{
	return IRECT((int)L, (int)T, (int)R, (int)B);
}
