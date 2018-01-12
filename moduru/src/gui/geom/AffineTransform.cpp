#include <gui/geom/AffineTransform.hpp>

#include <gui/Shape.hpp>
#include <gui/geom/Path2D_Double.hpp>
#include <gui/geom/Point2D_Double.hpp>
#include <gui/geom/Point2D_Float.hpp>
#include <gui/geom/Point2D.hpp>
//#include <java/io/ObjectInputStream.hpp>
//#include <java/io/ObjectOutputStream.hpp>
//#include <lang/Double.hpp>

#include <VecUtil.hpp>

#include <cmath>

#ifdef __APPLE__
#include <cfloat>
#endif

#ifdef __linux__
#include <stdexcept>
#include <cfloat>
#endif

using namespace moduru::gui::geom;
using namespace std;

AffineTransform::AffineTransform(double m00, double m10, double m01, double m11, double m02, double m12, int state)
{
	this->m00 = m00;
	this->m10 = m10;
	this->m01 = m01;
	this->m11 = m11;
	this->m02 = m02;
	this->m12 = m12;
	this->state = state;
	this->type = TYPE_UNKNOWN;
}

AffineTransform::AffineTransform()
{
	m00 = m11 = 1.0;
}

AffineTransform::AffineTransform(AffineTransform* Tx)
{
	this->m00 = Tx->m00;
	this->m10 = Tx->m10;
	this->m01 = Tx->m01;
	this->m11 = Tx->m11;
	this->m02 = Tx->m02;
	this->m12 = Tx->m12;
	this->state = Tx->state;
	this->type = Tx->type;
}

AffineTransform::AffineTransform(float m00, float m10, float m01, float m11, float m02, float m12)
{
	this->m00 = m00;
	this->m10 = m10;
	this->m01 = m01;
	this->m11 = m11;
	this->m02 = m02;
	this->m12 = m12;
	updateState();
}

AffineTransform::AffineTransform(vector<float>* flatmatrix)
{
	m00 = (*flatmatrix)[0];
	m10 = (*flatmatrix)[1];
	m01 = (*flatmatrix)[2];
	m11 = (*flatmatrix)[3];
	if (flatmatrix->size() > 5) {
		m02 = (*flatmatrix)[4];
		m12 = (*flatmatrix)[5];
	}
	updateState();
}

AffineTransform::AffineTransform(double m00, double m10, double m01, double m11, double m02, double m12)
{
	this->m00 = m00;
	this->m10 = m10;
	this->m01 = m01;
	this->m11 = m11;
	this->m02 = m02;
	this->m12 = m12;
	updateState();
}

AffineTransform::AffineTransform(vector<double>* flatmatrix)
{
	m00 = (*flatmatrix)[0];
	m10 = (*flatmatrix)[1];
	m01 = (*flatmatrix)[2];
	m11 = (*flatmatrix)[3];
	if (flatmatrix->size() > 5) {
		m02 = (*flatmatrix)[4];
		m12 = (*flatmatrix)[5];
	}
	updateState();
}

const int AffineTransform::TYPE_UNKNOWN;
const int AffineTransform::TYPE_IDENTITY;
const int AffineTransform::TYPE_TRANSLATION;
const int AffineTransform::TYPE_UNIFORM_SCALE;
const int AffineTransform::TYPE_GENERAL_SCALE;
const int AffineTransform::TYPE_MASK_SCALE;
const int AffineTransform::TYPE_FLIP;
const int AffineTransform::TYPE_QUADRANT_ROTATION;
const int AffineTransform::TYPE_GENERAL_ROTATION;
const int AffineTransform::TYPE_MASK_ROTATION;
const int AffineTransform::TYPE_GENERAL_TRANSFORM;
const int AffineTransform::APPLY_IDENTITY;
const int AffineTransform::APPLY_TRANSLATE;
const int AffineTransform::APPLY_SCALE;
const int AffineTransform::APPLY_SHEAR;
const int AffineTransform::HI_SHIFT;
const int AffineTransform::HI_IDENTITY;
const int AffineTransform::HI_TRANSLATE;
const int AffineTransform::HI_SCALE;
const int AffineTransform::HI_SHEAR;

AffineTransform* AffineTransform::getTranslateInstance(double tx, double ty)
{
    auto Tx = new AffineTransform();
    Tx->setToTranslation(tx, ty);
    return Tx;
}

AffineTransform* AffineTransform::getRotateInstance(double theta)
{
    auto Tx = new AffineTransform();
    Tx->setToRotation(theta);
    return Tx;
}

AffineTransform* AffineTransform::getRotateInstance(double theta, double anchorx, double anchory)
{
    auto Tx = new AffineTransform();
    Tx->setToRotation(theta, anchorx, anchory);
    return Tx;
}

AffineTransform* AffineTransform::getRotateInstance(double vecx, double vecy)
{

    auto Tx = new AffineTransform();
    Tx->setToRotation(vecx, vecy);
    return Tx;
}

AffineTransform* AffineTransform::getRotateInstance(double vecx, double vecy, double anchorx, double anchory)
{

    auto Tx = new AffineTransform();
    Tx->setToRotation(vecx, vecy, anchorx, anchory);
    return Tx;
}

AffineTransform* AffineTransform::getQuadrantRotateInstance(int numquadrants)
{

    auto Tx = new AffineTransform();
    Tx->setToQuadrantRotation(numquadrants);
    return Tx;
}

AffineTransform* AffineTransform::getQuadrantRotateInstance(int numquadrants, double anchorx, double anchory)
{

    auto Tx = new AffineTransform();
    Tx->setToQuadrantRotation(numquadrants, anchorx, anchory);
    return Tx;
}

AffineTransform* AffineTransform::getScaleInstance(double sx, double sy)
{

    auto Tx = new AffineTransform();
    Tx->setToScale(sx, sy);
    return Tx;
}

AffineTransform* AffineTransform::getShearInstance(double shx, double shy)
{

    auto Tx = new AffineTransform();
    Tx->setToShear(shx, shy);
    return Tx;
}

int AffineTransform::getType()
{
    if(type == TYPE_UNKNOWN) {
        calculateType();
    }
    return type;
}

void AffineTransform::calculateType()
{
    auto ret = AffineTransform::TYPE_IDENTITY;
    bool sgn0, sgn1;
    double M0, M1, M2, M3;
    updateState();
    switch (state) {
    default:
        stateError();
    case (APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE):
        ret = TYPE_TRANSLATION;
    case (APPLY_SHEAR | APPLY_SCALE):
        if((M0 = m00) * (M2 = m01) + (M3 = m10) * (M1 = m11) != 0) {
            this->type = TYPE_GENERAL_TRANSFORM;
            return;
        }
        sgn0 = (M0 >= 0.0);
        sgn1 = (M1 >= 0.0);
        if(sgn0 == sgn1) {
            if(M0 != M1 || M2 != -M3) {
                ret |= (TYPE_GENERAL_ROTATION | TYPE_GENERAL_SCALE);
            } else if(M0 * M1 - M2 * M3 != 1.0) {
                ret |= (TYPE_GENERAL_ROTATION | TYPE_UNIFORM_SCALE);
            } else {
                ret |= TYPE_GENERAL_ROTATION;
            }
        } else {
            if(M0 != -M1 || M2 != M3) {
                ret |= (TYPE_GENERAL_ROTATION | TYPE_FLIP | TYPE_GENERAL_SCALE);
            } else if(M0 * M1 - M2 * M3 != 1.0) {
                ret |= (TYPE_GENERAL_ROTATION | TYPE_FLIP | TYPE_UNIFORM_SCALE);
            } else {
                ret |= (TYPE_GENERAL_ROTATION | TYPE_FLIP);
            }
        }
        break;
    case (APPLY_SHEAR | APPLY_TRANSLATE):
        ret = TYPE_TRANSLATION;
    case (APPLY_SHEAR):
        sgn0 = ((M0 = m01) >= 0.0);
        sgn1 = ((M1 = m10) >= 0.0);
        if(sgn0 != sgn1) {
            if(M0 != -M1) {
                ret |= (TYPE_QUADRANT_ROTATION | TYPE_GENERAL_SCALE);
            } else if(M0 != 1.0 && M0 != -1.0) {
                ret |= (TYPE_QUADRANT_ROTATION | TYPE_UNIFORM_SCALE);
            } else {
                ret |= TYPE_QUADRANT_ROTATION;
            }
        } else {
            if(M0 == M1) {
                ret |= (TYPE_QUADRANT_ROTATION | TYPE_FLIP | TYPE_UNIFORM_SCALE);
            } else {
                ret |= (TYPE_QUADRANT_ROTATION | TYPE_FLIP | TYPE_GENERAL_SCALE);
            }
        }
        break;
    case (APPLY_SCALE | APPLY_TRANSLATE):
        ret = TYPE_TRANSLATION;
    case (APPLY_SCALE):
        sgn0 = ((M0 = m00) >= 0.0);
        sgn1 = ((M1 = m11) >= 0.0);
        if(sgn0 == sgn1) {
            if(sgn0) {
                if(M0 == M1) {
                    ret |= TYPE_UNIFORM_SCALE;
                } else {
                    ret |= TYPE_GENERAL_SCALE;
                }
            } else {
                if(M0 != M1) {
                    ret |= (TYPE_QUADRANT_ROTATION | TYPE_GENERAL_SCALE);
                } else if(M0 != -1.0) {
                    ret |= (TYPE_QUADRANT_ROTATION | TYPE_UNIFORM_SCALE);
                } else {
                    ret |= TYPE_QUADRANT_ROTATION;
                }
            }
        } else {
            if(M0 == -M1) {
                if(M0 == 1.0 || M0 == -1.0) {
                    ret |= TYPE_FLIP;
                } else {
                    ret |= (TYPE_FLIP | TYPE_UNIFORM_SCALE);
                }
            } else {
                ret |= (TYPE_FLIP | TYPE_GENERAL_SCALE);
            }
        }
        break;
    case (APPLY_TRANSLATE):
        ret = TYPE_TRANSLATION;
        break;
    case (APPLY_IDENTITY):
        break;
    }

    this->type = ret;
}

