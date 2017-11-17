#pragma once

#include <gui/geom/Dimension2D.hpp>

namespace moduru {
	namespace gui {

		class Dimension
			: public moduru::gui::geom::Dimension2D
		{

		public:
			typedef moduru::gui::geom::Dimension2D super;
			unsigned int width;
			unsigned int height;

		public:
			double getWidth() override;
			double getHeight() override;
			void setSize(double width, double height) override;
			virtual Dimension* getSize();
			virtual void setSize(Dimension* d);
			virtual void setSize(unsigned int width, unsigned int height);
			//bool equals(moduru::lang::Object* obj) override;
			//int hashCode() override;
			//::String* toString() override;

			Dimension();
			Dimension(Dimension* d);
			Dimension(unsigned int width, unsigned int height);

		};

	}
}
