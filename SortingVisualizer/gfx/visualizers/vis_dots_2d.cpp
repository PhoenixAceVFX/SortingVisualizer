#include "visualizers.h"
#if GFX_ENABLED

#define DEPTH_CONTRACT_SPD 2.0
#define DEPTH_EXPAND_SPD 4.0
#define DEPTH_GRACE_TIME 0.5

namespace {
	struct _ : public Visualizer {
		_() {
			name = "Dots (2D) Visualizer";
		}

		float maxDepth = 1.f;
		float graceTime = 0.f;

		void prepare(VisualizerListInfo* inf, List<int>* mainList, double elapsedSeconds) {
			if (inf->maxDepth >= (int)ceil(maxDepth)) {
				maxDepth = fmin(maxDepth + (float)((double)inf->maxDepth * elapsedSeconds * DEPTH_CONTRACT_SPD), inf->maxDepth);
				graceTime = 0.f;
			}
			else {
				if (graceTime < DEPTH_GRACE_TIME) {
					graceTime += (float)elapsedSeconds;
					double ovr = (double)graceTime - DEPTH_GRACE_TIME;
					elapsedSeconds = ovr;
				}
				if (graceTime >= DEPTH_GRACE_TIME)
					maxDepth = fmax(maxDepth - (float)((double)maxDepth * elapsedSeconds * DEPTH_EXPAND_SPD), inf->maxDepth);
			}
		}

		bool drawList(VisualizerListInfo* inf, List<int>* l, int depthInd, int depth) { return depth < maxDepth; }

		void drawElement(VisualizerListInfo* inf, List<int>* l, int depthInd, ListElement<int>* e, int pos, int depth, bool marked) {
			float height = (float)e->val / inf->realSize * 1.98f / maxDepth;
			float x = ((float)l->offset + (float)pos) / inf->realSize * 1.98f - 1.f;
			float y = -1.f + (float)depth / maxDepth * 1.98f + height;
			vec4 col = marked ? vec4(0.f, 0.f, 0.f, 1.f) : colHSV((float)e->val / inf->realSize, 1.f, 1.f);
			//drawShape(shape_square, vec3(x, y, 0.f), vec3(width, height, 1.f), vec3(), col);
			drawFontString2DCenteredCorrected(0, std::to_string(e->val), vec3(x, y, 0.f), vec3(0.5f, 0.5f, 0.5f), vec3(), col);
		}
	}_;
}

Visualizer* visualizer_dots_2d = &::_;

#endif