double AffineTransform::getDeterminant()
{
    switch (state) {
    default:
        stateError();
    case (APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE):
    case (APPLY_SHEAR | APPLY_SCALE):
        return m00 * m11 - m01 * m10;
    case (APPLY_SHEAR | APPLY_TRANSLATE):
    case (APPLY_SHEAR):
        return -(m01 * m10);
    case (APPLY_SCALE | APPLY_TRANSLATE):
    case (APPLY_SCALE):
        return m00 * m11;
    case (APPLY_TRANSLATE):
    case (APPLY_IDENTITY):
        return 1.0;
    }

}

void AffineTransform::updateState()
{
    if(m01 == 0.0 && m10 == 0.0) {
        if(m00 == 1.0 && m11 == 1.0) {
            if(m02 == 0.0 && m12 == 0.0) {
                state = APPLY_IDENTITY;
                type = TYPE_IDENTITY;
            } else {
                state = APPLY_TRANSLATE;
                type = TYPE_TRANSLATION;
            }
        } else {
            if(m02 == 0.0 && m12 == 0.0) {
                state = APPLY_SCALE;
                type = TYPE_UNKNOWN;
            } else {
                state = (APPLY_SCALE | APPLY_TRANSLATE);
                type = TYPE_UNKNOWN;
            }
        }
    } else {
        if(m00 == 0.0 && m11 == 0.0) {
            if(m02 == 0.0 && m12 == 0.0) {
                state = APPLY_SHEAR;
                type = TYPE_UNKNOWN;
            } else {
                state = (APPLY_SHEAR | APPLY_TRANSLATE);
                type = TYPE_UNKNOWN;
            }
        } else {
            if(m02 == 0.0 && m12 == 0.0) {
                state = (APPLY_SHEAR | APPLY_SCALE);
                type = TYPE_UNKNOWN;
            } else {
                state = (APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE);
                type = TYPE_UNKNOWN;
            }
        }
    }
}

void AffineTransform::stateError()
{
    throw std::invalid_argument("missing case in transform state switch");
}

void AffineTransform::getMatrix(vector<double>* flatmatrix)
{
    (*flatmatrix)[0] = m00;
    (*flatmatrix)[1] = m10;
    (*flatmatrix)[2] = m01;
    (*flatmatrix)[3] = m11;
    if(flatmatrix->size() > 5) {
        (*flatmatrix)[4] = m02;
        (*flatmatrix)[5] = m12;
    }
}

double AffineTransform::getScaleX()
{
    return m00;
}

double AffineTransform::getScaleY()
{
    return m11;
}

double AffineTransform::getShearX()
{
    return m01;
}

double AffineTransform::getShearY()
{
    return m10;
}

double AffineTransform::getTranslateX()
{
    return m02;
}

double AffineTransform::getTranslateY()
{
    return m12;
}

void AffineTransform::translate(double tx, double ty)
{
    switch (state) {
    default:
        stateError();
    case (APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE):
        m02 = tx * m00 + ty * m01 + m02;
        m12 = tx * m10 + ty * m11 + m12;
        if(m02 == 0.0 && m12 == 0.0) {
            state = APPLY_SHEAR | APPLY_SCALE;
            if(type != TYPE_UNKNOWN) {
                type -= TYPE_TRANSLATION;
            }
        }
        return;
    case (APPLY_SHEAR | APPLY_SCALE):
        m02 = tx * m00 + ty * m01;
        m12 = tx * m10 + ty * m11;
        if(m02 != 0.0 || m12 != 0.0) {
            state = APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE;
            type |= TYPE_TRANSLATION;
        }
        return;
    case (APPLY_SHEAR | APPLY_TRANSLATE):
        m02 = ty * m01 + m02;
        m12 = tx * m10 + m12;
        if(m02 == 0.0 && m12 == 0.0) {
            state = APPLY_SHEAR;
            if(type != TYPE_UNKNOWN) {
                type -= TYPE_TRANSLATION;
            }
        }
        return;
    case (APPLY_SHEAR):
        m02 = ty * m01;
        m12 = tx * m10;
        if(m02 != 0.0 || m12 != 0.0) {
            state = APPLY_SHEAR | APPLY_TRANSLATE;
            type |= TYPE_TRANSLATION;
        }
        return;
    case (APPLY_SCALE | APPLY_TRANSLATE):
        m02 = tx * m00 + m02;
        m12 = ty * m11 + m12;
        if(m02 == 0.0 && m12 == 0.0) {
            state = APPLY_SCALE;
            if(type != TYPE_UNKNOWN) {
                type -= TYPE_TRANSLATION;
            }
        }
        return;
    case (APPLY_SCALE):
        m02 = tx * m00;
        m12 = ty * m11;
        if(m02 != 0.0 || m12 != 0.0) {
            state = APPLY_SCALE | APPLY_TRANSLATE;
            type |= TYPE_TRANSLATION;
        }
        return;
    case (APPLY_TRANSLATE):
        m02 = tx + m02;
        m12 = ty + m12;
        if(m02 == 0.0 && m12 == 0.0) {
            state = APPLY_IDENTITY;
            type = TYPE_IDENTITY;
        }
        return;
    case (APPLY_IDENTITY):
        m02 = tx;
        m12 = ty;
        if(tx != 0.0 || ty != 0.0) {
            state = APPLY_TRANSLATE;
            type = TYPE_TRANSLATION;
        }
        return;
    }

}

vector<int> AffineTransform::rot90conversion = vector<int>({ APPLY_SHEAR , APPLY_SHEAR | APPLY_TRANSLATE, APPLY_SHEAR
	, APPLY_SHEAR | APPLY_TRANSLATE, APPLY_SCALE, APPLY_SCALE | APPLY_TRANSLATE, APPLY_SHEAR | APPLY_SCALE, APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE });

void AffineTransform::rotate90()
{
    auto M0 = m00;
    m00 = m01;
    m01 = -M0;
    M0 = m10;
    m10 = m11;
    m11 = -M0;
    auto state = rot90conversion[this->state];
    if((state & (APPLY_SHEAR | APPLY_SCALE)) == APPLY_SCALE && m00 == 1.0 && m11 == 1.0) {
        state -= APPLY_SCALE;
    }
    this->state = state;
    type = TYPE_UNKNOWN;
}

void AffineTransform::rotate180()
{
    m00 = -m00;
    m11 = -m11;
    auto state = this->state;
    if((state & (APPLY_SHEAR)) != 0) {
        m01 = -m01;
        m10 = -m10;
    } else {
        if(m00 == 1.0 && m11 == 1.0) {
            this->state = state & ~APPLY_SCALE;
        } else {
            this->state = state | APPLY_SCALE;
        }
    }
    type = TYPE_UNKNOWN;
}

void AffineTransform::rotate270()
{
    auto M0 = m00;
    m00 = -m01;
    m01 = M0;
    M0 = m10;
    m10 = -m11;
    m11 = M0;
    auto state = rot90conversion[this->state];
    if((state & (APPLY_SHEAR | APPLY_SCALE)) == APPLY_SCALE && m00 == 1.0 && m11 == 1.0) {
        state -= APPLY_SCALE;
    }
    this->state = state;
    type = TYPE_UNKNOWN;
}

void AffineTransform::rotate(double theta)
{
	auto sine = sin(theta);
	if (sine == 1.0) {
		rotate90();
	}
	else if (sine == -1.0) {
		rotate270();
	}
	else {
		auto cosi = cos(theta);
		if (cosi == -1.0) {
			rotate180();
		}
		else if (cosi != 1.0) {
			double M0, M1;
			M0 = m00;
			M1 = m01;
			m00 = cosi * M0 + sine * M1;
			m01 = -sine * M0 + cosi * M1;
			M0 = m10;
			M1 = m11;
			m10 = cosi * M0 + sine * M1;
			m11 = -sine * M0 + cosi * M1;
			updateState();
		}
	}
}

void AffineTransform::rotate(double theta, double anchorx, double anchory)
{
    translate(anchorx, anchory);
    rotate(theta);
    translate(-anchorx, -anchory);
}

void AffineTransform::rotate(double vecx, double vecy)
{
	if (vecy == 0.0) {
		if (vecx < 0.0) {
			rotate180();
		}
	}
	else if (vecx == 0.0) {
		if (vecy > 0.0) {
			rotate90();
		}
		else {
			rotate270();
		}
	}
	else {
		auto len = sqrt(vecx * vecx + vecy * vecy);
		auto sine = vecy / len;
		auto cosi = vecx / len;
		double M0, M1;
		M0 = m00;
		M1 = m01;
		m00 = cosi * M0 + sine * M1;
		m01 = -sine * M0 + cosi * M1;
		M0 = m10;
		M1 = m11;
		m10 = cosi * M0 + sine * M1;
		m11 = -sine * M0 + cosi * M1;
		updateState();
	}
}

void AffineTransform::rotate(double vecx, double vecy, double anchorx, double anchory)
{
    translate(anchorx, anchory);
    rotate(vecx, vecy);
    translate(-anchorx, -anchory);
}

void AffineTransform::quadrantRotate(int numquadrants)
{
    switch (numquadrants & 3) {
    case 0:
        break;
    case 1:
        rotate90();
        break;
    case 2:
        rotate180();
        break;
    case 3:
        rotate270();
        break;
    }

}

void AffineTransform::quadrantRotate(int numquadrants, double anchorx, double anchory)
{
    switch (numquadrants & 3) {
    case 0:
        return;
    case 1:
        m02 += anchorx * (m00 - m01) + anchory * (m01 + m00);
        m12 += anchorx * (m10 - m11) + anchory * (m11 + m10);
        rotate90();
        break;
    case 2:
        m02 += anchorx * (m00 + m00) + anchory * (m01 + m01);
        m12 += anchorx * (m10 + m10) + anchory * (m11 + m11);
        rotate180();
        break;
    case 3:
        m02 += anchorx * (m00 + m01) + anchory * (m01 - m00);
        m12 += anchorx * (m10 + m11) + anchory * (m11 - m10);
        rotate270();
        break;
    }

    if(m02 == 0.0 && m12 == 0.0) {
        state &= ~APPLY_TRANSLATE;
    } else {
        state |= APPLY_TRANSLATE;
    }
}

