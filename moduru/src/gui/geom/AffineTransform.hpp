#pragma once

#include <vector>

namespace moduru {
	namespace gui {

		class Shape;

		namespace geom {

			class Point2D;

			class AffineTransform
				{

			private:
				static const int TYPE_UNKNOWN{ -1 };

			public:
				static const int TYPE_IDENTITY{ 0 };
				static const int TYPE_TRANSLATION{ 1 };
				static const int TYPE_UNIFORM_SCALE{ 2 };
				static const int TYPE_GENERAL_SCALE{ 4 };
				static const int TYPE_MASK_SCALE{ 6 };
				static const int TYPE_FLIP{ 64 };
				static const int TYPE_QUADRANT_ROTATION{ 8 };
				static const int TYPE_GENERAL_ROTATION{ 16 };
				static const int TYPE_MASK_ROTATION{ 24 };
				static const int TYPE_GENERAL_TRANSFORM{ 32 };

			public: 
				static const int APPLY_IDENTITY{ 0 };
				static const int APPLY_TRANSLATE{ 1 };
				static const int APPLY_SCALE{ 2 };
				static const int APPLY_SHEAR{ 4 };

			private:
				static const int HI_SHIFT{ 3 };
				static const int HI_IDENTITY{ 0 };
				static const int HI_TRANSLATE{ 8 };
				static const int HI_SCALE{ 16 };
				static const int HI_SHEAR{ 32 };

			public: 
				double m00{};
				double m10{};
				double m01{};
				double m11{};
				double m02{};
				double m12{};
				int state{};

			private:
				int type{};

			public:
				static AffineTransform* getTranslateInstance(double tx, double ty);
				static AffineTransform* getRotateInstance(double theta);
				static AffineTransform* getRotateInstance(double theta, double anchorx, double anchory);
				static AffineTransform* getRotateInstance(double vecx, double vecy);
				static AffineTransform* getRotateInstance(double vecx, double vecy, double anchorx, double anchory);
				static AffineTransform* getQuadrantRotateInstance(int numquadrants);
				static AffineTransform* getQuadrantRotateInstance(int numquadrants, double anchorx, double anchory);
				static AffineTransform* getScaleInstance(double sx, double sy);
				static AffineTransform* getShearInstance(double shx, double shy);
				virtual int getType();

			private:
				void calculateType();

			public:
				virtual double getDeterminant();

			public:
				virtual void updateState();

			private:
				void stateError();

			public:
				virtual void getMatrix(std::vector<double>* flatmatrix);
				virtual double getScaleX();
				virtual double getScaleY();
				virtual double getShearX();
				virtual double getShearY();
				virtual double getTranslateX();
				virtual double getTranslateY();
				virtual void translate(double tx, double ty);

			private:
				static std::vector<int> rot90conversion;
				void rotate90();
				void rotate180();
				void rotate270();

			public:
				virtual void rotate(double theta);
				virtual void rotate(double theta, double anchorx, double anchory);
				virtual void rotate(double vecx, double vecy);
				virtual void rotate(double vecx, double vecy, double anchorx, double anchory);
				virtual void quadrantRotate(int numquadrants);
				virtual void quadrantRotate(int numquadrants, double anchorx, double anchory);
				virtual void scale(double sx, double sy);
				virtual void shear(double shx, double shy);
				virtual void setToIdentity();
				virtual void setToTranslation(double tx, double ty);
				virtual void setToRotation(double theta);
				virtual void setToRotation(double theta, double anchorx, double anchory);
				virtual void setToRotation(double vecx, double vecy);
				virtual void setToRotation(double vecx, double vecy, double anchorx, double anchory);
				virtual void setToQuadrantRotation(int numquadrants);
				virtual void setToQuadrantRotation(int numquadrants, double anchorx, double anchory);
				virtual void setToScale(double sx, double sy);
				virtual void setToShear(double shx, double shy);
				virtual void setTransform(AffineTransform* Tx);
				virtual void setTransform(double m00, double m10, double m01, double m11, double m02, double m12);
				virtual void concatenate(AffineTransform* Tx);
				virtual void preConcatenate(AffineTransform* Tx);
				virtual AffineTransform* createInverse();
				virtual void invert();
				virtual moduru::gui::geom::Point2D* transform(moduru::gui::geom::Point2D* ptSrc, moduru::gui::geom::Point2D* ptDst);
				virtual void transform(std::vector<moduru::gui::geom::Point2D*> ptSrc, int srcOff, std::vector<moduru::gui::geom::Point2D*>* ptDst, int dstOff, int numPts);
				virtual void transform(std::vector<float> srcPts, int srcOff, std::vector<float>* dstPts, int dstOff, int numPts);
				virtual void transform(std::vector<double> srcPts, int srcOff, std::vector<double>* dstPts, int dstOff, int numPts);
				virtual void transform(std::vector<float> srcPts, int srcOff, std::vector<double>* dstPts, int dstOff, int numPts);
				virtual void transform(std::vector<double> srcPts, int srcOff, std::vector<float>* dstPts, int dstOff, int numPts);
				virtual moduru::gui::geom::Point2D* inverseTransform(moduru::gui::geom::Point2D* ptSrc, moduru::gui::geom::Point2D* ptDst);
				virtual void inverseTransform(std::vector<double> srcPts, int srcOff, std::vector<double>* dstPts, int dstOff, int numPts);
				virtual moduru::gui::geom::Point2D* deltaTransform(moduru::gui::geom::Point2D* ptSrc, moduru::gui::geom::Point2D* ptDst);
				virtual void deltaTransform(std::vector<double> srcPts, int srcOff, std::vector<double>* dstPts, int dstOff, int numPts);
				virtual moduru::gui::Shape* createTransformedShape(moduru::gui::Shape* pSrc);

			private:
				static double _matround(double matval);

			public:
				//::String* toString() override;
				virtual bool isIdentity();
				//moduru::lang::Object* clone() override;
				//int hashCode() override;
				//bool equals(moduru::lang::Object* obj) override;

			private:
				//void writeObject(::java::io::ObjectOutputStream* s);
				//void readObject(::java::io::ObjectInputStream* s);

				AffineTransform(double m00, double m10, double m01, double m11, double m02, double m12, int state);

			public:
				AffineTransform();
				AffineTransform(AffineTransform* Tx);
				AffineTransform(float m00, float m10, float m01, float m11, float m02, float m12);
				AffineTransform(std::vector<float>* flatmatrix);
				AffineTransform(double m00, double m10, double m01, double m11, double m02, double m12);
				AffineTransform(std::vector<double>* flatmatrix);
			};

		}
	}
}
