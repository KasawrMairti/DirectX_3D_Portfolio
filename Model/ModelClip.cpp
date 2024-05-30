#include "Framework.h"
#include "ModelClip.h"


ModelClip::ModelClip()
{
}

ModelClip::~ModelClip()
{
	for (pair<string, KeyFrame*> frame : keyFrames)
		delete frame.second;

	keyFrames.clear();
}

KeyFrame* ModelClip::GetKeyFrames(string name)
{
	if (keyFrames.count(name) == 0) return nullptr;

	return keyFrames[name];
}

void ModelClip::SetEndEvent(float ratio, function<void()> EndEvent)
{
	EndEvents.emplace(ratio, EndEvent);
}

void ModelClip::Init()
{
	eventIter = EndEvents.begin();
}

void ModelClip::Execute(float ratio)
{
	if (ratio > 0.9f) Init();

	if (EndEvents.empty()) return;

	if (eventIter == EndEvents.end()) return;

	if (eventIter->first > ratio) return;

	eventIter->second();
	eventIter++;
}