void AffineTransform::scale(double sx, double sy)
{
    auto state = this->state;
    switch (state) {
    default:
        stateError();
    case (APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE):
    case (APPLY_SHEAR | APPLY_SCALE):
        m00 *= sx;
        m11 *= sy;
    case (APPLY_SHEAR | APPLY_TRANSLATE):
    case (APPLY_SHEAR):
        m01 *= sy;
        m10 *= sx;
        if(m01 == 0 && m10 == 0) {
            state &= APPLY_TRANSLATE;
            if(m00 == 1.0 && m11 == 1.0) {
                this->type = (state == APPLY_IDENTITY ? TYPE_IDENTITY : TYPE_TRANSLATION);
            } else {
                state |= APPLY_SCALE;
                this->type = TYPE_UNKNOWN;
            }
            this->state = state;
        }
        return;
    case (APPLY_SCALE | APPLY_TRANSLATE):
    case (APPLY_SCALE):
        m00 *= sx;
        m11 *= sy;
        if(m00 == 1.0 && m11 == 1.0) {
            this->state = (state &= APPLY_TRANSLATE);
            this->type = (state == APPLY_IDENTITY ? TYPE_IDENTITY : TYPE_TRANSLATION);
        } else {
            this->type = TYPE_UNKNOWN;
        }
        return;
    case (APPLY_TRANSLATE):
    case (APPLY_IDENTITY):
        m00 = sx;
        m11 = sy;
        if(sx != 1.0 || sy != 1.0) {
            this->state = state | APPLY_SCALE;
            this->type = TYPE_UNKNOWN;
        }
        return;
    }

}

void AffineTransform::shear(double shx, double shy)
{
    auto state = this->state;
    {
        double M0;
        double M1;
        switch (state) {
        default:
            stateError();
        case (APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE):
        case (APPLY_SHEAR | APPLY_SCALE):
            M0 = m00;
            M1 = m01;
            m00 = M0 + M1 * shy;
            m01 = M0 * shx + M1;
            M0 = m10;
            M1 = m11;
            m10 = M0 + M1 * shy;
            m11 = M0 * shx + M1;
            updateState();
            return;
        case (APPLY_SHEAR | APPLY_TRANSLATE):
        case (APPLY_SHEAR):
            m00 = m01 * shy;
            m11 = m10 * shx;
            if(m00 != 0.0 || m11 != 0.0) {
                this->state = state | APPLY_SCALE;
            }
            this->type = TYPE_UNKNOWN;
            return;
        case (APPLY_SCALE | APPLY_TRANSLATE):
        case (APPLY_SCALE):
            m01 = m00 * shx;
            m10 = m11 * shy;
            if(m01 != 0.0 || m10 != 0.0) {
                this->state = state | APPLY_SHEAR;
            }
            this->type = TYPE_UNKNOWN;
            return;
        case (APPLY_TRANSLATE):
        case (APPLY_IDENTITY):
            m01 = shx;
            m10 = shy;
            if(m01 != 0.0 || m10 != 0.0) {
                this->state = state | APPLY_SCALE | APPLY_SHEAR;
                this->type = TYPE_UNKNOWN;
            }
            return;
        }
    }

}

void AffineTransform::setToIdentity()
{
    m00 = m11 = 1.0;
    m10 = m01 = m02 = m12 = 0.0;
    state = APPLY_IDENTITY;
    type = TYPE_IDENTITY;
}

void AffineTransform::setToTranslation(double tx, double ty)
{
    m00 = 1.0;
    m10 = 0.0;
    m01 = 0.0;
    m11 = 1.0;
    m02 = tx;
    m12 = ty;
    if(tx != 0.0 || ty != 0.0) {
        state = APPLY_TRANSLATE;
        type = TYPE_TRANSLATION;
    } else {
        state = APPLY_IDENTITY;
        type = TYPE_IDENTITY;
    }
}

void AffineTransform::setToRotation(double theta)
{
    auto sine = sin(theta);
    double cosi;
    if(sine == 1.0 || sine == -1.0) {
        cosi = 0.0;
        state = APPLY_SHEAR;
        type = TYPE_QUADRANT_ROTATION;
	}
	else {
		cosi = cos(theta);
		if (cosi == -1.0) {
			sine = 0.0;
			state = APPLY_SCALE;
			type = TYPE_QUADRANT_ROTATION;
		}
		else if (cosi == 1.0) {
			sine = 0.0;
			state = APPLY_IDENTITY;
			type = TYPE_IDENTITY;
		}
		else {
			state = APPLY_SHEAR | APPLY_SCALE;
			type = TYPE_GENERAL_ROTATION;
		}
	}
    m00 = cosi;
    m10 = sine;
    m01 = -sine;
    m11 = cosi;
    m02 = 0.0;
    m12 = 0.0;
}

void AffineTransform::setToRotation(double theta, double anchorx, double anchory)
{
    setToRotation(theta);
    auto sine = m10;
    auto oneMinusCos = 1.0 - m00;
    m02 = anchorx * oneMinusCos + anchory * sine;
    m12 = anchory * oneMinusCos - anchorx * sine;
    if(m02 != 0.0 || m12 != 0.0) {
        state |= APPLY_TRANSLATE;
        type |= TYPE_TRANSLATION;
    }
}

void AffineTransform::setToRotation(double vecx, double vecy)
{
    double sine, cosi;
    if(vecy == 0) {
        sine = 0.0;
        if(vecx < 0.0) {
            cosi = -1.0;
            state = APPLY_SCALE;
            type = TYPE_QUADRANT_ROTATION;
        } else {
            cosi = 1.0;
            state = APPLY_IDENTITY;
            type = TYPE_IDENTITY;
        }
    } else if(vecx == 0) {
        cosi = 0.0;
        sine = (vecy > 0.0) ? 1.0 : -1.0;
        state = APPLY_SHEAR;
        type = TYPE_QUADRANT_ROTATION;
    } else {
        auto len = sqrt(vecx * vecx + vecy * vecy);
        cosi = vecx / len;
        sine = vecy / len;
        state = APPLY_SHEAR | APPLY_SCALE;
        type = TYPE_GENERAL_ROTATION;
    }
    m00 = cosi;
    m10 = sine;
    m01 = -sine;
    m11 = cosi;
    m02 = 0.0;
    m12 = 0.0;
}

void AffineTransform::setToRotation(double vecx, double vecy, double anchorx, double anchory)
{
    setToRotation(vecx, vecy);
    auto sin = m10;
    auto oneMinusCos = 1.0 - m00;
    m02 = anchorx * oneMinusCos + anchory * sin;
    m12 = anchory * oneMinusCos - anchorx * sin;
    if(m02 != 0.0 || m12 != 0.0) {
        state |= APPLY_TRANSLATE;
        type |= TYPE_TRANSLATION;
    }
}

void AffineTransform::setToQuadrantRotation(int numquadrants)
{
    switch (numquadrants & 3) {
    case 0:
        m00 = 1.0;
        m10 = 0.0;
        m01 = 0.0;
        m11 = 1.0;
        m02 = 0.0;
        m12 = 0.0;
        state = APPLY_IDENTITY;
        type = TYPE_IDENTITY;
        break;
    case 1:
        m00 = 0.0;
        m10 = 1.0;
        m01 = -1.0;
        m11 = 0.0;
        m02 = 0.0;
        m12 = 0.0;
        state = APPLY_SHEAR;
        type = TYPE_QUADRANT_ROTATION;
        break;
    case 2:
        m00 = -1.0;
        m10 = 0.0;
        m01 = 0.0;
        m11 = -1.0;
        m02 = 0.0;
        m12 = 0.0;
        state = APPLY_SCALE;
        type = TYPE_QUADRANT_ROTATION;
        break;
    case 3:
        m00 = 0.0;
        m10 = -1.0;
        m01 = 1.0;
        m11 = 0.0;
        m02 = 0.0;
        m12 = 0.0;
        state = APPLY_SHEAR;
        type = TYPE_QUADRANT_ROTATION;
        break;
    }

}

void AffineTransform::setToQuadrantRotation(int numquadrants, double anchorx, double anchory)
{
    switch (numquadrants & 3) {
    case 0:
        m00 = 1.0;
        m10 = 0.0;
        m01 = 0.0;
        m11 = 1.0;
        m02 = 0.0;
        m12 = 0.0;
        state = APPLY_IDENTITY;
        type = TYPE_IDENTITY;
        break;
    case 1:
        m00 = 0.0;
        m10 = 1.0;
        m01 = -1.0;
        m11 = 0.0;
        m02 = anchorx + anchory;
        m12 = anchory - anchorx;
        if(m02 == 0.0 && m12 == 0.0) {
            state = APPLY_SHEAR;
            type = TYPE_QUADRANT_ROTATION;
        } else {
            state = APPLY_SHEAR | APPLY_TRANSLATE;
            type = TYPE_QUADRANT_ROTATION | TYPE_TRANSLATION;
        }
        break;
    case 2:
        m00 = -1.0;
        m10 = 0.0;
        m01 = 0.0;
        m11 = -1.0;
        m02 = anchorx + anchorx;
        m12 = anchory + anchory;
        if(m02 == 0.0 && m12 == 0.0) {
            state = APPLY_SCALE;
            type = TYPE_QUADRANT_ROTATION;
        } else {
            state = APPLY_SCALE | APPLY_TRANSLATE;
            type = TYPE_QUADRANT_ROTATION | TYPE_TRANSLATION;
        }
        break;
    case 3:
        m00 = 0.0;
        m10 = -1.0;
        m01 = 1.0;
        m11 = 0.0;
        m02 = anchorx - anchory;
        m12 = anchory + anchorx;
        if(m02 == 0.0 && m12 == 0.0) {
            state = APPLY_SHEAR;
            type = TYPE_QUADRANT_ROTATION;
        } else {
            state = APPLY_SHEAR | APPLY_TRANSLATE;
            type = TYPE_QUADRANT_ROTATION | TYPE_TRANSLATION;
        }
        break;
    }

}

