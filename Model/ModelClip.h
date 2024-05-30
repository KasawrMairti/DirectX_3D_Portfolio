#pragma once

class ModelClip
{
	friend class ModelAnimator;
	friend class ModelAnimatorInstancing;
public:
	ModelClip();
	~ModelClip();

	KeyFrame* GetKeyFrames(string name);

	void SetEndEvent(float ratio, function<void()> EndEvent);

	void Init();
	void Execute(float ratio);

private:
	string name = "";

	UINT frameCount = 0;

	float ticksPerSecond = 0.0f;
	float duration		 = 0.0f;

	unordered_map<string, KeyFrame*> keyFrames = {};

	multimap<float, function<void()>> EndEvents;
	multimap<float, function<void()>>::iterator eventIter;

	float ratio;


};