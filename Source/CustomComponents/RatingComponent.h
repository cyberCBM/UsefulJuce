#include "JuceHeader.h"

#ifndef __RatingComponent__H__
#define __RatingComponent__H__

class RatingComponent;

class StarComponent :   public Component,
	                    public ActionBroadcaster
{
public:
	StarComponent(int indx);

	void paint(Graphics& g);

	void mouseDown(const MouseEvent&);

	void mouseEnter(const MouseEvent& event);

	void mouseExit(const MouseEvent& event);
	void setmouseEvents(int mdetails);
	bool getmouseclick();
private:
	bool mouseclick;
	bool mousein;
	int index;

};

class RatingComponent :public Component,
	public ActionListener
{
public:
	enum MouseDetails
	{
		Click,
		Enter,
		Exit,
		Unclick
	};
	RatingComponent(int no_of_compo);

	void paint(Graphics& g);
	void resized();
	void actionListenerCallback(const String & 	message);
	int getratting();
private:
	std::vector<StarComponent *> ratcomp;
	int lastClick;
};

#endif   // __RatingComponent__H__