void AffineTransform::setToScale(double sx, double sy)
{
    m00 = sx;
    m10 = 0.0;
    m01 = 0.0;
    m11 = sy;
    m02 = 0.0;
    m12 = 0.0;
    if(sx != 1.0 || sy != 1.0) {
        state = APPLY_SCALE;
        type = TYPE_UNKNOWN;
    } else {
        state = APPLY_IDENTITY;
        type = TYPE_IDENTITY;
    }
}

void AffineTransform::setToShear(double shx, double shy)
{
    m00 = 1.0;
    m01 = shx;
    m10 = shy;
    m11 = 1.0;
    m02 = 0.0;
    m12 = 0.0;
    if(shx != 0.0 || shy != 0.0) {
        state = (APPLY_SHEAR | APPLY_SCALE);
        type = TYPE_UNKNOWN;
    } else {
        state = APPLY_IDENTITY;
        type = TYPE_IDENTITY;
    }
}

void AffineTransform::setTransform(AffineTransform* Tx)
{
	this->m00 = Tx->m00;
	this->m10 = Tx->m10;
	this->m01 = Tx->m01;
	this->m11 = Tx->m11;
	this->m02 = Tx->m02;
	this->m12 = Tx->m12;
	this->state = Tx->state;
	this->type = Tx->type;
}

void AffineTransform::setTransform(double m00, double m10, double m01, double m11, double m02, double m12)
{
	this->m00 = m00;
	this->m10 = m10;
	this->m01 = m01;
	this->m11 = m11;
	this->m02 = m02;
	this->m12 = m12;
	updateState();
}

void AffineTransform::concatenate(AffineTransform* Tx)
{
    double M0, M1;
    double T00, T01, T10, T11;
    double T02, T12;
    auto mystate = state;
    auto txstate = Tx->state;
    switch ((txstate << HI_SHIFT) | mystate) {
    case (HI_IDENTITY | APPLY_IDENTITY):
    case (HI_IDENTITY | APPLY_TRANSLATE):
    case (HI_IDENTITY | APPLY_SCALE):
    case (HI_IDENTITY | APPLY_SCALE | APPLY_TRANSLATE):
    case (HI_IDENTITY | APPLY_SHEAR):
    case (HI_IDENTITY | APPLY_SHEAR | APPLY_TRANSLATE):
    case (HI_IDENTITY | APPLY_SHEAR | APPLY_SCALE):
    case (HI_IDENTITY | APPLY_SHEAR | APPLY_SCALE| APPLY_TRANSLATE):
        return;
    case (HI_SHEAR | HI_SCALE | HI_TRANSLATE| APPLY_IDENTITY):
        m01 = Tx->m01;
        m10 = Tx->m10;
    case (HI_SCALE | HI_TRANSLATE | APPLY_IDENTITY):
        m00 = Tx->m00;
        m11 = Tx->m11;
    case (HI_TRANSLATE | APPLY_IDENTITY):
        m02 = Tx->m02;
        m12 = Tx->m12;
        state = txstate;
        type = Tx->type;
        return;
    case (HI_SHEAR | HI_SCALE | APPLY_IDENTITY):
        m01 = Tx->m01;
        m10 = Tx->m10;
    case (HI_SCALE | APPLY_IDENTITY):
        m00 = Tx->m00;
        m11 = Tx->m11;
        state = txstate;
        type = Tx->type;
        return;
    case (HI_SHEAR | HI_TRANSLATE | APPLY_IDENTITY):
        m02 = Tx->m02;
        m12 = Tx->m12;
    case (HI_SHEAR | APPLY_IDENTITY):
        m01 = Tx->m01;
        m10 = Tx->m10;
        m00 = m11 = 0.0;
        state = txstate;
        type = Tx->type;
        return;
    case (HI_TRANSLATE | APPLY_SHEAR | APPLY_SCALE| APPLY_TRANSLATE):
    case (HI_TRANSLATE | APPLY_SHEAR | APPLY_SCALE):
    case (HI_TRANSLATE | APPLY_SHEAR | APPLY_TRANSLATE):
    case (HI_TRANSLATE | APPLY_SHEAR):
    case (HI_TRANSLATE | APPLY_SCALE | APPLY_TRANSLATE):
    case (HI_TRANSLATE | APPLY_SCALE):
    case (HI_TRANSLATE | APPLY_TRANSLATE):
        translate(Tx->m02, Tx->m12);
        return;
    case (HI_SCALE | APPLY_SHEAR | APPLY_SCALE| APPLY_TRANSLATE):
    case (HI_SCALE | APPLY_SHEAR | APPLY_SCALE):
    case (HI_SCALE | APPLY_SHEAR | APPLY_TRANSLATE):
    case (HI_SCALE | APPLY_SHEAR):
    case (HI_SCALE | APPLY_SCALE | APPLY_TRANSLATE):
    case (HI_SCALE | APPLY_SCALE):
    case (HI_SCALE | APPLY_TRANSLATE):
        scale(Tx->m00, Tx->m11);
        return;
    case (HI_SHEAR | APPLY_SHEAR | APPLY_SCALE| APPLY_TRANSLATE):
    case (HI_SHEAR | APPLY_SHEAR | APPLY_SCALE):
        T01 = Tx->m01;
        T10 = Tx->m10;
        M0 = m00;
        m00 = m01 * T10;
        m01 = M0 * T01;
        M0 = m10;
        m10 = m11 * T10;
        m11 = M0 * T01;
        type = TYPE_UNKNOWN;
        return;
    case (HI_SHEAR | APPLY_SHEAR | APPLY_TRANSLATE):
    case (HI_SHEAR | APPLY_SHEAR):
        m00 = m01 * Tx->m10;
        m01 = 0.0;
        m11 = m10 * Tx->m01;
        m10 = 0.0;
        state = mystate ^ (APPLY_SHEAR | APPLY_SCALE);
        type = TYPE_UNKNOWN;
        return;
    case (HI_SHEAR | APPLY_SCALE | APPLY_TRANSLATE):
    case (HI_SHEAR | APPLY_SCALE):
        m01 = m00 * Tx->m01;
        m00 = 0.0;
        m10 = m11 * Tx->m10;
        m11 = 0.0;
        state = mystate ^ (APPLY_SHEAR | APPLY_SCALE);
        type = TYPE_UNKNOWN;
        return;
    case (HI_SHEAR | APPLY_TRANSLATE):
        m00 = 0.0;
        m01 = Tx->m01;
        m10 = Tx->m10;
        m11 = 0.0;
        state = APPLY_TRANSLATE | APPLY_SHEAR;
        type = TYPE_UNKNOWN;
        return;
    }

    T00 = Tx->m00;
    T01 = Tx->m01;
    T02 = Tx->m02;
    T10 = Tx->m10;
    T11 = Tx->m11;
    T12 = Tx->m12;
    switch (mystate) {
    default:
        stateError();
    case (APPLY_SHEAR | APPLY_SCALE):
        state = mystate | txstate;
    case (APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE):
        M0 = m00;
        M1 = m01;
        m00 = T00 * M0 + T10 * M1;
        m01 = T01 * M0 + T11 * M1;
        m02 += T02 * M0 + T12 * M1;
        M0 = m10;
        M1 = m11;
        m10 = T00 * M0 + T10 * M1;
        m11 = T01 * M0 + T11 * M1;
        m12 += T02 * M0 + T12 * M1;
        type = TYPE_UNKNOWN;
        return;
    case (APPLY_SHEAR | APPLY_TRANSLATE):
    case (APPLY_SHEAR):
        M0 = m01;
        m00 = T10 * M0;
        m01 = T11 * M0;
        m02 += T12 * M0;
        M0 = m10;
        m10 = T00 * M0;
        m11 = T01 * M0;
        m12 += T02 * M0;
        break;
    case (APPLY_SCALE | APPLY_TRANSLATE):
    case (APPLY_SCALE):
        M0 = m00;
        m00 = T00 * M0;
        m01 = T01 * M0;
        m02 += T02 * M0;
        M0 = m11;
        m10 = T10 * M0;
        m11 = T11 * M0;
        m12 += T12 * M0;
        break;
    case (APPLY_TRANSLATE):
        m00 = T00;
        m01 = T01;
        m02 += T02;
        m10 = T10;
        m11 = T11;
        m12 += T12;
        state = txstate | APPLY_TRANSLATE;
        type = TYPE_UNKNOWN;
        return;
    }

    updateState();
}

