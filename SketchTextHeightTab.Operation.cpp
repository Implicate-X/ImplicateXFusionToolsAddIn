#include "pch.h"
#include "resource.h"  
#include "ResourceHelper.h"
#include "Logging.h"
#include "ToolsBar.h"
#include "ToolsApp.h"
#include "ImplicateXFusionToolsAddIn.h"
#include "SketchTextCommandControl.h"
#include "SketchTextSettingsTab.h"
#include "SketchTextHeightTab.h"
#include "SketchTextPanel.h"

namespace implicatex {
	namespace fusion {
		/// <summary>
		/// <para>getTextSizeMatch retrieves and filters sketch texts based on specified minimum and maximum height,</para>
		/// <para>updating a command input with the count of matching texts.</para>
		/// </summary>
		///
		/// <param name="inputs">
		/// <para>is a constant reference to a smart pointer of type adsk::core::CommandInputs, </para>
		/// <para>which is typically used to manage command input parameters in the Autodesk Fusion 360 API.</para>
		/// </param>
		///
		/// <returns>True if it succeeds, false if it fails.</returns>
		bool SketchTextHeightTab::getTextHeightMatchItems(const Ptr<CommandInputs>& inputs, std::vector<Ptr<SketchText>>& filteredTexts) {
			Ptr<ValueCommandInput> minTextHeight = inputs->itemById(IDS_ITEM_TEXT_HEIGHT_MIN);
			Ptr<ValueCommandInput> maxTextHeight = inputs->itemById(IDS_ITEM_TEXT_HEIGHT_MAX);
			Ptr<TextBoxCommandInput> matchesTextHeightInput = inputs->itemById(IDS_ITEM_TEXT_HEIGHT_MATCH);
			Ptr<DropDownCommandInput> dropdown = inputs->itemById(IDS_ITEM_DROPDOWN_SELECT_SKETCH);

			Ptr<Sketch> sketch = nullptr;
			if (!toolsApp->sketchTextPanel->getSelectedSketch(dropdown, sketch)) {
				LOG_ERROR("Failed to get selected sketch");
				return false;
			}

			double minHeightValue = minTextHeight->value();
			double maxHeightValue = maxTextHeight->value();

			Ptr<SketchTexts> sketchTexts = sketch->sketchTexts();
			if (!sketchTexts) {
				LOG_ERROR("No SketchTexts found in the sketch.");
				return false;
			}

			for (size_t i = 0; i < sketchTexts->count(); ++i) {
				Ptr<SketchText> text = sketchTexts->item(i);
				Ptr<Point3D> minPoint = text->boundingBox()->minPoint();
				Ptr<Point3D> maxPoint = text->boundingBox()->maxPoint();
				if (!minPoint || !maxPoint) {
					LOG_ERROR("Failed to get bounding box points");
					return false;
				}

				Ptr<Point3D> center = Point3D::create(
					(minPoint->x() + maxPoint->x()) / 2.0,
					(minPoint->y() + maxPoint->y()) / 2.0,
					(minPoint->z() + maxPoint->z()) / 2.0
				);

				//NOLOG: LOG_INFO("Min: " + std::to_string(minPoint->x()) + ", " + std::to_string(minPoint->y()) + ", " + std::to_string(minPoint->z()));
				//NOLOG: LOG_INFO("Max: " + std::to_string(maxPoint->x()) + ", " + std::to_string(maxPoint->y()) + ", " + std::to_string(maxPoint->z()));
				//NOLOG: LOG_INFO("Center: " + std::to_string(center->x()) + ", " + std::to_string(center->y()) + ", " + std::to_string(center->z()));

				if (text) {
					double textHeight = text->height();
					if (textHeight >= minHeightValue && textHeight <= maxHeightValue) {
						filteredTexts.push_back(text);
					}
				}
			}

			size_t textHeightMatchCount = filteredTexts.size();

			matchesTextHeightInput->text(std::to_string(textHeightMatchCount));

			//for (const auto& text : filteredTexts) {
			//	TRACE("Filtered Text: " + text->text());
			//}
			return true;
		}

		unsigned int SketchTextHeightTab::getSelectedRowNumber(std::string& inputId) {
			unsigned int selectedRow = 0;
			std::smatch match;
			std::string expression = 
				std::format("^({}|{}|{}|{})_(\\d+)$", IDS_CELL_TEXT_ID, IDS_CELL_TEXT_VALUE, IDS_CELL_TEXT_HEIGHT, IDS_CELL_TEXT_TOGGLE);

			if (std::regex_match(inputId, match, std::regex(expression))) {
				selectedRow = std::stoi(match[2]);
			}

			return selectedRow;
		}

		Ptr<SketchText> SketchTextHeightTab::getTextById(const unsigned int id) const
		{
			auto it = idTextMap_.find(id);
			if (it != idTextMap_.end())
				return it->second;
			return nullptr;
		}

		SketchTextHeightTab* SketchTextHeightTab::get() { 
			return toolsApp->sketchTextPanel->getTextHeightTab().get();
		}
	}
}