void AffineTransform::preConcatenate(AffineTransform* Tx)
{
    double M0, M1;
    double T00, T01, T10, T11;
    double T02, T12;
    auto mystate = state;
    auto txstate = Tx->state;
    switch ((txstate << HI_SHIFT) | mystate) {
    case (HI_IDENTITY | APPLY_IDENTITY):
    case (HI_IDENTITY | APPLY_TRANSLATE):
    case (HI_IDENTITY | APPLY_SCALE):
    case (HI_IDENTITY | APPLY_SCALE | APPLY_TRANSLATE):
    case (HI_IDENTITY | APPLY_SHEAR):
    case (HI_IDENTITY | APPLY_SHEAR | APPLY_TRANSLATE):
    case (HI_IDENTITY | APPLY_SHEAR | APPLY_SCALE):
    case (HI_IDENTITY | APPLY_SHEAR | APPLY_SCALE| APPLY_TRANSLATE):
        return;
    case (HI_TRANSLATE | APPLY_IDENTITY):
    case (HI_TRANSLATE | APPLY_SCALE):
    case (HI_TRANSLATE | APPLY_SHEAR):
    case (HI_TRANSLATE | APPLY_SHEAR | APPLY_SCALE):
        m02 = Tx->m02;
        m12 = Tx->m12;
        state = mystate | APPLY_TRANSLATE;
        type |= TYPE_TRANSLATION;
        return;
    case (HI_TRANSLATE | APPLY_TRANSLATE):
    case (HI_TRANSLATE | APPLY_SCALE | APPLY_TRANSLATE):
    case (HI_TRANSLATE | APPLY_SHEAR | APPLY_TRANSLATE):
    case (HI_TRANSLATE | APPLY_SHEAR | APPLY_SCALE| APPLY_TRANSLATE):
        m02 = m02 + Tx->m02;
        m12 = m12 + Tx->m12;
        return;
    case (HI_SCALE | APPLY_TRANSLATE):
    case (HI_SCALE | APPLY_IDENTITY):
        state = mystate | APPLY_SCALE;
    case (HI_SCALE | APPLY_SHEAR | APPLY_SCALE| APPLY_TRANSLATE):
    case (HI_SCALE | APPLY_SHEAR | APPLY_SCALE):
    case (HI_SCALE | APPLY_SHEAR | APPLY_TRANSLATE):
    case (HI_SCALE | APPLY_SHEAR):
    case (HI_SCALE | APPLY_SCALE | APPLY_TRANSLATE):
    case (HI_SCALE | APPLY_SCALE):
        T00 = Tx->m00;
        T11 = Tx->m11;
        if((mystate & APPLY_SHEAR) != 0) {
            m01 = m01 * T00;
            m10 = m10 * T11;
            if((mystate & APPLY_SCALE) != 0) {
                m00 = m00 * T00;
                m11 = m11 * T11;
            }
        } else {
            m00 = m00 * T00;
            m11 = m11 * T11;
        }
        if((mystate & APPLY_TRANSLATE) != 0) {
            m02 = m02 * T00;
            m12 = m12 * T11;
        }
        type = TYPE_UNKNOWN;
        return;
    case (HI_SHEAR | APPLY_SHEAR | APPLY_TRANSLATE):
    case (HI_SHEAR | APPLY_SHEAR):
        mystate = mystate | APPLY_SCALE;
    case (HI_SHEAR | APPLY_TRANSLATE):
    case (HI_SHEAR | APPLY_IDENTITY):
    case (HI_SHEAR | APPLY_SCALE | APPLY_TRANSLATE):
    case (HI_SHEAR | APPLY_SCALE):
        state = mystate ^ APPLY_SHEAR;
    case (HI_SHEAR | APPLY_SHEAR | APPLY_SCALE| APPLY_TRANSLATE):
    case (HI_SHEAR | APPLY_SHEAR | APPLY_SCALE):
        T01 = Tx->m01;
        T10 = Tx->m10;
        M0 = m00;
        m00 = m10 * T01;
        m10 = M0 * T10;
        M0 = m01;
        m01 = m11 * T01;
        m11 = M0 * T10;
        M0 = m02;
        m02 = m12 * T01;
        m12 = M0 * T10;
        type = TYPE_UNKNOWN;
        return;
    }

    T00 = Tx->m00;
    T01 = Tx->m01;
    T02 = Tx->m02;
    T10 = Tx->m10;
    T11 = Tx->m11;
    T12 = Tx->m12;
    switch (mystate) {
    default:
        stateError();
    case (APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE):
        M0 = m02;
        M1 = m12;
        T02 += M0 * T00 + M1 * T01;
        T12 += M0 * T10 + M1 * T11;
    case (APPLY_SHEAR | APPLY_SCALE):
        m02 = T02;
        m12 = T12;
        M0 = m00;
        M1 = m10;
        m00 = M0 * T00 + M1 * T01;
        m10 = M0 * T10 + M1 * T11;
        M0 = m01;
        M1 = m11;
        m01 = M0 * T00 + M1 * T01;
        m11 = M0 * T10 + M1 * T11;
        break;
    case (APPLY_SHEAR | APPLY_TRANSLATE):
        M0 = m02;
        M1 = m12;
        T02 += M0 * T00 + M1 * T01;
        T12 += M0 * T10 + M1 * T11;
    case (APPLY_SHEAR):
        m02 = T02;
        m12 = T12;
        M0 = m10;
        m00 = M0 * T01;
        m10 = M0 * T11;
        M0 = m01;
        m01 = M0 * T00;
        m11 = M0 * T10;
        break;
    case (APPLY_SCALE | APPLY_TRANSLATE):
        M0 = m02;
        M1 = m12;
        T02 += M0 * T00 + M1 * T01;
        T12 += M0 * T10 + M1 * T11;
    case (APPLY_SCALE):
        m02 = T02;
        m12 = T12;
        M0 = m00;
        m00 = M0 * T00;
        m10 = M0 * T10;
        M0 = m11;
        m01 = M0 * T01;
        m11 = M0 * T11;
        break;
    case (APPLY_TRANSLATE):
        M0 = m02;
        M1 = m12;
        T02 += M0 * T00 + M1 * T01;
        T12 += M0 * T10 + M1 * T11;
    case (APPLY_IDENTITY):
        m02 = T02;
        m12 = T12;
        m00 = T00;
        m10 = T10;
        m01 = T01;
        m11 = T11;
        state = mystate | txstate;
        type = TYPE_UNKNOWN;
        return;
    }

    updateState();
}

AffineTransform* AffineTransform::createInverse()
{
    double det;
    switch (state) {
    default:
        stateError();
    case (APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE):
        det = m00 * m11 - m01 * m10;
        if(abs(det) <= DBL_MIN) {
            //throw new ::NoninvertibleTransformException(::StringBuilder().append("Determinant is ")->append(det)->toString());
			throw std::invalid_argument("Non-invertable");
        }
        return new AffineTransform(m11 / det, -m10 / det, -m01 / det, m00 / det, (m01 * m12 - m11 * m02) / det, (m10 * m02 - m00 * m12) / det, (APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE));
    case (APPLY_SHEAR | APPLY_SCALE):
        det = m00 * m11 - m01 * m10;
        if(abs(det) <= DBL_MIN) {
            //throw new ::NoninvertibleTransformException(::StringBuilder().append("Determinant is ")->append(det)->toString());
			throw std::invalid_argument("Non-invertable");
        }
        return new AffineTransform(m11 / det, -m10 / det, -m01 / det, m00 / det, 0.0, 0.0, (APPLY_SHEAR | APPLY_SCALE));
    case (APPLY_SHEAR | APPLY_TRANSLATE):
        if(m01 == 0.0 || m10 == 0.0) {
            //throw new ::NoninvertibleTransformException("Determinant is 0");
			throw std::invalid_argument("Non-invertable");
        }
        return new AffineTransform(0.0, 1.0 / m01, 1.0 / m10, 0.0, -m12 / m10, -m02 / m01, (APPLY_SHEAR | APPLY_TRANSLATE));
    case (APPLY_SHEAR):
        if(m01 == 0.0 || m10 == 0.0) {
            //throw new ::NoninvertibleTransformException("Determinant is 0");
			throw std::invalid_argument("Non-invertable");
        }
        return new AffineTransform(0.0, 1.0 / m01, 1.0 / m10, 0.0, 0.0, 0.0, (APPLY_SHEAR));
    case (APPLY_SCALE | APPLY_TRANSLATE):
        if(m00 == 0.0 || m11 == 0.0) {
            //throw new ::NoninvertibleTransformException("Determinant is 0");
			throw std::invalid_argument("Non-invertable");
        }
        return new AffineTransform(1.0 / m00, 0.0, 0.0, 1.0 / m11, -m02 / m00, -m12 / m11, (APPLY_SCALE | APPLY_TRANSLATE));
    case (APPLY_SCALE):
        if(m00 == 0.0 || m11 == 0.0) {
            //throw new ::NoninvertibleTransformException("Determinant is 0");
			throw std::invalid_argument("Non-invertable");
        }
        return new AffineTransform(1.0 / m00, 0.0, 0.0, 1.0 / m11, 0.0, 0.0, (APPLY_SCALE));
    case (APPLY_TRANSLATE):
        return new AffineTransform(1.0, 0.0, 0.0, 1.0, -m02, -m12, (APPLY_TRANSLATE));
    case (APPLY_IDENTITY):
        return new AffineTransform();
    }

}

void AffineTransform::invert()
{
	double M00, M01, M02;
	double M10, M11, M12;
	double det;
	switch (state) {
	default:
		stateError();
	case (APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE):
		M00 = m00;
		M01 = m01;
		M02 = m02;
		M10 = m10;
		M11 = m11;
		M12 = m12;
		det = M00 * M11 - M01 * M10;
		if (abs(det) <= DBL_MIN) {
			//throw new ::NoninvertibleTransformException(::StringBuilder().append("Determinant is ")->append(det)->toString());
			throw std::invalid_argument("Non-invertable");
		}
		m00 = M11 / det;
		m10 = -M10 / det;
		m01 = -M01 / det;
		m11 = M00 / det;
		m02 = (M01 * M12 - M11 * M02) / det;
		m12 = (M10 * M02 - M00 * M12) / det;
		break;
	case (APPLY_SHEAR | APPLY_SCALE):
		M00 = m00;
		M01 = m01;
		M10 = m10;
		M11 = m11;
		det = M00 * M11 - M01 * M10;
		if (abs(det) <= DBL_MIN) {
			//throw new ::NoninvertibleTransformException(::StringBuilder().append("Determinant is ")->append(det)->toString());
			throw std::invalid_argument("Non-invertable");
		}
		m00 = M11 / det;
		m10 = -M10 / det;
		m01 = -M01 / det;
		m11 = M00 / det;
		break;
	case (APPLY_SHEAR | APPLY_TRANSLATE):
		M01 = m01;
		M02 = m02;
		M10 = m10;
		M12 = m12;
		if (M01 == 0.0 || M10 == 0.0) {
			//throw new ::NoninvertibleTransformException("Determinant is 0");
			throw std::invalid_argument("Non-invertable");
		}
		m10 = 1.0 / M01;
		m01 = 1.0 / M10;
		m02 = -M12 / M10;
		m12 = -M02 / M01;
		break;
	case (APPLY_SHEAR):
		M01 = m01;
		M10 = m10;
		if (M01 == 0.0 || M10 == 0.0) {
			//throw new ::NoninvertibleTransformException("Determinant is 0");
			throw std::invalid_argument("Non-invertable");
		}
		m10 = 1.0 / M01;
		m01 = 1.0 / M10;
		break;
	case (APPLY_SCALE | APPLY_TRANSLATE):
		M00 = m00;
		M02 = m02;
		M11 = m11;
		M12 = m12;
		if (M00 == 0.0 || M11 == 0.0) {
			//throw new ::NoninvertibleTransformException("Determinant is 0");
			throw std::invalid_argument("Non-invertable");
		}
		m00 = 1.0 / M00;
		m11 = 1.0 / M11;
		m02 = -M02 / M00;
		m12 = -M12 / M11;
		break;
	case (APPLY_SCALE):
		M00 = m00;
		M11 = m11;
		if (M00 == 0.0 || M11 == 0.0) {
			//throw new ::NoninvertibleTransformException("Determinant is 0");
			throw std::invalid_argument("Non-invertable");
		}
		m00 = 1.0 / M00;
		m11 = 1.0 / M11;
		break;
	case (APPLY_TRANSLATE):
		m02 = -m02;
		m12 = -m12;
		break;
	case (APPLY_IDENTITY):
		break;
	}

}

Point2D* AffineTransform::transform(::Point2D* ptSrc, ::Point2D* ptDst)
{
    if(ptDst == nullptr) {
        if(dynamic_cast< ::Point2D_Double* >(ptSrc) != nullptr) {
            ptDst = new ::Point2D_Double();
        } else {
            ptDst = new ::Point2D_Float();
        }
    }
    auto x = ptSrc->getX();
    auto y = ptSrc->getY();
    switch (state) {
    default:
        stateError();
    case (APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE):
        ptDst->setLocation(x * m00 + y * m01 + m02, x * m10 + y * m11 + m12);
        return ptDst;
    case (APPLY_SHEAR | APPLY_SCALE):
        ptDst->setLocation(x * m00 + y * m01, x * m10 + y * m11);
        return ptDst;
    case (APPLY_SHEAR | APPLY_TRANSLATE):
        ptDst->setLocation(y * m01 + m02, x * m10 + m12);
        return ptDst;
    case (APPLY_SHEAR):
        ptDst->setLocation(y * m01, x * m10);
        return ptDst;
    case (APPLY_SCALE | APPLY_TRANSLATE):
        ptDst->setLocation(x * m00 + m02, y * m11 + m12);
        return ptDst;
    case (APPLY_SCALE):
        ptDst->setLocation(x * m00, y * m11);
        return ptDst;
    case (APPLY_TRANSLATE):
        ptDst->setLocation(x + m02, y + m12);
        return ptDst;
    case (APPLY_IDENTITY):
        ptDst->setLocation(x, y);
        return ptDst;
    }

}

void AffineTransform::transform(vector<moduru::gui::geom::Point2D*> ptSrc, int srcOff, vector<moduru::gui::geom::Point2D*>* ptDst, int dstOff, int numPts)
{
    auto state = this->state;
    while (--numPts >= 0) {
        auto src = ptSrc[srcOff++];
        auto x = src->getX();
        auto y = src->getY();
        auto dst = (*ptDst)[dstOff++];
		if (dst == nullptr) {
			if (dynamic_cast<::Point2D_Double*>(src) != nullptr) {
				dst = new moduru::gui::geom::Point2D_Double();
			}
			else {
				dst = new moduru::gui::geom::Point2D_Float();
			}
			(*ptDst)[dstOff - 1] = dst;
		}
        switch (state) {
        default:
            stateError();
        case (APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE):
            dst->setLocation(x * m00 + y * m01 + m02, x * m10 + y * m11 + m12);
            break;
        case (APPLY_SHEAR | APPLY_SCALE):
            dst->setLocation(x * m00 + y * m01, x * m10 + y * m11);
            break;
        case (APPLY_SHEAR | APPLY_TRANSLATE):
            dst->setLocation(y * m01 + m02, x * m10 + m12);
            break;
        case (APPLY_SHEAR):
            dst->setLocation(y * m01, x * m10);
            break;
        case (APPLY_SCALE | APPLY_TRANSLATE):
            dst->setLocation(x * m00 + m02, y * m11 + m12);
            break;
        case (APPLY_SCALE):
            dst->setLocation(x * m00, y * m11);
            break;
        case (APPLY_TRANSLATE):
            dst->setLocation(x + m02, y + m12);
            break;
        case (APPLY_IDENTITY):
            dst->setLocation(x, y);
            break;
        }

    }
}

void AffineTransform::transform(vector<float> srcPts, int srcOff, vector<float>* dstPts, int dstOff, int numPts)
{
	double M00, M01, M02, M10, M11, M12;
	if (dstPts == &srcPts && dstOff > srcOff && dstOff < srcOff + numPts * 2) {
		moduru::VecUtil::VecCopy(&srcPts, srcOff, dstPts, dstOff, numPts * 2);
		srcOff = dstOff;
	}
	switch (state) {
	default:
		stateError();
	case (APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE):
		M00 = m00;
		M01 = m01;
		M02 = m02;
		M10 = m10;
		M11 = m11;
		M12 = m12;
		while (--numPts >= 0) {
			double x = srcPts[srcOff++];
			double y = srcPts[srcOff++];
			(*dstPts)[dstOff++] = static_cast<float>((M00 * x + M01 * y + M02));
			(*dstPts)[dstOff++] = static_cast<float>((M10 * x + M11 * y + M12));
		}
		return;
	case (APPLY_SHEAR | APPLY_SCALE):
		M00 = m00;
		M01 = m01;
		M10 = m10;
		M11 = m11;
		while (--numPts >= 0) {
			double x = srcPts[srcOff++];
			double y = srcPts[srcOff++];
			(*dstPts)[dstOff++] = static_cast<float>((M00 * x + M01 * y));
			(*dstPts)[dstOff++] = static_cast<float>((M10 * x + M11 * y));
		}
		return;
	case (APPLY_SHEAR | APPLY_TRANSLATE):
		M01 = m01;
		M02 = m02;
		M10 = m10;
		M12 = m12;
		while (--numPts >= 0) {
			double x = srcPts[srcOff++];
			(*dstPts)[dstOff++] = static_cast<float>((M01 * srcPts[srcOff++] + M02));
			(*dstPts)[dstOff++] = static_cast<float>((M10 * x + M12));
		}
		return;
	case (APPLY_SHEAR):
		M01 = m01;
		M10 = m10;
		while (--numPts >= 0) {
			double x = srcPts[srcOff++];
			(*dstPts)[dstOff++] = static_cast<float>((M01 * srcPts[srcOff++]));
			(*dstPts)[dstOff++] = static_cast<float>((M10 * x));
		}
		return;
	case (APPLY_SCALE | APPLY_TRANSLATE):
		M00 = m00;
		M02 = m02;
		M11 = m11;
		M12 = m12;
		while (--numPts >= 0) {
			(*dstPts)[dstOff++] = static_cast<float>((M00 * srcPts[srcOff++] + M02));
			(*dstPts)[dstOff++] = static_cast<float>((M11 * srcPts[srcOff++] + M12));
		}
		return;
	case (APPLY_SCALE):
		M00 = m00;
		M11 = m11;
		while (--numPts >= 0) {
			(*dstPts)[dstOff++] = static_cast<float>((M00 * srcPts[srcOff++]));
			(*dstPts)[dstOff++] = static_cast<float>((M11 * srcPts[srcOff++]));
		}
		return;
	case (APPLY_TRANSLATE):
		M02 = m02;
		M12 = m12;
		while (--numPts >= 0) {
			(*dstPts)[dstOff++] = static_cast<float>((srcPts[srcOff++] + M02));
			(*dstPts)[dstOff++] = static_cast<float>((srcPts[srcOff++] + M12));
		}
		return;
	case (APPLY_IDENTITY):
		if (&srcPts != dstPts || srcOff != dstOff) {
			moduru::VecUtil::VecCopy(&srcPts, srcOff, dstPts, dstOff, numPts * 2);
		}
		return;
	}
}

void AffineTransform::transform(vector<double> srcPts, int srcOff, vector<double>* dstPts, int dstOff, int numPts)
{
    double M00, M01, M02, M10, M11, M12;
    if(dstPts == &srcPts && dstOff > srcOff && dstOff < srcOff + numPts * 2) {
        moduru::VecUtil::VecCopy(&srcPts, srcOff, dstPts, dstOff, numPts * 2);
        srcOff = dstOff;
    }
    switch (state) {
    default:
        stateError();
    case (APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE):
        M00 = m00;
        M01 = m01;
        M02 = m02;
        M10 = m10;
        M11 = m11;
        M12 = m12;
        while (--numPts >= 0) {
            auto x = srcPts[srcOff++];
            auto y = srcPts[srcOff++];
            (*dstPts)[dstOff++] = M00 * x + M01 * y + M02;
            (*dstPts)[dstOff++] = M10 * x + M11 * y + M12;
        }
        return;
    case (APPLY_SHEAR | APPLY_SCALE):
        M00 = m00;
        M01 = m01;
        M10 = m10;
        M11 = m11;
        while (--numPts >= 0) {
            auto x = srcPts[srcOff++];
            auto y = srcPts[srcOff++];
            (*dstPts)[dstOff++] = M00 * x + M01 * y;
            (*dstPts)[dstOff++] = M10 * x + M11 * y;
        }
        return;
    case (APPLY_SHEAR | APPLY_TRANSLATE):
        M01 = m01;
        M02 = m02;
        M10 = m10;
        M12 = m12;
        while (--numPts >= 0) {
            auto x = srcPts[srcOff++];
            (*dstPts)[dstOff++] = M01 * srcPts[srcOff++] + M02;
            (*dstPts)[dstOff++] = M10 * x + M12;
        }
        return;
    case (APPLY_SHEAR):
        M01 = m01;
        M10 = m10;
        while (--numPts >= 0) {
            auto x = srcPts[srcOff++];
            (*dstPts)[dstOff++] = M01 * srcPts[srcOff++];
            (*dstPts)[dstOff++] = M10 * x;
        }
        return;
    case (APPLY_SCALE | APPLY_TRANSLATE):
        M00 = m00;
        M02 = m02;
        M11 = m11;
        M12 = m12;
        while (--numPts >= 0) {
            (*dstPts)[dstOff++] = M00 * srcPts[srcOff++] + M02;
            (*dstPts)[dstOff++] = M11 * srcPts[srcOff++] + M12;
        }
        return;
    case (APPLY_SCALE):
        M00 = m00;
        M11 = m11;
        while (--numPts >= 0) {
            (*dstPts)[dstOff++] = M00 * srcPts[srcOff++];
            (*dstPts)[dstOff++] = M11 * srcPts[srcOff++];
        }
        return;
    case (APPLY_TRANSLATE):
        M02 = m02;
        M12 = m12;
        while (--numPts >= 0) {
            (*dstPts)[dstOff++] = srcPts[srcOff++] + M02;
            (*dstPts)[dstOff++] = srcPts[srcOff++] + M12;
        }
        return;
    case (APPLY_IDENTITY):
        if(&srcPts != dstPts || srcOff != dstOff) {
            moduru::VecUtil::VecCopy(&srcPts, srcOff, dstPts, dstOff, numPts * 2);
        }
        return;
    }

}

void AffineTransform::transform(vector<float> srcPts, int srcOff, vector<double>* dstPts, int dstOff, int numPts)
{
    double M00, M01, M02, M10, M11, M12;
    switch (state) {
    default:
        stateError();
    case (APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE):
        M00 = m00;
        M01 = m01;
        M02 = m02;
        M10 = m10;
        M11 = m11;
        M12 = m12;
        while (--numPts >= 0) {
            double x = srcPts[srcOff++];
            double y = srcPts[srcOff++];
            (*dstPts)[dstOff++] = M00 * x + M01 * y + M02;
            (*dstPts)[dstOff++] = M10 * x + M11 * y + M12;
        }
        return;
    case (APPLY_SHEAR | APPLY_SCALE):
        M00 = m00;
        M01 = m01;
        M10 = m10;
        M11 = m11;
        while (--numPts >= 0) {
            double x = srcPts[srcOff++];
            double y = srcPts[srcOff++];
            (*dstPts)[dstOff++] = M00 * x + M01 * y;
            (*dstPts)[dstOff++] = M10 * x + M11 * y;
        }
        return;
    case (APPLY_SHEAR | APPLY_TRANSLATE):
        M01 = m01;
        M02 = m02;
        M10 = m10;
        M12 = m12;
        while (--numPts >= 0) {
            double x = srcPts[srcOff++];
            (*dstPts)[dstOff++] = M01 * srcPts[srcOff++] + M02;
            (*dstPts)[dstOff++] = M10 * x + M12;
        }
        return;
    case (APPLY_SHEAR):
        M01 = m01;
        M10 = m10;
        while (--numPts >= 0) {
            double x = srcPts[srcOff++];
            (*dstPts)[dstOff++] = M01 * srcPts[srcOff++];
            (*dstPts)[dstOff++] = M10 * x;
        }
        return;
    case (APPLY_SCALE | APPLY_TRANSLATE):
        M00 = m00;
        M02 = m02;
        M11 = m11;
        M12 = m12;
        while (--numPts >= 0) {
            (*dstPts)[dstOff++] = M00 * srcPts[srcOff++] + M02;
            (*dstPts)[dstOff++] = M11 * srcPts[srcOff++] + M12;
        }
        return;
    case (APPLY_SCALE):
        M00 = m00;
        M11 = m11;
        while (--numPts >= 0) {
            (*dstPts)[dstOff++] = M00 * srcPts[srcOff++];
            (*dstPts)[dstOff++] = M11 * srcPts[srcOff++];
        }
        return;
    case (APPLY_TRANSLATE):
        M02 = m02;
        M12 = m12;
        while (--numPts >= 0) {
            (*dstPts)[dstOff++] = srcPts[srcOff++] + M02;
            (*dstPts)[dstOff++] = srcPts[srcOff++] + M12;
        }
        return;
    case (APPLY_IDENTITY):
        while (--numPts >= 0) {
            (*dstPts)[dstOff++] = srcPts[srcOff++];
            (*dstPts)[dstOff++] = srcPts[srcOff++];
        }
        return;
    }

}

void AffineTransform::transform(vector<double> srcPts, int srcOff, vector<float>* dstPts, int dstOff, int numPts)
{
    double M00, M01, M02, M10, M11, M12;
    switch (state) {
    default:
        stateError();
    case (APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE):
        M00 = m00;
        M01 = m01;
        M02 = m02;
        M10 = m10;
        M11 = m11;
        M12 = m12;
        while (--numPts >= 0) {
            auto x = srcPts[srcOff++];
            auto y = srcPts[srcOff++];
            (*dstPts)[dstOff++] = static_cast< float >((M00 * x + M01 * y + M02));
            (*dstPts)[dstOff++] = static_cast< float >((M10 * x + M11 * y + M12));
        }
        return;
    case (APPLY_SHEAR | APPLY_SCALE):
        M00 = m00;
        M01 = m01;
        M10 = m10;
        M11 = m11;
        while (--numPts >= 0) {
            auto x = srcPts[srcOff++];
            auto y = srcPts[srcOff++];
            (*dstPts)[dstOff++] = static_cast< float >((M00 * x + M01 * y));
            (*dstPts)[dstOff++] = static_cast< float >((M10 * x + M11 * y));
        }
        return;
    case (APPLY_SHEAR | APPLY_TRANSLATE):
        M01 = m01;
        M02 = m02;
        M10 = m10;
        M12 = m12;
        while (--numPts >= 0) {
            auto x = srcPts[srcOff++];
            (*dstPts)[dstOff++] = static_cast< float >((M01 * srcPts[srcOff++] + M02));
            (*dstPts)[dstOff++] = static_cast< float >((M10 * x + M12));
        }
        return;
    case (APPLY_SHEAR):
        M01 = m01;
        M10 = m10;
        while (--numPts >= 0) {
            auto x = srcPts[srcOff++];
            (*dstPts)[dstOff++] = static_cast< float >((M01 * srcPts[srcOff++]));
            (*dstPts)[dstOff++] = static_cast< float >((M10 * x));
        }
        return;
    case (APPLY_SCALE | APPLY_TRANSLATE):
        M00 = m00;
        M02 = m02;
        M11 = m11;
        M12 = m12;
        while (--numPts >= 0) {
            (*dstPts)[dstOff++] = static_cast< float >((M00 * srcPts[srcOff++] + M02));
            (*dstPts)[dstOff++] = static_cast< float >((M11 * srcPts[srcOff++] + M12));
        }
        return;
    case (APPLY_SCALE):
        M00 = m00;
        M11 = m11;
        while (--numPts >= 0) {
            (*dstPts)[dstOff++] = static_cast< float >((M00 * srcPts[srcOff++]));
            (*dstPts)[dstOff++] = static_cast< float >((M11 * srcPts[srcOff++]));
        }
        return;
    case (APPLY_TRANSLATE):
        M02 = m02;
        M12 = m12;
        while (--numPts >= 0) {
            (*dstPts)[dstOff++] = static_cast< float >((srcPts[srcOff++] + M02));
            (*dstPts)[dstOff++] = static_cast< float >((srcPts[srcOff++] + M12));
        }
        return;
    case (APPLY_IDENTITY):
        while (--numPts >= 0) {
            (*dstPts)[dstOff++] = static_cast< float >((srcPts[srcOff++]));
            (*dstPts)[dstOff++] = static_cast< float >((srcPts[srcOff++]));
        }
        return;
    }

}

Point2D* AffineTransform::inverseTransform(::Point2D* ptSrc, ::Point2D* ptDst)
{
    if(ptDst == nullptr) {
        if(dynamic_cast< ::Point2D_Double* >(ptSrc) != nullptr) {
            ptDst = new ::Point2D_Double();
        } else {
            ptDst = new ::Point2D_Float();
        }
    }
    auto x = ptSrc->getX();
    auto y = ptSrc->getY();
    {
        double det;
        switch (state) {
        default:
            stateError();
        case (APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE):
            x -= m02;
            y -= m12;
        case (APPLY_SHEAR | APPLY_SCALE):
            det = m00 * m11 - m01 * m10;
            if(abs(det) <= DBL_MIN) {
                //throw new ::NoninvertibleTransformException(::StringBuilder().append("Determinant is ")->append(det)->toString());
				throw std::invalid_argument("Non-invertible");
            }
            ptDst->setLocation((x * m11 - y * m01) / det, (y * m00 - x * m10) / det);
            return ptDst;
        case (APPLY_SHEAR | APPLY_TRANSLATE):
            x -= m02;
            y -= m12;
        case (APPLY_SHEAR):
            if(m01 == 0.0 || m10 == 0.0) {
                //throw new ::NoninvertibleTransformException("Determinant is 0");
				throw std::invalid_argument("Non-invertible");
            }
            ptDst->setLocation(y / m10, x / m01);
            return ptDst;
        case (APPLY_SCALE | APPLY_TRANSLATE):
            x -= m02;
            y -= m12;
        case (APPLY_SCALE):
            if(m00 == 0.0 || m11 == 0.0) {
                //throw new ::NoninvertibleTransformException("Determinant is 0");
				throw std::invalid_argument("Non-invertible");
            }
            ptDst->setLocation(x / m00, y / m11);
            return ptDst;
        case (APPLY_TRANSLATE):
            ptDst->setLocation(x - m02, y - m12);
            return ptDst;
        case (APPLY_IDENTITY):
            ptDst->setLocation(x, y);
            return ptDst;
        }
    }

}

void AffineTransform::inverseTransform(vector<double> srcPts, int srcOff, vector<double>* dstPts, int dstOff, int numPts)
{
	double M00, M01, M02, M10, M11, M12;
	double det;
	if (dstPts == &srcPts && dstOff > srcOff && dstOff < srcOff + numPts * 2) {
		moduru::VecUtil::VecCopy(&srcPts, srcOff, dstPts, dstOff, numPts * 2);
		srcOff = dstOff;
	}
	switch (state) {
	default:
		stateError();
	case (APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE):
		M00 = m00;
		M01 = m01;
		M02 = m02;
		M10 = m10;
		M11 = m11;
		M12 = m12;
		det = M00 * M11 - M01 * M10;
		if (abs(det) <= DBL_MIN) {
			//throw new ::NoninvertibleTransformException(::StringBuilder().append("Determinant is ")->append(det)->toString());
			throw std::invalid_argument("Non-invertible");
		}
		while (--numPts >= 0) {
			auto x = srcPts[srcOff++] - M02;
			auto y = srcPts[srcOff++] - M12;
			(*dstPts)[dstOff++] = (x * M11 - y * M01) / det;
			(*dstPts)[dstOff++] = (y * M00 - x * M10) / det;
		}
		return;
	case (APPLY_SHEAR | APPLY_SCALE):
		M00 = m00;
		M01 = m01;
		M10 = m10;
		M11 = m11;
		det = M00 * M11 - M01 * M10;
		if (abs(det) <= DBL_MIN) {
			//throw new ::NoninvertibleTransformException(::StringBuilder().append("Determinant is ")->append(det)->toString());
			throw std::invalid_argument("Non-invertible");
		}
		while (--numPts >= 0) {
			auto x = srcPts[srcOff++];
			auto y = srcPts[srcOff++];
			(*dstPts)[dstOff++] = (x * M11 - y * M01) / det;
			(*dstPts)[dstOff++] = (y * M00 - x * M10) / det;
		}
		return;
	case (APPLY_SHEAR | APPLY_TRANSLATE):
		M01 = m01;
		M02 = m02;
		M10 = m10;
		M12 = m12;
		if (M01 == 0.0 || M10 == 0.0) {
			//throw new ::NoninvertibleTransformException("Determinant is 0");
			throw std::invalid_argument("Non-invertible");
		}
		while (--numPts >= 0) {
			auto x = srcPts[srcOff++] - M02;
			(*dstPts)[dstOff++] = (srcPts[srcOff++] - M12) / M10;
			(*dstPts)[dstOff++] = x / M01;
		}
		return;
	case (APPLY_SHEAR):
		M01 = m01;
		M10 = m10;
		if (M01 == 0.0 || M10 == 0.0) {
			//throw new ::NoninvertibleTransformException("Determinant is 0");
			throw std::invalid_argument("Non-invertible");
		}
		while (--numPts >= 0) {
			auto x = srcPts[srcOff++];
			(*dstPts)[dstOff++] = srcPts[srcOff++] / M10;
			(*dstPts)[dstOff++] = x / M01;
		}
		return;
	case (APPLY_SCALE | APPLY_TRANSLATE):
		M00 = m00;
		M02 = m02;
		M11 = m11;
		M12 = m12;
		if (M00 == 0.0 || M11 == 0.0) {
			//throw new ::NoninvertibleTransformException("Determinant is 0");
			throw std::invalid_argument("Non-invertible");
		}
		while (--numPts >= 0) {
			(*dstPts)[dstOff++] = (srcPts[srcOff++] - M02) / M00;
			(*dstPts)[dstOff++] = (srcPts[srcOff++] - M12) / M11;
		}
		return;
	case (APPLY_SCALE):
		M00 = m00;
		M11 = m11;
		if (M00 == 0.0 || M11 == 0.0) {
			//throw new ::NoninvertibleTransformException("Determinant is 0");
			throw std::invalid_argument("Non-invertible");
		}
		while (--numPts >= 0) {
			(*dstPts)[dstOff++] = srcPts[srcOff++] / M00;
			(*dstPts)[dstOff++] = srcPts[srcOff++] / M11;
		}
		return;
	case (APPLY_TRANSLATE):
		M02 = m02;
		M12 = m12;
		while (--numPts >= 0) {
			(*dstPts)[dstOff++] = srcPts[srcOff++] - M02;
			(*dstPts)[dstOff++] = srcPts[srcOff++] - M12;
		}
		return;
	case (APPLY_IDENTITY):
		if (&srcPts != dstPts || srcOff != dstOff) {
			moduru::VecUtil::VecCopy(&srcPts, srcOff, dstPts, dstOff, numPts * 2);
		}
		return;
	}
}

Point2D* AffineTransform::deltaTransform(::Point2D* ptSrc, ::Point2D* ptDst)
{
    if(ptDst == nullptr) {
        if(dynamic_cast< ::Point2D_Double* >(ptSrc) != nullptr) {
            ptDst = new ::Point2D_Double();
        } else {
            ptDst = new ::Point2D_Float();
        }
    }
    auto x = ptSrc->getX();
    auto y = ptSrc->getY();
    switch (state) {
    default:
        stateError();
    case (APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE):
    case (APPLY_SHEAR | APPLY_SCALE):
        ptDst->setLocation(x * m00 + y * m01, x * m10 + y * m11);
        return ptDst;
    case (APPLY_SHEAR | APPLY_TRANSLATE):
    case (APPLY_SHEAR):
        ptDst->setLocation(y * m01, x * m10);
        return ptDst;
    case (APPLY_SCALE | APPLY_TRANSLATE):
    case (APPLY_SCALE):
        ptDst->setLocation(x * m00, y * m11);
        return ptDst;
    case (APPLY_TRANSLATE):
    case (APPLY_IDENTITY):
        ptDst->setLocation(x, y);
        return ptDst;
    }

}

void AffineTransform::deltaTransform(vector<double> srcPts, int srcOff, vector<double>* dstPts, int dstOff, int numPts)
{
	double M00, M01, M10, M11;
	if (dstPts == &srcPts && dstOff > srcOff && dstOff < srcOff + numPts * 2) {
		moduru::VecUtil::VecCopy(&srcPts, srcOff, dstPts, dstOff, numPts * 2);
		srcOff = dstOff;
	}
	switch (state) {
	default:
		stateError();
	case (APPLY_SHEAR | APPLY_SCALE | APPLY_TRANSLATE):
	case (APPLY_SHEAR | APPLY_SCALE):
		M00 = m00;
		M01 = m01;
		M10 = m10;
		M11 = m11;
		while (--numPts >= 0) {
			auto x = srcPts[srcOff++];
			auto y = srcPts[srcOff++];
			(*dstPts)[dstOff++] = x * M00 + y * M01;
			(*dstPts)[dstOff++] = x * M10 + y * M11;
		}
		return;
	case (APPLY_SHEAR | APPLY_TRANSLATE):
	case (APPLY_SHEAR):
		M01 = m01;
		M10 = m10;
		while (--numPts >= 0) {
			auto x = srcPts[srcOff++];
			(*dstPts)[dstOff++] = srcPts[srcOff++] * M01;
			(*dstPts)[dstOff++] = x * M10;
		}
		return;
	case (APPLY_SCALE | APPLY_TRANSLATE):
	case (APPLY_SCALE):
		M00 = m00;
		M11 = m11;
		while (--numPts >= 0) {
			(*dstPts)[dstOff++] = srcPts[srcOff++] * M00;
			(*dstPts)[dstOff++] = srcPts[srcOff++] * M11;
		}
		return;
	case (APPLY_TRANSLATE):
	case (APPLY_IDENTITY):
		if (&srcPts != dstPts || srcOff != dstOff) {
			moduru::VecUtil::VecCopy(&srcPts, srcOff, dstPts, dstOff, numPts * 2);
		}
		return;
	}
}

moduru::gui::Shape* AffineTransform::createTransformedShape(moduru::gui::Shape* pSrc)
{
    if(pSrc == nullptr) {
        return nullptr;
    }
    return new Path2D_Double(pSrc);
}

double AffineTransform::_matround(double matval)
{

    return rint(matval * 1.0E15) / 1.0E15;
}

/*
String* AffineTransform::toString()
{
    return (::StringBuilder().append("AffineTransform[[")->append(_matround(m00))
        ->append(", ")
        ->append(_matround(m01))
        ->append(", ")
        ->append(_matround(m02))
        ->append("], [")
        ->append(_matround(m10))
        ->append(", ")
        ->append(_matround(m11))
        ->append(", ")
        ->append(_matround(m12))
        ->append("]]")->toString());
}
*/

bool AffineTransform::isIdentity()
{
    return (state == APPLY_IDENTITY || (getType() == TYPE_IDENTITY));
}

/*
int AffineTransform::hashCode()
{
    auto bits = ::Double::doubleToLongBits(m00);
    bits = bits * 31 + ::Double::doubleToLongBits(m01);
    bits = bits * 31 + ::Double::doubleToLongBits(m02);
    bits = bits * 31 + ::Double::doubleToLongBits(m10);
    bits = bits * 31 + ::Double::doubleToLongBits(m11);
    bits = bits * 31 + ::Double::doubleToLongBits(m12);
    return ((static_cast< int >(bits)) ^ (static_cast< int >((bits >> 32))));
}

bool AffineTransform::equals(moduru::lang::Object* obj)
{
    if(!(dynamic_cast< AffineTransform* >(obj) != nullptr)) {
        return false;
    }
    auto a = dynamic_cast< AffineTransform* >(obj);
    return ((m00 == a->m00) && (m01 == a->m01) && (m02 == a->m02)&& (m10 == a->m10)&& (m11 == a->m11)&& (m12 == a->m12));
}
*/
/*
void AffineTransform::writeObject(::java::io::ObjectOutputStream* s)
{
    npc(s)->defaultWriteObject();
}

void AffineTransform::readObject(::java::io::ObjectInputStream* s)
{
    npc(s)->defaultReadObject();
    updateState();
}
*/
